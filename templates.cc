#include <cstddef>
#include <cstdint>

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
  for(unsigned int n = 0 ; n < 1000000000; ++n) {
    v.push_back(n);
  }
}
