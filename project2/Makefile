P = qliang8
CFLAGS= -g -Wall -Wextra -DDEBUG -pedantic -std=c++14

all: $(P)

run: all
	./$(P) trace1.txt output1.txt
	./$(P) trace2.txt output2.txt
	./$(P) trace3.txt output3.txt
clean:
	rm -rf *.o $(P)
	rm $(P)

checkmem: all
	valgrind ./$(P)

$(P): $(P).o Cache.o
	g++ $(CFLAGS) $(P).o Cache.o -o $(P)

$(P).o: $(P).cpp Cache.h
	g++ $(CFLAGS) -c $(P).cpp -o $(P).o

Cache.o: Cache.cpp Cache.h 
	g++ $(CFLAGS) -c Cache.cpp -o Cache.o
