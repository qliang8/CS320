#ifndef CACHE_H
#define CACHE_H
#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <fstream>
#include <iterator>
#include <cmath>
#include <climits>


#define LINE_SIZE 32

typedef struct{
	unsigned int address;
	bool isLoad;
} Input;

typedef struct{
	int time;
	unsigned int tag;
	bool valid;
} Line;


class Cache{

	private:
		std::vector<Input> traceDataBase;
	public:
		int traceNumber;
		Cache(std::string filename);
		int directMapped(int size);
		int setAssociative(int associativity);
		int fullyAssoLRU();
		int fullyAssoHC();
		int setAssoNoAllocation(int associativity);
		int setNextLine(int associativity);
		int setPrefetchMiss(int associativity);


};



#endif
