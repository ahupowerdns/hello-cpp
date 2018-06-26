CXXFLAGS:=-std=gnu++17 -Wall -O2 -MMD -MP -ggdb -pthread -fopenmp

PROGRAMS = test exception smartfp string thread memory windex

all: $(PROGRAMS)

clean:
	rm -f *~ *.o *.d test $(PROGRAMS)

-include *.d

test: test.o 
	g++ -std=gnu++17 $^ -o $@ -pthread -fopenmp

smartfp: smartfp.o 
	g++ -std=gnu++17 $^ -o $@ -pthread -fopenmp

memory: memory.o 
	g++ -std=gnu++17 $^ -o $@ -pthread -fopenmp

windex: windex.o 
	g++ -std=gnu++17 $^ -o $@ -pthread -fopenmp

