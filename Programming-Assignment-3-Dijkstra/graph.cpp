#include <iostream>
#include <stack>
#include "graph.h"
using namespace std;


// Takes name of a valid input text file, parses the file, and constructs graph.
graph :: graph(string input_file){
	vertices = new hashTable(1000);

	ifstream inputFile(input_file);
	string line;
	
	while (getline(inputFile, line)){
		int start = 0;
		int end = 0;
		int i = 0;
		vector<string> edge;

		// Parse each row into three components (source, destination, cost)
		while(end != string::npos){
			end = line.find(" ", start);
			edge.push_back(line.substr(start, end - start));
			start = end + 1;
			i++;
		}

		// Check to see if source vertex exists already. If not, insert into hash table & adjacency list.
		if(!(vertices->contains(edge[0]))){
			Vertex* source = new Vertex(edge[0]);							
			vertices->insert(edge[0], source);
			adj_list.push_back(source);
		}

		// Check to see if destination vertex exists already. If not, insert into hash table & adjacency list.
		if(!(vertices->contains(edge[1]))){
			Vertex* dest = new Vertex(edge[1]);			
			vertices->insert(edge[1], dest);
			adj_list.push_back(dest);

		}
		
		// Get pointers to source & destination vertices
		Vertex* source = static_cast<Vertex *> (vertices->getPointer(edge[0]));
		Vertex* dest = static_cast<Vertex *> (vertices->getPointer(edge[1]));
		
		// Create an object instance to store the destination vertex pointer & cost of the edge
		edgeCost* eC = new edgeCost(dest, string_to_int(edge[2]));
		
		// Push object instance pointer onto source's adjacency list
		(source->adjacent).push_back(eC);
	}
			
}

// Vertex constructor
graph :: Vertex :: Vertex (std::string n, unsigned int d, Vertex* p, bool k): name(n), dv(d), Pv(p), known(k) {}

// edgeCost constructor
graph :: edgeCost :: edgeCost(Vertex *v, unsigned int c) : pv(v), cost(c) {}

void graph :: setDv(Vertex * p, unsigned int d){
	(p->dv) = d;
}

void graph :: setPv(Vertex * p, Vertex * pPV){
	(p->Pv) = pPV;
}

void graph:: setKnown(Vertex * p, bool k){
	(p->known) = k;
}

// Conversion of a string to an integer (for edge cost purposes)
int graph :: string_to_int(const string &input_string){
	stringstream ss;
	ss << input_string;	
	int input_int;
	ss >> input_int;
	return input_int;
}

bool graph :: checkValid(string s){
	return vertices->contains(s);
}

void graph :: dijkstra(string s){

	int num_vertices = adj_list.size();
	heap Uk(num_vertices);
	
	// First initialize vertices properties
	list<Vertex *>::iterator itr;
	for( itr = adj_list.begin(); itr != adj_list.end(); ++itr ){
		setDv((*itr), 1000000000);
		setKnown( (*itr), false);
		Uk.insert((*itr)->name, (*itr)->dv, (*itr) );
	}
	
	
	// Initialize source vertex
	Vertex * ps = static_cast<Vertex *> (vertices->getPointer(s));
	
	Uk.setKey(s, 0);
	setDv(ps, 0);
	setPv(ps, NULL);

	string *pId = new string;
	int *pKey = new int;
	void *ppData = new Vertex *;
	
	// While there are still unknown vertices
	while(!Uk.deleteMin(pId, pKey, ppData)){
		
		Vertex * p = *(static_cast<Vertex **> (ppData));
		setKnown(p, true);
		
		unsigned int dmin = *pKey;
		list<edgeCost *>::iterator itr;
		
		// Loop through adjacent vertices & update minimum cost if necessary
		for( itr = (p->adjacent).begin(); itr != (p->adjacent).end(); ++itr ){
			if( (dmin + (*itr)->cost) < (*itr)->pv->dv ) {
				  setDv((*itr)->pv, (dmin + (*itr)->cost));
				  Uk.setKey((*itr)->pv->name, (*itr)->pv->dv);
				  setPv((*itr)->pv, p);
			}
		}
	}
}

void graph::printPaths(std::string output_file){
	ofstream outputFile(output_file);
	
	list<Vertex *>::iterator itr;
	for( itr = adj_list.begin(); itr != adj_list.end(); ++itr ){
		outputFile << (*itr)->name << ": " << flush;
		if( (*itr)->dv >= 1000000000 ){
			outputFile << "NO PATH" << endl;
		}
		
		else{
			outputFile << (*itr)->dv << flush;
			stack<string> path;
			Vertex * p = (*itr)->Pv;
			while(p != NULL){
				path.push(p->name);
				p = p->Pv;
			}
			outputFile << " [" << flush;
			while (!path.empty()){
			outputFile << path.top() << ", " << flush;
			path.pop();
			}
			outputFile << (*itr)->name << "]" << endl;
		}
		
	}
}