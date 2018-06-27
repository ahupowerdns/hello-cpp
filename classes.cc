#include <string>
#include <time.h>
#include <iostream>

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
  PortScanEvent() 
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
  PortScanEvent pse;
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
 
  
}
