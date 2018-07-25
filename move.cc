#include <memory>
#include <iostream>
#include <vector>
#include <string.h>
#include <string>

using namespace std;

void function()
{
  auto uptr = std::make_unique<uint32_t>(42);

  cout << *uptr << endl;

  std::unique_ptr<uint32_t> uptr2 = std::unique_ptr<uint32_t>(new uint32_t(42));

  //  uptr2 = uptr;
  cout << (void*)uptr.get() << endl; // prints address

  cout << (void*)*reinterpret_cast<uint32_t**>(&uptr) << endl;
  
  uptr2 = std::move(uptr);

  cout << (void*)uptr.get() << endl; // prints 0

  cout << (void*)uptr2.get() << endl; // prints same address


  std::unique_ptr<int> iptr;
  auto p = [](const auto& a) {
    cout << "pointer is " << (a ? "" : "not ") << "set\n";
  };

  p(iptr);
  cout << (void*) iptr.get() << endl;  // prints 0
  iptr = std::make_unique<int>(12);
  p(iptr);
  iptr.reset();
  p(iptr);
  
}

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
  SmartFP(SmartFP&& src)
  {
    d_fp = src.d_fp;
    src.d_fp = 0;
  }
  ~SmartFP()
  {
    if(d_fp)
      fclose(d_fp);
  }
  FILE* d_fp{0};
};

SmartFP getTmpFP()
{
  return SmartFP("/tmp/tmp", "w");
}


int main(int argc, char **argv)
{
  SmartFP sfp("/etc/passwd", "r");
  SmartFP sfp2(std::move(sfp));

  vector<SmartFP> vec;
  vec.emplace_back("move.cc", "r");

  SmartFP sfp3 = getTmpFP();
  
  function();
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
