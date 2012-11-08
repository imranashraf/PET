#TODO fix floating point number printing
CC=g++
CFLAGS=-Wall -O3 -fopenmp
LDFLAGS= -fopenmp
LIBS=-lm
CSRCS=

CPPSRCS=globals.cpp main.cpp rng.cpp edge.cpp application.cpp function.cpp \
	cluster.cpp partition.cpp algorithm.cpp bruteforce.cpp \
	heuristic.cpp count.cpp utility.cpp exception.cpp

OBJECTS=$(CSRCS:.c=.o)
OBJECTS+=$(CPPSRCS:.cpp=.o)

EXEC=partool

all:$(EXEC)
$(EXEC): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

%.o: %.cpp
	$(CC) -c $(CFLAGS) $< -o $@ 

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@ 

run: $(EXEC)     #   n  k 
	@-./$(EXEC)  10  3
# 	@-cat graph.dot 2> /dev/null | dot -Tpdf -o graph.pdf

debug: CFLAGS= -Wall -g -fopenmp 
debug: all   #   n  k 
# 	@- gdb $(EXEC) 

valgrind: CFLAGS= -Wall -g -fopenmp 
valgrind: clean all
	valgrind --tool=memcheck --log-fd=1 -v --show-reachable=yes --leak-check=yes --track-origins=yes ./$(EXEC) 9 3

gprof: CFLAGS= -Wall -g -O3 -fopenmp -pg 
gprof: LDFLAGS=-fopenmp -pg
gprof: clean all
gprof:    	 #   n  k 
	@-./$(EXEC)  12  4
	@-gprof -b ./$(EXEC) > profile.txt
	@-cat profile.txt | ./gprof2dot.py --skew=0.01 | dot -Tpdf -o profile.pdf 
# 	@-cat profile.txt | ./gprof2dot.py -e0 -n0 --skew=0.01 | dot -Tpdf -o profile.pdf   #all functions

open:
	kate makefile globals.h globals.cpp main.cpp algorithm.h algorithm.cpp bruteforce.h bruteforce.cpp \
	heuristic.h heuristic.cpp \
	application.h application.cpp edge.h edge.cpp function.h function.cpp \
	partition.h partition.cpp cluster.h cluster.cpp &> /dev/null &
	

clean:
	@-rm -f $(OBJECTS) $(EXEC) *~ gmon.out graph_*.dot graph_*.pdf profile.txt profile.pdf costs_*.txt output_*.txt
