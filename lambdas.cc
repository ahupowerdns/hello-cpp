#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <functional>

using std::vector;
using std::string;
using std::cout;
using std::endl;

int main()
{
  vector<std::string> v{"9", "12", "13", "14", "18", "42", "75"};

  string prefix("hi ");
  
  for_each(v.begin(), v.end(), [&prefix](const auto& a) {
      cout << prefix + a << endl;
    });

  auto print = [](const vector<std::string>& c) {
    for(const auto& a : c)
        cout << a << endl;
  };

  std::function<void(const vector<std::string>&)> stored=print;
    
  cout<<"Starting order: "<<endl;
  print(v);
  cout<<typeid(print).name()<<endl;
  cout<<typeid(stored).name()<<endl;

  cout<<"Ascii sort: "<<endl;
  std::sort(v.begin(), v.end());

  print(v);

  cout<<"Numerical sort: "<<endl;
  std::sort(v.begin(), v.end(), [](const std::string& a, const std::string& b)
            {
              return atoi(a.c_str()) < atoi(b.c_str());
            });

  print(v);

  cout<<"Absolute sort"<<endl;
  
  std::vector<int> v2{3, -1, -4, 1, -5, -9, -2, 6, -5};
  qsort(&v2[0], v2.size(), sizeof(int), [](const void *a, const void* b)
        {
          if(abs(*(int*)a) < abs(*(int*)b))
            return -1;
          if(abs(*(int*)a) > abs(*(int*)b))
            return 1;
          return 0;
        });

  for(const auto& val : v2)
    cout<<val<<endl;
}
