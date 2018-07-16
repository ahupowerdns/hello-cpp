#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/key_extractors.hpp>
#include <iostream>
using namespace std;

typedef uint32_t IPAddress;

using namespace ::boost::multi_index;

struct AddressTupleTag{};
struct TimeTag{};

struct Entry
{
  IPAddress srcIP;
  uint16_t srcPort;
  IPAddress dstIP;
  uint16_t dstPort;
  double time;
};

typedef multi_index_container<
  Entry,
  indexed_by<
    ordered_unique<
        tag<AddressTupleTag>,
        composite_key<Entry,
          member<Entry, IPAddress, &Entry::srcIP>,
          member<Entry, uint16_t, &Entry::srcPort>,
          member<Entry, IPAddress, &Entry::dstIP>,
          member<Entry, uint16_t, &Entry::dstPort> >
        >,
      ordered_non_unique<
        tag<TimeTag>,
        member<Entry, double, &Entry::time>
        >
      >
    > tcpsessions_t;

int main()
{
  tcpsessions_t sessions;
  double now = time(0);
  Entry e{0x01020304, 80, 0x04030201, 123, now};

  sessions.insert(e);
  e = Entry{0x01020305, 80, 0x04030201, 1323, now+1.0};

  auto range = sessions.equal_range(std::make_tuple(0x01020304));
  for(auto iter = range.first; iter != range.second ; ++iter)
    cout << iter->srcIP << ":" << iter->srcPort<< " -> "<< iter->dstIP <<":"<<iter->dstPort<<endl;
}
