CC=g++
CFLAGS=-Wall -O3 -fopenmp
LDFLAGS= -fopenmp
LIBS=-lm
CSRCS=

CPPSRCS=main.cpp rng.cpp edge.cpp application.cpp function.cpp \
	cluster.cpp partition.cpp algorithm.cpp bruteforce.cpp \
	heuristic.cpp count.cpp utility.cpp

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
	@-cat graph.dot | dot -Tpdf -o graph.pdf

debug: CFLAGS= -Wall -g -fopenmp 
debug: clean all   #   n  k 
	@-./$(EXEC)  10  3


gprof: CFLAGS= -Wall -g -O3 -fopenmp -pg 
gprof: LDFLAGS=-fopenmp -pg
gprof: clean all
gprof:    	 #   n  k 
	@-./$(EXEC)  12  4
	@-gprof -b ./$(EXEC) > profile.txt
	@-cat profile.txt | ./gprof2dot.py --skew=0.01 | dot -Tpdf -o profile.pdf 
# 	@-cat profile.txt | ./gprof2dot.py -e0 -n0 --skew=0.01 | dot -Tpdf -o profile.pdf   #all functions

open:
	kate makefile globals.h main.cpp algorithm.h algorithm.cpp bruteforce.h bruteforce.cpp \
	heuristic.h heuristic.cpp \
	application.h application.cpp edge.h edge.cpp function.h function.cpp \
	partition.h partition.cpp cluster.h cluster.cpp &
	

clean:
	@-rm -f $(OBJECTS) $(EXEC) *~ gmon.out graph.dot graph.pdf profile.txt profile.pdf
