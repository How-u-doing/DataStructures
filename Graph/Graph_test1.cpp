#include "Graph.h"
#include <string>
#include <iostream>

using namespace std;

int main()
{
	using namespace myGraph;

	//string filename{ "SymbolGraph.txt" };
	//string filename{ "SymbolGraph_w.txt" };
	//string filename{ "tinyG.txt" };
	//string filename{ "tinyG_w.txt" };
	try {		
		bool weighted = true;
		Graph<string> G("SymbolGraph_w.txt", weighted);
		G.output_graph(cout, weighted);

		G.read_file("tinyG.txt", !weighted);
		G.output_graph(cout, !weighted);
	}
	catch (const GraphReadException& e) {
		cout << e.what() << endl;
	}
	catch (const runtime_error& e) {
		cout << e.what() << endl;
	}
	catch (...) {
		cout << "Some unknown error occured" << endl;
	}
	
	return 0;
}
