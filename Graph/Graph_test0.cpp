#include "Graph.h"
#include <string>
#include <iostream>

using namespace std;

int main(int argc, const char* argv[])
{
	using namespace myGraph;

	if (argc < 2) {
		cerr << "lack of input file" << endl;
		return 1;
	}
	string filename{ argv[1] };
	
	try {
		bool weighted = true;
		Graph<string> G(filename, weighted);
		G.output_graph(cout, weighted);
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
