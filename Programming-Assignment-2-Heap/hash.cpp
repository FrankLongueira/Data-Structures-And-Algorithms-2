#include <iostream>
#include "hash.h"
using namespace std;


hashTable :: hashTable(int size): capacity(getPrime(size)), filled(0), data(capacity) { }


int hashTable :: insert(const std::string &key, void *p){
	
	int position = findPos(key);
		
	// Does not exist, insertion then rehash if needed
	if( position == -1 ){
		position = hash(key);
		
		while( data[position].isOccupied ){ 
			if(data[position].key == key){
				break;
			}
		position++;	
		position %= capacity;
		}
		
		data[position].key = key;
		data[position].isOccupied = true;
		data[position].isDeleted = false;
		data[position].pv = p;
		
		if( filled++ > (capacity/2) ) {
			int success = rehash();
			if(!success){
				return 2;
			}
		}
		return 0;
	}
	
	// Exists already
	else{
		data[position].pv = p;
		return 1;
	}		
}


bool hashTable :: contains(const std::string &key){
	int position = findPos(key);
	
	if(position == -1){ return false; }
	
	else{ return true; }
}

void * hashTable :: getPointer(const std::string &key, bool *b){
	int position = findPos(key);
	if(position == -1){
		if( b != NULL ){
			*b = false;
		}
		return NULL;
	}
	
	else{
		if( b != NULL ){
			*b = true;
		}
		return data[position].pv;
	}
}

int hashTable :: setPointer(const std::string &key, void *p){
	int position = findPos(key);
	if(position == -1){
		return 1;
	}
	
	else{
		data[position].pv = p;
		return 0;
	}
}

bool hashTable :: remove(const std::string &key){
	int position = findPos(key);
	if(position == -1){
		return false;
	}
	
	else{
		data[position].isDeleted = true;
		return true;
	}
}

hashTable :: hashItem :: hashItem (const std::string &k, const bool &o, const bool &d, void *p)
	: key(k), isOccupied(o), isDeleted(d), pv(p) {}


int hashTable :: hash(const std::string &key){
	unsigned long hash = 5381;
	for(int i = 0; i < key.size(); i++){
    	hash = ((((hash << 5) + hash) + key[i])); /* (hash * 33 + c) */
    }
	return hash % capacity;
}


int hashTable :: findPos(const std::string &key){
	int position = hash(key);
	while( data[position].isOccupied ){
		if(data[position].key == key){
			if(!(data[position].isDeleted)){
				return position;
			}
			return -1;
		}
		position++;
		position %= capacity;

	}
	return -1;
}


bool hashTable :: rehash(){
  	
  	try{
  		vector<hashItem> old_data = data;
  		capacity = getPrime(capacity);
  		data.clear();
  		data.resize(capacity);
  	
  		for(int i = 0; i < old_data.size(); i++){
			if( (old_data[i].isOccupied) && (old_data[i].isDeleted == false) ){
				insert(old_data[i].key);
			}
		}
		return true;
	}
	
	catch(exception &e){
		cerr << "Resize failed: " << e.what() << endl;
		return false;
	}
	
}


unsigned int hashTable :: getPrime(int size)	{
	int primes[] = {1223, 5003, 10007, 25013, 50101, 100003, 200017, 400051, 800161, 1600421, 3201109};
	int i = 0;
	while(primes[i] < 2*size){ i++; }
	
	return primes[i];
}
	