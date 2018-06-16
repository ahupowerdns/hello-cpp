#include <thread>
#include <future>
#include <iostream>

double factorial(unsigned int limit)
{
        double ret = 1;
        for(unsigned int n = 1 ; n <= limit ; ++n)
                ret *= n;
        return ret;
}


int main()
{
      auto future1 = std::async(factorial, 19);
      auto future2 = std::async(factorial, 12);      
      double result = future1.get() + future2.get();
      
      std::cout<<"Result is: " << result << std::endl;
}

