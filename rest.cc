#include <iostream>
#include <limits>
#include <fstream>
#include <regex>
#include <optional>

using namespace std;

struct Event
{};
  
struct Policy
{
  bool check() { return false; }
}; 
  
struct Channel
{
  enum class Priority { Low, Medium, High};
  void sendEvent(const Event& e);
  void sendEvent(const Event& e, Priority p);
  void sendEvent2(const Event& e, std::optional<Priority> p={})
  {
    if(!p)
      cout << "No priority passed" << endl;
    switch(*p) {
      case Priority::Low:
        cout << "Low!" << endl; break;
      case Priority::Medium:     
        cout << "Medium!" << endl; break;      
      case Priority::High:
        cout << "High!" << endl; break;      
      ;
    }
  }
  
  std::optional<Policy> getPolicy(const Event& e)
  {
    return std::nullopt;
  }
};


#include <type_traits>

template<typename T>
bool isSet(const T& t)
{
  static_assert(std::is_pointer<T>(), "isSet requires a pointer");
  return t != nullptr;
}

int main(int argc, char** argv)
{
  static_assert(sizeof(int) == 4, "Int must be 4 bytes");

  if(isSet(argv))
    cout << "Yes, is set"<<endl;

  
  int i;
  cout << std::numeric_limits<int>::min() << endl;
  cout << std::numeric_limits<decltype(i)>::max() << endl;

  ifstream in("/etc/passwd");
  if (!in.is_open()) {
    std::cout << "failed to open\n";
  } else {
    string line;
    int count = 0;
    while(getline(in, line)) 
      ++count;
    cout << "Read " << count << " lines\n";
  }
  
  vector<int> vi{1, 2, 3, 4};
  vector<string> vs{"foo", "bar", "baz"};

  auto printAverages = [](int i){};

  for(auto a : { 1, 10, 15} )
    printAverages(a);

  auto doAverage = [](vector<double> v) {};

  vector<double> x, y, z;
  for(auto& a : {x, y, z})
    doAverage(a);

  struct Coordinate
  {
        double x, y;
  };


  Coordinate coord{-1.0, 1.0};

  auto [xc, yc] = coord;
  xc = yc = 0;
  
  std::string s = R"foo(Some people, when confronted with a problem, think: 
"I know, I'll use regular expressions."
Now they have two problems.)foo";

  cout << s << endl;
  std::regex word_regex("(\\S+)");
  auto words_begin = 
        std::sregex_iterator(s.begin(), s.end(), word_regex);
  auto words_end = std::sregex_iterator();
 
  std::cout << "Found "
            << std::distance(words_begin, words_end)
            << " words\n";

  for_each(words_begin, words_end, [](const auto& w) {
    cout << "Word: " << w.str() << endl;
  });


  Channel c;
  Event e1, e2;
  
  c.sendEvent2(e1);
  c.sendEvent2(e2, Channel::Priority::High);
  
  std::optional<Channel::Priority> p;
  if(!p)
    cout << "Priority not set" << endl;
  p = Channel::Priority::High;
  if(*p == Channel::Priority::High)
    cout << "Priority is now set to High" << endl;
    
  if(auto policy = c.getPolicy(e1)) {
    if(policy->check())
      c.sendEvent(e1);

  }
  else
    cout << "No policy was set" <<endl;  
    
  if(auto policy = c.getPolicy(e1); policy->check())
    c.sendEvent(e1);
  
}

