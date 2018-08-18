#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/key_extractors.hpp>
#include <iostream>
using namespace std;

struct IPAddress
{
  constexpr IPAddress() : addr(0) {}
  IPAddress(uint32_t addr_) : addr(addr_) {}
  uint32_t addr;
  bool operator<(const IPAddress& rhs) const
  {
    return addr < rhs.addr;
  }

};

std::ostream& operator<<(std::ostream& ios, const IPAddress& ip) 
{
  cout<< (ip.addr & 0xff) << "." << ((ip.addr >> 8) & 0xff) << "." << ((ip.addr >> 16) & 0xff) << "." << ((ip.addr >> 24) & 0xff);
  return ios;
}

constexpr uint32_t chtonl(uint32_t s)
{
    return 
        ((s & 0x000000FF) << 24) | ((s & 0x0000FF00) << 8)
      | ((s & 0xFF000000) >> 24) | ((s & 0x00FF0000) >> 8);
    
}

constexpr IPAddress operator "" _ipv4(const char* p, size_t l)
{
  IPAddress ret;
  ret.addr=0;
  
  uint8_t octet=0;

  size_t n=0;
  for(; n < l; ++n) {
    if(p[n]==':')
      break;
    if(p[n]=='.') {
      ret.addr*=0x100;
      ret.addr+=octet;
      octet=0;
    }
    else {
      octet*=10;
      octet+=p[n]-'0';
    }
  }
  ret.addr*=0x100;
  ret.addr+=octet;

  ret.addr = chtonl(ret.addr);
  return ret;
}

using namespace ::boost::multi_index;

struct AddressTupleTag{};
struct DestTag{};
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
      tag<DestTag>,
      composite_key<Entry,
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
  Entry e{"1.2.3.4"_ipv4, 80, "4.3.2.1"_ipv4, 123, now};

  sessions.insert(e);
  sessions.insert({"1.2.3.4"_ipv4, 81, "4.3.2.5"_ipv4, 1323, now+1.0});
  sessions.insert({"1.2.3.5"_ipv4, 80, "4.3.2.2"_ipv4, 4215, now+2.0});

  cout<<"Search for source 1.2.3.4, every port"<<endl;
  auto range = sessions.equal_range(std::make_tuple("1.2.3.4"_ipv4));
  for(auto iter = range.first; iter != range.second ; ++iter)
    cout << iter->srcIP << ":" << iter->srcPort<< " -> "<< iter->dstIP <<":"<<iter->dstPort<<endl;

  cout<<"Second search for src 1.2.3.5 port 80: "<<endl;
  range = sessions.equal_range(std::make_tuple("1.2.3.5"_ipv4, 80));
  for(auto iter = range.first; iter != range.second ; ++iter)
    cout << iter->srcIP << ":" << iter->srcPort<< " -> "<< iter->dstIP <<":"<<iter->dstPort<<endl;

  cout<<"Destination search for 4.3.2.1 port 123: "<<endl;
  auto range2 = sessions.get<DestTag>().equal_range(std::make_tuple("4.3.2.1"_ipv4, 123));
  for(auto iter = range2.first; iter != range2.second ; ++iter)
    cout << iter->srcIP << ":" << iter->srcPort<< " -> "<< iter->dstIP <<":"<<iter->dstPort<<endl;

  
  cout<<"In time order: "<<endl;
  
  auto& idx = sessions.get<TimeTag>();
  for(auto iter = idx.begin(); iter != idx.end(); ++iter)
    cout << iter->srcIP << ":" << iter->srcPort<< " -> "<< iter->dstIP <<":"<<iter->dstPort<<endl;
}
