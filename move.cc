#include <memory>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main(int argc, char **argv)
{
  std::unique_ptr<uint32_t> testUnique;
  std::shared_ptr<uint32_t> testShared;
  uint32_t* testRaw;
  cout<<"sizeof(testUnique): "<<sizeof(testUnique)<<endl;
  cout<<"sizeof(testRaw): "<<sizeof(testRaw)<<endl;
  cout<<"sizeof(testShared): "<<sizeof(testShared)<<endl;

  if(argc < 2) {
    cerr<<"Syntax: move u|s|ms|r|n"<<endl;
    cerr<<"To test unique_ptr, shared_ptr, std::make_shared, raw and no pointers"<<endl;
    exit(1);
  }
  
  if(*argv[1]=='u') {
    vector<std::unique_ptr<uint32_t>> vec;
    vec.reserve(10000000);
    for(unsigned int n = 0 ; n < 10000000; ++n)
      vec.emplace_back(std::make_unique<uint32_t>(n));
    
    double val=0;
    for(const auto& e : vec)
      val += *e;
    
    cout << val / vec.size() << endl;

  }
  else if(*argv[1]=='s') {
    vector<std::shared_ptr<uint32_t>> vec;
    vec.reserve(10000000);
    for(unsigned int n = 0 ; n < 10000000; ++n) {
      vec.emplace_back(std::shared_ptr<uint32_t>(new uint32_t(n)));
    }
    
    double val=0;
    for(const auto& e : vec)
      val += *e;
    
    cout << val / vec.size() << endl;

  }
  else if(string(argv[1])=="ms") {
    vector<std::shared_ptr<uint32_t>> vec;
    vec.reserve(10000000);
    for(unsigned int n = 0 ; n < 10000000; ++n) {
      vec.emplace_back(std::make_shared<uint32_t>(n));
    }
    
    double val=0;
    for(const auto& e : vec)
      val += *e;
    
    cout << val / vec.size() << endl;

  }

  else if(*argv[1]=='r') {
    vector<uint32_t*> vec;
    vec.reserve(10000000);
    for(unsigned int n = 0 ; n < 10000000; ++n)
      vec.emplace_back(new uint32_t(n));
    
    double val=0;
    for(const auto& e : vec)
      val += *e;
    
    cout << val / vec.size() << endl;
    for(auto& e : vec)
      delete e;
  }
  else if(*argv[1]=='n') {
    vector<uint32_t> vec;
    vec.reserve(10000000);
    for(unsigned int n = 0 ; n < 10000000; ++n)
      vec.emplace_back(n);
    
    double val=0;
    for(const auto& e : vec)
      val += e;
    
    cout << val / vec.size() << endl;
  }
  else
    cerr<<"Unknown mode "<<argv[1]<<endl;

}
