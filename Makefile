P = predictors
CFLAGS= -g -Wall -Wextra -DDEBUG -pedantic -std=c++14

all: $(P)

run: all
	./$(P)

clean:
	rm -rf *.o $(P)
	rm $(P)

checkmem: all
	valgrind ./$(P)

$(P): $(P).o Predictors.o
	g++ $(CFLAGS) $(P).o Predictors.o -o $(P)

$(P).o: $(P).cpp Predictors.h
	g++ $(CFLAGS) -c $(P).cpp -o $(P).o

Predictors.o: Predictors.cpp Predictors.h 
	g++ $(CFLAGS) -c Predictors.cpp -o Predictors.o


