#include "graph.h"
#include <ctime>

using namespace std;


int main() {

	cout << "Enter name of graph file: " << flush;
	string input_file;
	cin >> input_file;
	
	graph G = graph(input_file);
	string source;

	while(true){
		cout << "Enter a valid vertex id for the starting vertex: " << flush;
		cin >> source;
		if( G.checkValid(source) ){ break;}
	}
	
	clock_t begin = clock();
	G.dijkstra(source);
	clock_t end = clock();
	
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	
	cout << "Total time (in seconds) to apply Dijkstra's algorithm: " << elapsed_secs << endl;
	cout << "Enter name of output file: " << flush;
	string output_file;
	cin >> output_file;
	
	G.printPaths(output_file);


return 0;
}