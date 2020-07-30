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

		cout << "\nG.has_path(G.find_vertex(\"LAS\"), G.find_vertex(\"MCO\"), \"DFS\")==" <<
			G.has_path(G.find_vertex("LAS"), G.find_vertex("MCO"), "DFS");

		vector<string> path_DFS{};
		G.path(G.find_vertex("LAS"), G.find_vertex("MCO"), path_DFS);
		cout << "\nPaths from LAS to MCO via DFS can be:\n";
		for (const auto& x : path_DFS)
			if (x == "LAS") cout << x;
			else cout << "-->" << x;
		cout << "\n";

		cout << "\nG.has_path(G.find_vertex(\"LAS\"), G.find_vertex(\"MCO\"), \"BFS\")==" <<
			G.has_path(G.find_vertex("LAS"), G.find_vertex("MCO"), "BFS");

		vector<string> path_BFS{};
		G.path(G.find_vertex("LAS"), G.find_vertex("MCO"), path_BFS, "BFS");
		cout << "\nPaths from LAS to MCO via BFS can be:\n";
		for (const auto& x : path_BFS)
			if (x == "LAS") cout << x;
			else cout << "-->" << x;
		cout << "\n";

		G.remove_vertex(G.find_vertex("PHX"));
		G.remove_vertex(G.find_vertex("HOU"));

		cout << "\nAfter remove \"PHX\" & \"HOU\": \n";
		G.output_graph(cout, weighted);

		cout << "\nDFS SymbolGraph_w.txt starting at \"LAS\":\n";
		vector<bool>marked(G.vertex_size(), false);
		G.DFS(G.find_vertex("LAS"), marked, print_vertex);

		cout << "\nBFS SymbolGraph_w.txt starting at \"LAS\":\n";
		vector<bool>marked_2(G.vertex_size(), false);
		G.BFS(G.find_vertex("LAS"), marked_2, print_vertex);

		cout << "\ntinyG.txt: \n";
		G.read_file("tinyG.txt", !weighted);
		G.output_graph(cout, !weighted);

		cout << "\nConnected components: \n";
		cout << "via DFS:\n";
		G.connected_component("DFS");
		cout << "via BFS:\n";
		G.connected_component("BFS");
		
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