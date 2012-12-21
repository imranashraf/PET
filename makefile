CC=g++
CFLAGS=-Wall -O3 -fopenmp
LDFLAGS=-fopenmp
LIBS=-lm

CPPSRCS=globals.cpp main.cpp rng.cpp edge.cpp application.cpp function.cpp \
	cluster.cpp partition.cpp count.cpp utility.cpp exception.cpp \
	algorithm.cpp bruteforce.cpp heuristic.cpp SimulatedAnnealer.cpp \
	TabuSearcher.cpp EvolutionarySearcher.cpp

OBJECTS=$(CPPSRCS:.cpp=.o)

EXEC=partool

all: depend $(EXEC)

depend: .depend

.depend: $(CPPSRCS)
	@-rm -f .depend
	$(CC) $(CFLAGS) -MM $^ > .depend;

include .depend

$(EXEC): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

%.o: %.cpp
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
valgrind: all
	valgrind --tool=memcheck --log-fd=1 -v --show-reachable=yes --leak-check=yes --track-origins=yes ./$(EXEC) 1 9 3

gprof: CFLAGS= -Wall -g -O3 -fopenmp -pg 
gprof: LDFLAGS=-fopenmp -pg
gprof: clean all
gprof:    	 # Mode  n  k 
	@-./$(EXEC) 1 20  4
	@-gprof -b ./$(EXEC) > profile.txt
	@-cat profile.txt | ./gprof2dot.py --skew=0.01 | dot -Tpdf -o profile.pdf 
# 	@-cat profile.txt | ./gprof2dot.py -e0 -n0 --skew=0.01 | dot -Tpdf -o profile.pdf   #all functions

open:
	kate makefile globals.h globals.cpp main.cpp algorithm.h algorithm.cpp bruteforce.h bruteforce.cpp \
	heuristic.h heuristic.cpp \
	SimulatedAnnealer.h SimulatedAnnealer.cpp \
	TabuSearcher.h TabuSearcher.cpp \
	EvolutionarySearcher.h EvolutionarySearcher.cpp \
	application.h application.cpp edge.h edge.cpp function.h function.cpp \
	partition.h partition.cpp cluster.h cluster.cpp utility.h utility.cpp \
	count.h count.cpp &> /dev/null &
	

clean:
	@-rm -f .depend $(OBJECTS) $(EXEC) *~ gmon.out graph_*.dot graph_*.pdf profile.txt profile.pdf costs_*.txt output_*.txt

#TODO use STL as much as possible
#TODO remove all globals for application and number of functions
#TODO Heuristic adds all remaining functions to same cluster, Check !!!
#TODO minimum value of double