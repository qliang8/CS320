#include <string>
#include <iostream>
#include <fstream>
#include <list>
#include <cstdlib>
#include "Predictors.h"

using namespace std;



int main(int argc, char *argv[]){
	if(argc != 3){
		cout << "Please invoke as ./predictors input_tarce.txt ouput.txt" << endl;
		exit(1);
	}
	

	Predictors predictors(argv[1]);

	ofstream output;
	output.open(argv[2], ios::out);//ios::app
	output << predictors.alwaysTaken() << "," << predictors.branchNumber << ";" << "\n";
	
	output << predictors.alwaysNotTaken() << "," << predictors.branchNumber << ";" << "\n";

	output << predictors.binomialSingleBit(16) << "," << predictors.branchNumber << "; ";
	output << predictors.binomialSingleBit(32) << "," << predictors.branchNumber << "; " ;
	output << predictors.binomialSingleBit(128) << "," << predictors.branchNumber << "; ";
	output << predictors.binomialSingleBit(256) << "," << predictors.branchNumber << "; ";
	output << predictors.binomialSingleBit(512) << "," << predictors.branchNumber << "; ";
	output << predictors.binomialSingleBit(1024) << "," << predictors.branchNumber << "; ";
	output << predictors.binomialSingleBit(2048) << "," << predictors.branchNumber << ";" << "\n";

	output << predictors.binomialTwoBits(16) << "," << predictors.branchNumber << "; ";
	output << predictors.binomialTwoBits(32) << "," << predictors.branchNumber << "; ";
	output << predictors.binomialTwoBits(128) << "," << predictors.branchNumber << "; ";
	output << predictors.binomialTwoBits(256) << "," << predictors.branchNumber << "; ";
	output << predictors.binomialTwoBits(512) << "," << predictors.branchNumber << "; ";
	output << predictors.binomialTwoBits(1024) << "," << predictors.branchNumber << "; ";
	output << predictors.binomialTwoBits(2048) << "," << predictors.branchNumber << ";" << "\n";

	output << predictors.gShare(3) << "," << predictors.branchNumber << "; ";
	output << predictors.gShare(4) << "," << predictors.branchNumber << "; ";
	output << predictors.gShare(5) << "," << predictors.branchNumber << "; ";
	output << predictors.gShare(6) << "," << predictors.branchNumber << "; ";
	output << predictors.gShare(7) << "," << predictors.branchNumber << "; ";
	output << predictors.gShare(8) << "," << predictors.branchNumber << "; ";
	output << predictors.gShare(9) << "," << predictors.branchNumber << "; ";
	output << predictors.gShare(10) << "," << predictors.branchNumber << "; ";
	output << predictors.gShare(11) << "," << predictors.branchNumber << ";" << "\n";

	output << predictors.tournament() << "," << predictors.branchNumber << ";" << "\n";


	output.close();

	return 0;
}
