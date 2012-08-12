CC=g++
CFLAGS=-Wall -O3 -fopenmp
LDFLAGS= -fopenmp
LIBS=-lm
CSRCS=
PROFILE=profile.txt

CPPSRCS=main.cpp rng.cpp edge.cpp function.cpp application.cpp \
	cluster.cpp partition.cpp algorithm.cpp bruteforce.cpp \
	count.cpp utility.cpp

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
	@-./$(EXEC)  4  2

gprof: CFLAGS= -Wall -g -fopenmp -pg 
gprof: LDFLAGS=-fopenmp -pg
gprof: clean all
gprof:    	 #   n  k 
	@-./$(EXEC)  12  4
	@-gprof -b ./$(EXEC) > $(PROFILE)
	
open:
	kate makefile globals.h main.cpp algorithm.h algorithm.cpp bruteforce.h bruteforce.cpp \
	application.h application.cpp edge.h edge.cpp function.h function.cpp \
	partition.h partition.cpp cluster.h cluster.cpp &
	

clean:
	@-rm -f $(OBJECTS) $(EXEC) *~ gmon.out $(PROFILE)
