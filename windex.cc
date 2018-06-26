#include <string>
#include <string.h>
#include <stdexcept>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <fstream>

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
  str="";
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
};

int main(int argc, char** argv)
{
  vector<string> filenames;
  ifstream ifs(argv[1]);
  std::string line;
  while(getline(ifs, line))
    filenames.push_back(line);
  cout<<"Have "<<filenames.size()<<" files"<<endl;
  unsigned long wordcount=0, bytecount=0;

  unordered_map<string, vector<Location>> allWords;

  unsigned int fileno=0;
  for(const auto& fname : filenames) {
    cout<<"\rFile: "<<fname<<", have "<<wordcount<<" words, "<<allWords.size()<<" different"<<"..";
    cout.flush();
    SmartFP sfp(fname.c_str(), "r");
    std::string word;
    size_t offset=0;
    while(getWord(sfp.d_fp, word, offset)) {
      ++wordcount;
      allWords[word].push_back({fileno, offset});
      //      cout<<word<<"\n";
    }
    bytecount += ftell(sfp.d_fp);
    ++fileno;
  }
  cout<<"\nRead "<<bytecount<<" bytes"<<endl;

  while(getline(cin, line)) {
    auto iter = allWords.find(line);
    if(iter == allWords.end()) {
      cout<<"Word '"<<line<<"' was not found"<<endl;
      continue;
    }
    cout<<"Word '"<<line<<"' occurred "<<iter->second.size()<<" times: "<<endl;
    for(const auto& l : iter->second) {
      cout<<"\tFile "<<filenames[l.fileno]<<", offset "<<l.offset<<endl;
    }
    cout<<"\tEnd of "<<iter->second.size()<<" hits"<<endl;
  }
    
  
}
