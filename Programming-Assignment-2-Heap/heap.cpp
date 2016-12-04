#include <iostream>
#include <limits.h> 
#include "heap.h"
using namespace std;

heap :: heap(int capacity){
	currentSize = 0;
	this->capacity = capacity;
	data.resize(capacity+1);
  	mapping = new hashTable(capacity);
}

int heap :: insert(const std::string &id, int key, void *pv){
	
	if( currentSize == capacity ){ return 1; }
	
	else if( mapping->contains(id) ){ return 2; }
		
	++currentSize;
	
	data[currentSize].id = id;
	data[currentSize].key = key;
	data[currentSize].pData = pv;
	
	mapping->insert(id);
	percolateUp(currentSize);
	
	return 0;
}

int heap :: setKey(const std::string &id, int key){

	node *pn =  static_cast<node *> (mapping->getPointer(id)); // Get pointer to id's node in binary heap
	if( pn == NULL ){ return 1; }
	
	int old_key = pn->key; // Store old key
	pn->key = key; // Set new key
	int posCur = getPos(pn); // Get position in vector in order to percolate up/down
	
	if( old_key < key ){
		percolateDown(posCur);
	}
	else{
		percolateUp(posCur);
	}
	
	return 0;
}

int heap :: deleteMin(std::string *pId, int *pKey, void *ppData){
		if( currentSize == 0 ){ return 1; }
		
		if( pId != NULL ){ (*pId) = data[1].id; }
		if( pKey != NULL ){ (*pKey) = data[1].key; }
		if( ppData != NULL ){ *(static_cast<void **> (ppData)) = data[1].pData; }
		mapping->remove(data[1].id);
		data[1] = data[currentSize--];
		percolateDown(1);
		return 0;
} 

int heap :: remove(const std::string &id, int *pKey, void *ppData){
	node *pn =  static_cast<node *> (mapping->getPointer(id));
	if( pn == NULL ){ return 1; }
	if( pKey != NULL ){ (*pKey) = pn->key; }
	if( ppData != NULL ){ *(static_cast<void **> (ppData)) = pn->pData; }

	setKey(id, INT_MIN); // Set id's key to smallest possible integer then call deleteMin()
	deleteMin();
	mapping->remove(id); // Remove id from hashTable
	return 0;
}
  	
int heap :: getPos(node *pn){
	int pos = pn - &data[0];
	return pos;
}

void heap :: percolateUp(int posCur){
	node tmp = data[ posCur ];
	for( ; (posCur > 1) && (tmp.key < data[ posCur/2 ].key ) ; posCur /= 2 ){
		data[ posCur ] = data[ posCur / 2 ]; 
		mapping->setPointer(data[ posCur ].id, &data[ posCur ]);
	}
	data[ posCur ] = tmp;
	mapping->setPointer(data[ posCur ].id, &data[ posCur ]);
}


void heap :: percolateDown(int posCur){
	int child;
	node tmp  = data[ posCur ];
	for( ; (2*posCur ) <= currentSize; posCur = child ){
		child = 2*posCur;
		
		// Check to see if there are two children & go to the smaller of the two
		if( child != currentSize && data[ child + 1 ].key < data[ child ].key ){ child++; }
	
		if( data[ child ].key < (tmp.key) ){ 
			data[ posCur ] = data[ child ];
			mapping->setPointer(data[ posCur ].id, &data[ posCur ]);
		}
	
		else{ break; }
	}
	data[ posCur ] = tmp;
	mapping->setPointer(data[ posCur ].id, &data[ posCur ]);
}
	
