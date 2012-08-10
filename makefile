CC=g++
CFLAGS=-g -Wall -O2 -fopenmp
LDFLAGS= -fopenmp
LIBS=-lm
CSRCS=count.c
CPPSRCS=main.cpp rng.cpp edge.cpp function.cpp application.cpp \
	cluster.cpp partition.cpp algorithm.cpp bruteforce.cpp

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

clean:
	@-rm -f $(OBJECTS) $(EXEC) *~
