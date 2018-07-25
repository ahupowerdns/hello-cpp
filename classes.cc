#include <string>
#include <time.h>
#include <iostream>
#include <deque>
#include <memory>

class Event
{
public:
  std::string getType();
  struct timeval getTime();
  virtual std::string getDescription()=0;
  
protected:
  std::string m_type;
  struct timeval m_time;
};

std::string Event::getType()
{
  return m_type;
}

class PortScanEvent : public Event
{
public:
  PortScanEvent(std::string fromIP) : m_fromIP(fromIP) 
  {
    m_type = "Portscan";
  }
  virtual std::string getDescription() override
  {
    return "Port scan from "+m_fromIP;
  }       
private:
  std::string m_fromIP;
};

class ICMPEvent : public Event
{
public:
  ICMPEvent() 
  {
    m_type = "ICMP";
  }
  virtual std::string getDescription() override
  {
    return "ICMP type "+std::to_string(m_icmptype);
  }       
private:
  int m_icmptype;
};


using namespace std;

int main()
{
  PortScanEvent pse("1.2.3.4");
  cout << pse.getType() << endl;
  ICMPEvent ice;
  cout << ice.getType() << endl;

  Event* e = &ice;
  cout << e->getDescription() <<endl;

  e = &pse;
  cout << e->getDescription() <<endl;

  auto ptr = dynamic_cast<PortScanEvent*>(e);
  if(ptr) {
    cout<<"This is a PortScanEvent"<<endl;
  }


  std::deque<std::unique_ptr<Event>> eventQueue;

  eventQueue.push_back(std::make_unique<PortScanEvent>("1.2.3.4"));
  eventQueue.push_back(std::make_unique<ICMPEvent>());

  for(const auto& e : eventQueue) {
    cout << e->getDescription() << endl;
  }
  
}
