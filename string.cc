#include <cstdlib>
#include <iostream>
#include <string>
#include <boost/container/string.hpp>

// replace operator new and delete to log allocations
void* operator new(std::size_t n) {
    std::cout << "[Allocating " << n << " bytes]";
    return malloc(n);
}
void operator delete(void* p) throw() {
    free(p);
}

int main() {
    typedef boost::container::string string;
    std::cout << sizeof(string) << std::endl;
    string r;
    for (size_t i = 0; i < 64; ++i) {
        std::cout << i << ": " << r.append(1, '=') << std::endl;
    }
}
