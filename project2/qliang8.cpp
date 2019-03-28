#include <string>
#include <iostream>
#include <fstream>
#include <list>
#include <cstdlib>
#include "Cache.h"

using namespace std;

int main(int argc, char *argv[]){
	if(argc != 3){
		cout << "Please invoke as ./qliang8 input_tarce.txt ouput.txt" << endl;
		exit(1);
	}

	Cache cache(argv[1]); // create an object of cache

	ofstream output;
	output.open(argv[2], ios::out);//ios::app

	output << cache.directMapped(1) << "," << cache.traceNumber << "; ";
	output << cache.directMapped(4) << "," << cache.traceNumber << "; ";
	output << cache.directMapped(16) << "," << cache.traceNumber << "; ";
	output << cache.directMapped(32) << "," << cache.traceNumber << ";\n";

	output << cache.setAssociative(2) << "," << cache.traceNumber << "; ";
	output << cache.setAssociative(4) << "," << cache.traceNumber << "; ";
	output << cache.setAssociative(8) << "," << cache.traceNumber << "; ";
	output << cache.setAssociative(16) << "," << cache.traceNumber << ";\n";

	output << cache.fullyAssoLRU() << "," << cache.traceNumber << ";\n";
	output << cache.fullyAssoHC() << "," << cache.traceNumber << ";\n";

	output << cache.setAssoNoAllocation(2) << "," << cache.traceNumber << "; ";
	output << cache.setAssoNoAllocation(4) << "," << cache.traceNumber << "; ";
	output << cache.setAssoNoAllocation(8) << "," << cache.traceNumber << "; ";
	output << cache.setAssoNoAllocation(16) << "," << cache.traceNumber << ";\n";

	output << cache.setNextLine(2) << "," << cache.traceNumber << "; ";
	output << cache.setNextLine(4) << "," << cache.traceNumber << "; ";
	output << cache.setNextLine(8) << "," << cache.traceNumber << "; ";
	output << cache.setNextLine(16) << "," << cache.traceNumber << ";\n";

	output << cache.setPrefetchMiss(2) << "," << cache.traceNumber << "; ";
	output << cache.setPrefetchMiss(4) << "," << cache.traceNumber << "; ";
	output << cache.setPrefetchMiss(8) << "," << cache.traceNumber << "; ";
	output << cache.setPrefetchMiss(16) << "," << cache.traceNumber << ";\n";

	output.close();

	return 0;
}
