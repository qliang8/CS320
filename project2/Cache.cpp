#include "Cache.h"

using namespace std;

Cache::Cache(string filename){
	ifstream myfile(filename, std::ifstream::in);
	string line;
	while(getline(myfile, line)){
		int pos = line.find(" ");
		string type = line.substr(0, pos);
		string addressStr = line.substr(pos + 1);
		unsigned int address = strtoul(addressStr.c_str(), nullptr, 16);
		if(type == "L"){
			traceDataBase.push_back(Input{address, true});
		}else{
			traceDataBase.push_back(Input{address, false});
		}
	}
	myfile.close();
	this->traceNumber = traceDataBase.size();
}

int Cache::directMapped(int size){
	int hits = 0;
	int cacheSize = size*1024 / LINE_SIZE;
	int offset = log2(LINE_SIZE);
	int index = log2(cacheSize);
	vector<Line> cacheTable;
	for(int i = 0; i < cacheSize; i++){
		cacheTable.push_back(Line{0,0,false});
	}

	vector<Input>::iterator it;
	for(it = traceDataBase.begin(); it != traceDataBase.end(); it++){
		int pos = (it->address >> offset) % cacheSize;
		unsigned int tag = it->address >> (offset + index);
		if(cacheTable[pos].valid && cacheTable[pos].tag == tag){
			hits++;
		}else{
			cacheTable[pos].valid = true;
			cacheTable[pos].tag = tag;
		}
	}
	return hits;
}


int Cache::setAssociative(int associativity){
	int hits = 0;
	int cacheSize = 16*1024 / (associativity*LINE_SIZE);
	int offset = log2(LINE_SIZE);
	int index = log2(cacheSize);
	vector<vector<Line>> cacheTable;
	for(int i = 0; i < cacheSize; i++){
		vector<Line> temp;
		for(int j = 0; j < associativity; j++){
			temp.push_back(Line{0,0,false});
		}
		cacheTable.push_back(temp);
	}
	vector<Input>::iterator it;
	for(it = traceDataBase.begin(); it != traceDataBase.end(); it++){
		int pos = (it->address >> offset) % cacheSize;
		unsigned int tag = it->address >> (offset + index);
		int maxTime = 0;
		int replace = -1;
		bool hitFlag = false;
		for(int i = 0; i < associativity; i++){
			if(cacheTable[pos][i].valid && cacheTable[pos][i].tag == tag){
				hits++;
				cacheTable[pos][i].time = 0;
				replace = i;			
				hitFlag = true;
				break;
			}else if(cacheTable[pos][i].valid == false){
				replace = i;
				break;
			}
			if(cacheTable[pos][i].time > maxTime){
				maxTime = cacheTable[pos][i].time;
				replace = i;
			}

		}
		if(!hitFlag){                                                                     
			cacheTable[pos][replace].valid = true;
			cacheTable[pos][replace].tag = tag;
			cacheTable[pos][replace].time = 0;	
		}
		for(int i = 0; i < associativity; i++){
			if(cacheTable[pos][i].valid && i != replace){
				cacheTable[pos][i].time++;
			}
		} 
	}
	return hits;
}

int Cache::fullyAssoLRU(){
	int hits = 0;
	int cacheSize = 16*1024 / LINE_SIZE;
	int offset = log2(LINE_SIZE);
	vector<Line> cacheTable;
	for(int i = 0; i < cacheSize; i++){
		cacheTable.push_back(Line{0,0,false});
	}

	vector<Input>::iterator it;
	for(it = traceDataBase.begin(); it != traceDataBase.end(); it++){
		unsigned int tag = it->address >> offset;
		int maxTime = 0;
		int replace = -1;
		bool hitFlag = false;
		for(int i = 0; i < cacheSize; i++){
			if(cacheTable[i].valid && cacheTable[i].tag == tag){
				hits++;
				cacheTable[i].time = 0;
				replace = i;			
				hitFlag = true;
				break;
			}else if(cacheTable[i].valid == false){
				replace = i;
				break;
			}
			if(cacheTable[i].time > maxTime){
				maxTime = cacheTable[i].time;
				replace = i;
			}

		}
		if(!hitFlag){                                                                     
			cacheTable[replace].valid = true;
			cacheTable[replace].tag = tag;
			cacheTable[replace].time = 0;	
		}
		for(int i = 0; i < cacheSize; i++){
			if(cacheTable[i].valid && i != replace){
				cacheTable[i].time++;
			}
		} 
	}
	return hits;
}

int Cache::fullyAssoHC(){
	int hits = 0;
	int cacheSize = 16*1024 / LINE_SIZE;
	int offset = log2(LINE_SIZE);

	vector<Line> cacheTable;
	for(int i = 0; i < cacheSize; i++){
		cacheTable.push_back(Line{0,0,false});
	}
	vector<int> hotColdTable;
	for(int i = 0; i < cacheSize - 1; i++){
		hotColdTable.push_back(0); // 0 left hot right cold; 1 left cold right hot 
	}

	vector<Input>::iterator it;
	for(it = traceDataBase.begin(); it != traceDataBase.end(); it++){
		unsigned int tag = it->address >> offset;
		bool hitFlag = false;
		for(int i = 0; i < cacheSize; i++){
			if(cacheTable[i].valid && cacheTable[i].tag == tag){
				hits++;			
				hitFlag = true;
				int index = (cacheSize-1)/2;
				int count = log2(cacheSize)-1;			
				for(int j = count; j > 0;){
					j--;
					if(index >= i){
						hotColdTable[index] = 0;
						index = index - pow(2,j);
					}else{
						hotColdTable[index] = 1;
						index = index + pow(2,j);
					}
				}
				if(index == i) hotColdTable[index] = 0;
				else hotColdTable[index] = 1;
				//cout << index << endl;									
			} 
		}
		if(!hitFlag){
			int index = (cacheSize-1)/2; 
			//cout << index << endl;
			int count = log2(cacheSize)-1;
			for(int i = count; i > 0;){
				i--;
				//cout << i << endl;
				if(hotColdTable[index] == 0){
					hotColdTable[index] = 1;
					index = index + pow(2,i);
					//cout << index <<" 0 "<< endl;
				}else{
					hotColdTable[index] = 0;
					index = index - pow(2,i);
					//cout << index << " 1 " << endl;						
				}
			}
			if(hotColdTable[index] == 0){
				hotColdTable[index] = 1;
				index++;
			}else{
				hotColdTable[index] = 0;
			}
			cacheTable[index].valid = true;
			cacheTable[index].tag = tag;				

		}
	}
	return hits;

}

int Cache::setAssoNoAllocation(int associativity){
	int hits = 0;
	int cacheSize = 16*1024 / (associativity*LINE_SIZE);
	int offset = log2(LINE_SIZE);
	int index = log2(cacheSize);
	vector<vector<Line>> cacheTable;
	for(int i = 0; i < cacheSize; i++){
		vector<Line> temp;
		for(int j = 0; j < associativity; j++){
			temp.push_back(Line{0,0,false});
		}
		cacheTable.push_back(temp);
	}
	vector<Input>::iterator it;
	for(it = traceDataBase.begin(); it != traceDataBase.end(); it++){
		int pos = (it->address >> offset) % cacheSize;
		unsigned int tag = it->address >> (offset + index);
		int maxTime = 0;
		int replace = -1;
		bool hitFlag = false;
		for(int i = 0; i < associativity; i++){
			if(cacheTable[pos][i].valid && cacheTable[pos][i].tag == tag){
				hits++;
				cacheTable[pos][i].time = 0;
				replace = i;			
				hitFlag = true;
				break;
			}else if(cacheTable[pos][i].valid == false){
				replace = i;
				break;
			}
			if(cacheTable[pos][i].time > maxTime){
				maxTime = cacheTable[pos][i].time;
				replace = i;
			}

		}
		if(!hitFlag && it->isLoad){                                                                     
			cacheTable[pos][replace].valid = true;
			cacheTable[pos][replace].tag = tag;
			cacheTable[pos][replace].time = 0;	
		}
		if(!it->isLoad && !hitFlag){
			for(int i = 0; i < associativity; i++){
				if(cacheTable[pos][i].valid){
					cacheTable[pos][i].time++;
				}
			} 
		}else{
			for(int i = 0; i < associativity; i++){
				if(cacheTable[pos][i].valid && i != replace){
					cacheTable[pos][i].time++;
				}
			} 

		}
	}
	return hits;
}


int Cache::setNextLine(int associativity){
	int hits = 0;
	int cacheSize = 16*1024 / (associativity*LINE_SIZE);
	int offset = log2(LINE_SIZE);
	int index = log2(cacheSize);
	vector<vector<Line>> cacheTable;
	for(int i = 0; i < cacheSize; i++){
		vector<Line> temp;
		for(int j = 0; j < associativity; j++){
			temp.push_back(Line{0,0,false});
		}
		cacheTable.push_back(temp);
	}
	vector<Input>::iterator it;
	for(it = traceDataBase.begin(); it != traceDataBase.end(); it++){
		int pos = (it->address >> offset) % cacheSize;
		unsigned int tag = it->address >> (offset + index);

		int maxTime = 0;
		int replace = -1;
		bool hitFlag = false;
		for(int i = 0; i < associativity; i++){
			if(cacheTable[pos][i].valid && cacheTable[pos][i].tag == tag){
				hits++;
				cacheTable[pos][i].time = 0;
				replace = i;			
				hitFlag = true;
				break;
			}else if(cacheTable[pos][i].valid == false){
				replace = i;
				break;
			}
			if(cacheTable[pos][i].time > maxTime){
				maxTime = cacheTable[pos][i].time;
				replace = i;
			}

		}
		if(!hitFlag){                                                                     
			cacheTable[pos][replace].valid = true;
			cacheTable[pos][replace].tag = tag;
			cacheTable[pos][replace].time = 0;	
		}
		for(int i = 0; i < associativity; i++){
			if(cacheTable[pos][i].valid && i != replace){
				cacheTable[pos][i].time++;
			}
		} 

		int posNext = ((it->address + LINE_SIZE) >> offset) % cacheSize;
		unsigned int tagNext = (it->address + LINE_SIZE) >> (offset + index);

		int maxTimeNext = 0;
		int replaceNext = -1;
		bool hitFlagNext = false;
		for(int i = 0; i < associativity; i++){
			if(cacheTable[posNext][i].valid && cacheTable[posNext][i].tag == tagNext){
				cacheTable[posNext][i].time = 0;
				replaceNext = i;			
				hitFlagNext = true;
				break;
			}else if(cacheTable[posNext][i].valid == false){
				replaceNext = i;
				break;
			}
			if(cacheTable[posNext][i].time > maxTimeNext){
				maxTimeNext = cacheTable[posNext][i].time;
				replaceNext = i;
			}

		}
		if(!hitFlagNext){                                                                     
			cacheTable[posNext][replaceNext].valid = true;
			cacheTable[posNext][replaceNext].tag = tagNext;
			cacheTable[posNext][replaceNext].time = 0;	
		}
		for(int i = 0; i < associativity; i++){
			if(cacheTable[posNext][i].valid && i != replaceNext){
				cacheTable[posNext][i].time++;
			}
		} 
	}
	return hits;
}

int Cache::setPrefetchMiss(int associativity){
	int hits = 0;
	int cacheSize = 16*1024 / (associativity*LINE_SIZE);
	int offset = log2(LINE_SIZE);
	int index = log2(cacheSize);
	vector<vector<Line>> cacheTable;
	for(int i = 0; i < cacheSize; i++){
		vector<Line> temp;
		for(int j = 0; j < associativity; j++){
			temp.push_back(Line{0,0,false});
		}
		cacheTable.push_back(temp);
	}
	vector<Input>::iterator it;
	for(it = traceDataBase.begin(); it != traceDataBase.end(); it++){
		int pos = (it->address >> offset) % cacheSize;
		unsigned int tag = it->address >> (offset + index);

		int maxTime = 0;
		int replace = -1;
		bool hitFlag = false;
		for(int i = 0; i < associativity; i++){
			if(cacheTable[pos][i].valid && cacheTable[pos][i].tag == tag){
				hits++;
				cacheTable[pos][i].time = 0;
				replace = i;			
				hitFlag = true;
				break;
			}else if(cacheTable[pos][i].valid == false){
				replace = i;
				break;
			}
			if(cacheTable[pos][i].time > maxTime){
				maxTime = cacheTable[pos][i].time;
				replace = i;
			}

		}
		if(!hitFlag){                                                                     
			cacheTable[pos][replace].valid = true;
			cacheTable[pos][replace].tag = tag;
			cacheTable[pos][replace].time = 0;	
		}
		for(int i = 0; i < associativity; i++){
			if(cacheTable[pos][i].valid && i != replace){
				cacheTable[pos][i].time++;
			}
		} 
		
		if(!hitFlag){
			int posNext = ((it->address + LINE_SIZE) >> offset) % cacheSize;
			unsigned int tagNext = (it->address + LINE_SIZE) >> (offset + index);

			int maxTimeNext = 0;
			int replaceNext = -1;
			bool hitFlagNext = false;
			for(int i = 0; i < associativity; i++){
				if(cacheTable[posNext][i].valid && cacheTable[posNext][i].tag == tagNext){
					cacheTable[posNext][i].time = 0;
					replaceNext = i;			
					hitFlagNext = true;
					break;
				}else if(cacheTable[posNext][i].valid == false){
					replaceNext = i;
					break;
				}
				if(cacheTable[posNext][i].time > maxTimeNext){
					maxTimeNext = cacheTable[posNext][i].time;
					replaceNext = i;
				}

			}
			if(!hitFlagNext){                                                                     
				cacheTable[posNext][replaceNext].valid = true;
				cacheTable[posNext][replaceNext].tag = tagNext;
				cacheTable[posNext][replaceNext].time = 0;	
			}
			for(int i = 0; i < associativity; i++){
				if(cacheTable[posNext][i].valid && i != replaceNext){
					cacheTable[posNext][i].time++;
				}
			} 
		}
	}
	return hits;
}
