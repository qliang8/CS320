#ifndef PREDICTORS_H
#define PREDICTORS_H
#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <fstream>

class Predictors{
	private:
		std::vector<std::pair<unsigned long, bool>> traceDataBase;

	public:
		int branchNumber;
		Predictors(std::string filename);
		int alwaysTaken();
		int alwaysNotTaken();
		int binomialSingleBit(int tableSize);
		int binomialTwoBits(int tableSize);
		int twoBitsHelper(std::vector<int>, int);
		int gShare(int);
		int tournament();
		
};



#endif
