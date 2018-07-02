#include <cstddef>
#include <cstdint>
#include <iostream>
using std::cout;
using std::endl;

template<typename T>
struct Vector
{
  void push_back(const T& t)
  {
    if(size + 1 > capacity) {
      if(capacity == 0) 
        capacity = 1;
      auto newcontents = new T[capacity *= 2];
      for(size_t a = 0 ; a < size ; ++a)
        newcontents[a]=contents[a];
      delete[] contents;
      contents=newcontents;
    }
    contents[size++] = t;
  }
  T& operator[](size_t pos)
  {
    return contents[pos];
  }
  template<typename C>
  bool isSorted(C pred)
  {
    if(!size)
      return true;
    for(size_t n=0; n < size - 1; ++n)
      if(!pred(contents[n], contents[n+1]))
        return false;
    return true;
  }


  ~Vector()
  {
    delete[] contents;
  }
  
  size_t size{0}, capacity{0};
  T* contents{nullptr};
};


int main()
{
  Vector<uint32_t> v;
  for(unsigned int n = 0 ; n < 10; ++n) {
    v.push_back(n);
  }
  cout<<v.size<<endl;
  
  cout<< v.isSorted([](const auto& a, const auto& b) {
      return a < b; });
}
