CXXFLAGS:=-std=gnu++17 -Wall -O3 -MMD -MP -ggdb -pthread -fopenmp -Iext/flat_hash_map

PROGRAMS = test exception smartfp string thread memory windex windex-par classes templates lambdas multi move

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
	g++ -std=gnu++17 $^ -o $@ -pthread 

windex-par: windex-par.o 
	g++ -std=gnu++17 $^ -o $@ -pthread  -fopenmp


classes: classes.o 
	g++ -std=gnu++17 $^ -o $@ -pthread 

templates: templates.o 
	g++ -std=gnu++17 $^ -o $@ -pthread 

lambdas: lambdas.o 
	g++ -std=gnu++17 $^ -o $@ -pthread 


multi: multi.o 
	g++ -std=gnu++17 $^ -o $@ -pthread 

move: move.o 
	g++ -std=gnu++17 $^ -o $@ -pthread 
