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

		vector<Edge> path_DFS{};
		G.path(G.find_vertex("LAS"), G.find_vertex("MCO"), path_DFS);
		cout << "\nPaths from LAS to MCO via DFS can be:\n";
		for(const auto & e : path_DFS)
			if (e._dest == G.index("LAS")) cout << G.vertex(e._dest);
			else cout << " --" << e._cost << "--> " << G.vertex(e._dest);
		cout << "\n";

		cout << "\nG.has_path(G.find_vertex(\"LAS\"), G.find_vertex(\"MCO\"), \"BFS\")==" <<
			G.has_path(G.find_vertex("LAS"), G.find_vertex("MCO"), "BFS");

		vector<Edge> path_BFS{};
		G.path(G.find_vertex("LAS"), G.find_vertex("MCO"), path_BFS, "BFS");
		cout << "\nPaths from LAS to MCO via BFS can be:\n";
		for (const auto& e : path_BFS)
			if (e._dest == G.index("LAS")) cout << G.vertex(e._dest);
			else cout << " --" << e._cost << "--> " << G.vertex(e._dest);
		cout << "\n";

		vector<Edge> path_djs{};
		vector<double> dist(G.vertex_size()); vector<size_t> prev(G.vertex_size());
		G.Dijkstra(G.find_vertex("LAS"), dist, prev);
		G.path(G.index("LAS"), G.index("MCO"), path_djs, prev);
		cout << "\nShortest paths from LAS to MCO via Dijkstra's algo can be:\n"
			<< dist[G.index("MCO")] << ": ";
		for (const auto& e : path_djs)
			if (e._dest == G.index("LAS")) cout << G.vertex(e._dest);
			else cout << " --" << e._cost << "--> " << G.vertex(e._dest);
		cout << "\n";

		G.remove_edge(G.find_vertex("JFK"), G.find_vertex("MCO"));
		G.remove_vertex(G.find_vertex("PHX"));
		G.remove_vertex(G.find_vertex("HOU"));

		cout << "\nAfter removing edges between \"JFK\" & \"MCO\", and vertex \"PHX\" & \"HOU\" : \n";
		G.output_graph(cout, weighted);

		cout << "\nDFS SymbolGraph_w.txt starting at \"LAS\":\n";
		G.DFS(G.find_vertex("LAS"), print_vertex);

		cout << "\nBFS SymbolGraph_w.txt starting at \"LAS\":\n";
		G.BFS(G.find_vertex("LAS"), print_vertex);

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
	catch (const exception& e) {
		cout << e.what() << endl;
	}
	catch (...) {
		cout << "Some unknown error occured" << endl;
	}

	return 0;
}