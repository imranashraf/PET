#TODO fix floating point number printing
#TODO remove all globals for application and number of functions
CC=g++
CFLAGS=-Wall -O3 -fopenmp
LDFLAGS= -fopenmp
LIBS=-lm
CSRCS=

CPPSRCS=globals.cpp main.cpp rng.cpp edge.cpp application.cpp function.cpp \
	cluster.cpp partition.cpp algorithm.cpp bruteforce.cpp \
	heuristic.cpp count.cpp utility.cpp exception.cpp SimulatedAnnealer.cpp

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

mode1: $(EXEC)
	@-./$(EXEC)  1 9  3

mode2: $(EXEC)
	@-./$(EXEC)  2 appData.txt  3

mode3: $(EXEC)
	@-./$(EXEC)  3 maipData.txt  quadData.txt 3

mode4: $(EXEC)
	@-./$(EXEC)  4 5 10 2 4

run: mode1
# 	@-cat graph.dot 2> /dev/null | dot -Tpdf -o graph.pdf

debug: CFLAGS= -Wall -g -fopenmp 
debug: all

valgrind: CFLAGS= -Wall -g -fopenmp 
valgrind: clean all
	valgrind --tool=memcheck --log-fd=1 -v --show-reachable=yes --leak-check=yes --track-origins=yes ./$(EXEC) 9 3

gprof: CFLAGS= -Wall -g -O3 -fopenmp -pg 
gprof: LDFLAGS=-fopenmp -pg
gprof: clean all
gprof:    	 # Mode  n  k 
	@-./$(EXEC) 1 12  4
	@-gprof -b ./$(EXEC) > profile.txt
	@-cat profile.txt | ./gprof2dot.py --skew=0.01 | dot -Tpdf -o profile.pdf 
# 	@-cat profile.txt | ./gprof2dot.py -e0 -n0 --skew=0.01 | dot -Tpdf -o profile.pdf   #all functions

open:
	kate makefile globals.h globals.cpp main.cpp algorithm.h algorithm.cpp bruteforce.h bruteforce.cpp \
	heuristic.h heuristic.cpp \
	SimulatedAnnealer.h SimulatedAnnealer.cpp\
	application.h application.cpp edge.h edge.cpp function.h function.cpp \
	partition.h partition.cpp cluster.h cluster.cpp &> /dev/null &
	

clean:
	@-rm -f $(OBJECTS) $(EXEC) *~ gmon.out graph_*.dot graph_*.pdf profile.txt profile.pdf costs_*.txt output_*.txt
