#ifndef _GRAPH_H
#define _GRAPH_H

#include <vector>
#include <list>
#include <iostream>
#include <sstream>
#include <fstream>
#include <limits.h> 
#include <string>
#include "hash.h"
#include "heap.h"

// A graph class that's implementation is focused on using Dijkstra's algorithm to solve the single source,
// shortest path problem with positive edge weights
class graph{

	public:
		graph(std::string input_file);
		
		//
		// checkValid - Checks to if the given vertex is part of the graph
		//
		bool checkValid(std::string s);
		
		//
		// dijkstra - Performs Dijkstra's algorithm using the given string name as the source vertex
		//			  Updates each vertex's attributes appropriately
		//
		void dijkstra(std::string s);
	
		//
		// printPaths - Writes to output file the shortest paths (including cost) from source 
		//				vertex to all vertices of the graph.
		//
		void printPaths(std::string output_file);
	
	public:
		
		class Vertex; // Forward declaration
		class edgeCost { // An inner class within graph to be used for the adjacency list
  			public:
  				edgeCost(Vertex *v, unsigned int c);
  				Vertex *pv; // Pointer to adjacent vertex
  				unsigned int cost; // Cost to get to adjacent vertex
  		};
  		
		class Vertex { // An inner class within graph
  			public:
  				Vertex(std::string n, unsigned int d = 1000000000, Vertex* p = NULL, bool k = false);
    			std::string name; // The name of this vertex
    			unsigned int dv; //If v is known, dv is the cost of the best possible path. Else, the cost of 
			  					// the best path from s to v going through only known vertices.
    			Vertex *Pv; // Point to previous vertex on the best path so far from s to v
    			bool known; // True if v is known
    			std::list<edgeCost *> adjacent; // List of pointers to vertices (w/ cost) adjacent to this vertex
  		};
  		
  		//
  		// setDv - Uses 'd' to set the dv value of the vertex pointed to by 'p'
  		//
		void setDv(Vertex * p, unsigned int d);
		
		//
  		// setPv - Uses 'pPV' to set the Pv value of the vertex pointed to by 'p'
  		//
		void setPv(Vertex * p, Vertex * pPV);
		
		//
  		// setKnown - Uses 'k' to set the 'known' value of the vertex pointed to by 'p'
  		//
		void setKnown(Vertex * p, bool k);
				
  		hashTable *vertices; // Hash table that maps vertex names to memory locations
  		
  		std::list<Vertex *> adj_list; // Adjacency list of pointers to vertices for the graph
  		
  		int string_to_int(const std::string &input_string); // Converts a string to an integer
  		
};


#endif //_GRAPH_H