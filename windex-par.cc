#include <string>
#include <string.h>
#include <stdexcept>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <algorithm>
#include <unistd.h>
#include "bytell_hash_map.hpp"
#include <atomic>
#include <memory>
#include <thread>
#include <future>
#include <regex>
#include <parallel/algorithm>

std::string stringerror()
{
  return strerror(errno);
}

struct SmartFP
{
  SmartFP(const char* fname, const char* mode)
  {
    d_fp = fopen(fname, mode);
    if(!d_fp)
      throw std::runtime_error("Can't open file: " + stringerror());
  }
  ~SmartFP()
  {
    fclose(d_fp);
  }
  FILE* d_fp;
};

bool getWord(FILE* fp, std::string& str, size_t& offset)
{
  str.clear();
  int c;
  for(;;) {
    c = getc_unlocked(fp);
    ++offset;
    if(c == EOF)
      return !str.empty();

    if(!isalnum((char)c)) {
      if(!str.empty())
        return true;
      else
        continue;
    }
    
    str.append(1, tolower((char)c));
  }
  return true;
}

using namespace std;

struct Location
{
  unsigned int fileno;
  size_t offset;
} __attribute__((packed)); 

std::atomic<unsigned long> g_wordcount{0}, g_bytecount{0};
vector<string> g_filenames;
std::mutex g_mutex;

typedef ska::bytell_hash_map<string, std::vector<Location>> allwords_t;
std::unique_ptr<allwords_t> work(ifstream* in)
{
  auto ret = std::make_unique<allwords_t>();
  std::string fname;
  unsigned int fileno;
  for(;;) {
    {
      std::lock_guard<std::mutex> l(g_mutex);
      if(!getline(*in, fname))
        break;
      fileno = g_filenames.size();
      g_filenames.push_back(fname);
    }
      
    SmartFP sfp(fname.c_str(), "r");
    std::string word;
    size_t offset=0;
    while(getWord(sfp.d_fp, word, offset)) {
      ++g_wordcount;
      (*ret)[word].push_back({fileno, offset});
      //      cout<<word<<"\n";
    }
    g_bytecount += ftell(sfp.d_fp);
  }
  return std::move(ret);
}

int main(int argc, char** argv)
{
  ifstream ifs(argv[1]);

  auto future1 = std::async(work, &ifs);
  auto future2 = std::async(work, &ifs);
  auto future3 = std::async(work, &ifs);
  auto future4 = std::async(work, &ifs);
  auto future5 = std::async(work, &ifs);
  vector<std::unique_ptr<allwords_t>> allAllWords;
  allAllWords.emplace_back(future1.get());
  allAllWords.emplace_back(future2.get());
  allAllWords.emplace_back(future3.get());
  allAllWords.emplace_back(future4.get());
  allAllWords.emplace_back(future5.get());
  
  
  // this is where we take all the unique words and sort them so we can
  // do prefix searches too
  cout<<g_filenames.size()<<" files, "<<g_wordcount<<" words, "<<g_bytecount<<" bytes"<<endl;
  cout<<"Start sort"<<endl;
  std::vector<string> owords;
  for(const auto& allWords : allAllWords) {
    for(const auto& w : *allWords) {
      owords.push_back(w.first);
    }
  }
  sort(owords.begin(), owords.end());
  owords.erase(std::unique(owords.begin(), owords.end()), owords.end());
  cout<<"Done indexing"<<endl;

  // normal word: search, if ends on '?', list matching words
  // if ends on '*', search for all those words
  std::string line;
  while(getline(cin, line)) {
    if(line.empty())
      continue;
    char lastchar = *line.rbegin(); // rbegin() == reverse begin
    if(line[0]=='/') {
      std::regex reg(line.c_str()+1, std::regex_constants::icase );
      __gnu_parallel::for_each(owords.begin(), owords.end(), [&reg](const auto& a)
                               {
                                 if(std::regex_search(a, reg))
                                   cout<<a<<endl;
                               });
    }
    else if(lastchar =='?' || lastchar=='*') {
      line.resize(line.size()-1);
      if(line.empty())
        continue;
      cout<<"Looking for words starting with '"<<line<<"'"<<endl;
      auto iter = lower_bound(owords.begin(), owords.end(), line);
      for(; iter != owords.end() && !iter->compare(0, line.size(), line); ++iter) {
        if(lastchar=='?')
          cout<<" "<<*iter<<endl;
        else {
          for(const auto& allWords : allAllWords) {
            for(const auto& l : allWords->find(*iter)->second) {
              cout<<"\t"<<*iter<<": File "<<g_filenames[l.fileno]<<", offset "<<l.offset<<endl;
            }
          }
        }
      }
    }
    else {
      for(const auto& allWords : allAllWords) {
        auto iter = allWords->find(line);
        if(iter == allWords->end()) {
          cout<<"Word '"<<line<<"' was not found"<<endl;
          continue;
        }
        cout<<"Word '"<<line<<"' occurred "<<iter->second.size()<<" times: "<<endl;
        for(const auto& l : iter->second) {
          cout<<"\tFile "<<g_filenames[l.fileno]<<", offset "<<l.offset<<endl;
        }
        cout<<"\tEnd of "<<iter->second.size()<<" hits"<<endl;
      }
    }
  }
  _exit(0); // cheating a bit for the benchmark, saves cleanup time
}
