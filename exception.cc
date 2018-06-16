#include <iostream>
#include <string.h>
#include <stdio.h>
#include <iostream>

using namespace std;

std::string stringerror()
{
    return strerror(errno);
}

struct SmartFP
{
	SmartFP(const char* fname, const char* mode)
	{
		d_fp = fopen(fname, mode);
                if(!d_fp)
                    throw std::runtime_error("Can't open file: " + stringerror());
	}
	~SmartFP()
	{
		fclose(d_fp);
	}
	FILE* d_fp;
};

void func2()
{
	SmartFP fp("nosuchfile", "r");

	char line[512];
	while(fgets(line, sizeof(line), fp.d_fp)) {
		// do things with line
	}	
	// note, no fclose
}

void func()
{
    func2();
}

int main()
try {
    func();
} 
catch(std::exception& e) {
    std::cerr<< "Fatal error: " << e.what() << std::endl;
}
