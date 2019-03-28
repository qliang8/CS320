#include "Predictors.h"

using namespace std;


Predictors::Predictors(string filename){
	ifstream myfile(filename, std::ifstream::in);
	string line; 
	while(getline(myfile, line)){
		int pos = line.find(" ");
		pair<unsigned long, bool> temp; 
		string branch = line.substr(0, pos);
		temp.first = strtoul(branch.c_str(), nullptr, 16); 
		string prediction = line.substr(pos + 1);
		if(prediction == "T"){
			temp.second = true;
		}else{
			temp.second = false;
		}
		traceDataBase.push_back(temp);		
	}
	myfile.close();
	this->branchNumber = traceDataBase.size();
}

int Predictors::alwaysTaken(){
	int correct = 0;
	for(pair<unsigned long,bool> i : traceDataBase){
		if(i.second){
			correct++;
		}
	}	
	return correct;
}

int Predictors::alwaysNotTaken(){
	int correct = 0;
	for(pair<unsigned long,bool> i : traceDataBase){
		if(!i.second){
			correct++;
		}
	}	
	return correct;
}


int Predictors::binomialSingleBit(int tableSize){
	int correct = 0;
	vector<bool> singleBit;
	for(int i = 0; i < tableSize; i++){
		singleBit.push_back(true);
	}
	for(pair<unsigned long,bool> i : traceDataBase){
		int pos = i.first % tableSize;
		if(singleBit[pos] == i.second){
			correct++;
		}else{
			if(singleBit[pos] == true){
				singleBit[pos] = false;
			}else{
				singleBit[pos] = true;
			}
		}
	}
	return correct;
}


int Predictors::binomialTwoBits(int tableSize){
	int correct = 0;
	vector<int> twoBits;
	for(int i = 0; i < tableSize; i++){
		twoBits.push_back(3);
	}
	correct = twoBitsHelper(twoBits,tableSize);
	return correct;
}

int Predictors::twoBitsHelper(vector<int> twoBits, int tableSize){
	int correctHelper = 0;
	for(pair<unsigned long,bool> i : traceDataBase){
		int pos = i.first % tableSize;
		int predict = twoBits[pos];
		bool predictB = false;
		if(predict == 2 || predict == 3){
			predictB = true;
		}
		if(predictB == i.second){
			correctHelper++;
			if(predict == 2){
				twoBits[pos] = 3;
			}else if(predict == 1){
				twoBits[pos] = 0;
			}
		}else{
			if(predict == 3){
				twoBits[pos] = 2;
			}else if(predict == 2){
				twoBits[pos] = 1;
			}else if(predict == 1){
				twoBits[pos] = 2;
			}else{
				twoBits[pos] = 1;
			}
		}
	}
	return correctHelper;
}

int Predictors::gShare(int length){
	int correct = 0;
	unsigned int globalH = 0;
	vector<int> gShareTable;
	for(int i = 0; i < 2048; i++){
		gShareTable.push_back(3);
	}
	int num = (1UL << length) - 1;
	
	for(pair<unsigned long,bool> i : traceDataBase){
		int pos = (i.first % 2048) ^ (globalH & num) ; 
		
		int predict = gShareTable[pos];
		bool predictB = false;
		if(predict == 2 || predict == 3){
			predictB = true;
		}
		if(predictB == i.second){
			correct++;
			if(predict == 2){
				gShareTable[pos] = 3;
			}else if(predict == 1){
				gShareTable[pos] = 0;
			}
		}else{
			if(predict == 3){
				gShareTable[pos] = 2;
			}else if(predict == 2){
				gShareTable[pos] = 1;
			}else if(predict == 1){
				gShareTable[pos] = 2;
			}else{
				gShareTable[pos] = 1;
			}
		}
		if(i.second){
			globalH = globalH << 1;
			globalH++;
		}else{
			globalH = globalH << 1;
		}
	}
	return correct;
}

int Predictors::tournament(){
	int correct = 0;
	unsigned int globalH = 0;
	int num = (1UL << 11) - 1;	
	vector<int> selector;
	for(int i = 0; i < 2048; i++){
		selector.push_back(0);
	}
	vector<int> twoBits;
	for(int j = 0; j < 2048; j++){
		twoBits.push_back(3);
	}
	vector<int> gShareTable;
	for(int k = 0; k < 2048; k++){
		gShareTable.push_back(3);
	}

	for(pair<unsigned long,bool> i : traceDataBase){
			int pos = i.first % 2048;
			int predict = selector[pos];
			int predictFlagTwoBit = 0;
			int predictFlagGshare = 0;
			//binomialSingleBits
			int predictTwo = twoBits[pos];
			bool predictTwoBits = false;
			if(predictTwo == 2 || predictTwo == 3){
				predictTwoBits = true;
			}
			if(predictTwoBits == i.second){
				predictFlagTwoBit = 1;
				if(predictTwo == 2){
					twoBits[pos] = 3;
				}else if(predictTwo == 1){
					twoBits[pos] = 0;
				}
			}else{
				if(predictTwo == 3){
					twoBits[pos] = 2;
				}else if(predictTwo== 2){
					twoBits[pos] = 1;
				}else if(predictTwo == 1){
					twoBits[pos] = 2;
				}else{
					twoBits[pos] = 1;
				}
			}

			int posGshare = (i.first % 2048) ^ (globalH & num);
			int predictG = gShareTable[posGshare];
			bool predictGshare = false;
			if(predictG == 2 || predictG == 3){
				predictGshare = true;
			}
			if(predictGshare == i.second){
				predictFlagGshare = 1;		
				if(predictG == 2){
					gShareTable[posGshare] = 3;
				}else if(predictG == 1){
					gShareTable[posGshare] = 0;
				}
			}else{
				if(predictG == 3){
					gShareTable[posGshare] = 2;
				}else if(predictG == 2){
					gShareTable[posGshare] = 1;
				}else if(predictG == 1){
					gShareTable[posGshare] = 2;
				}else{
					gShareTable[posGshare] = 1;
				}
			}
		if((predict == 2 || predict == 3) && predictFlagTwoBit == 1){
			correct++;	
		}else if((predict == 0 || predict == 1) && predictFlagGshare == 1){
			correct++;			
		}
	
		if(predictFlagTwoBit == 1 && predictFlagGshare == 0){
			if(predict == 0){
				selector[pos] = 1;
			}else if(predict == 1){
				selector[pos] = 2;
			}else{
				selector[pos] = 3;
			}

		}else if(predictFlagTwoBit == 0 && predictFlagGshare == 1){
			if(predict == 3){
				selector[pos] = 2;
			}else if(predict == 2){
				selector[pos] = 1;
			}else{
				selector[pos] = 0;
			}
		}

		if(i.second){
			globalH = globalH << 1;
			globalH++;
		}else{
			globalH = globalH << 1;
		}
	}

	return correct;
}




