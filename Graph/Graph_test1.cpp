#include "Graph.h"
#include <string>
#include <iostream>

using namespace std;
using namespace myGraph;

template<typename T = std::string>
void print_vertex(Graph<T>& G, typename Graph<T>::iterator i) {
	cout << G.vertex(i) << endl;
}

int main()
{

	//string filename{ "SymbolGraph.txt" };
	//string filename{ "SymbolGraph_w.txt" };
	//string filename{ "tinyG.txt" };
	//string filename{ "tinyG_w.txt" };

	bool weighted = true;

	try {
		Graph<string> G("SymbolGraph_w.txt", weighted);
		cout << "Orginal: \n";
		G.output_graph(cout, weighted);

		cout << "\nG.has_path(G.find_vertex(\"LAS\"), G.find_vertex(\"MCO\"))==" <<
			G.has_path(G.find_vertex("LAS"), G.find_vertex("MCO"));

		vector<string> path{};
		G.path(G.find_vertex("LAS"), G.find_vertex("MCO"), path);
		cout << "\nPaths from LAS to MCO can be:\n";
		for (const auto& x : path)
			if (x == "LAS") cout << x;
			else cout << "-->" << x;
		cout << "\n";

		G.remove_vertex(G.find_vertex("PHX"));
		G.remove_vertex(G.find_vertex("HOU"));

		cout << "\nAfter remove \"PHX\" & \"HOU\": \n";
		G.output_graph(cout, weighted);

		cout << "\nDFS SymbolGraph_w.txt:\n";
		vector<bool>marked(G.vertex_size(), false);
		G.DFS(G.find_vertex("LAS"), print_vertex, marked);

		cout << "\ntinyG.txt: \n";
		G.read_file("tinyG.txt", !weighted);
		G.output_graph(cout, !weighted);

		cout << "\nConnected components: \n";
		G.connected_component();
		
	}
	catch (const GraphReadException& e) {
		cout << e.what() << endl;
	}	
	catch (const GraphEdgeException& e) {
		cout << e.what() << endl;
	}
	catch (const GraphInternalException& e) {
		cout << e.what() << endl;
	}
	catch (const GraphIterOutOfRange& e) {
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