#include <stdio.h>
#include <iostream>

using namespace std;

struct SmartFP
{
	SmartFP(const char* fname, const char* mode)
	{
		cout<<"Open"<<endl;
		d_fp = fopen(fname, mode);
	}
	~SmartFP()
	{
		cout<<"Close: " << (void*)d_fp <<endl;	
		if(d_fp) {
			fclose(d_fp);
		}
	}
	FILE* d_fp;
};

void func()
{
	SmartFP fp("/etc/passwd", "r");
	if(!fp.d_fp)
		; // do error things

	char line[512];
	while(fgets(line, sizeof(line), fp.d_fp)) {
		// do things with line
	}	
	// note, no fclose
}

int main()
{
	func();
	SmartFP one("/etc/passwd", "r");
	SmartFP two = one;
}