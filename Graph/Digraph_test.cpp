#include "Digraph.h"
#include <string>
#include <iostream>

using namespace std;
using namespace myDigraph;

template<typename T = std::string>
void print_vertex(Digraph<T>& G, typename Digraph<T>::iterator i) {
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
		Digraph<string> G("SymbolGraph_w.txt", weighted);
		cout << "Orginal: \n";
		G.output_graph(cout, weighted);

		auto from{ G.find_vertex("JFK") }, to{ G.find_vertex("LAX") };

		cout << "\nG.has_path(G.find_vertex(\"JFK\"), G.find_vertex(\"LAX\"), \"DFS\")==" <<
			G.has_path(from, to, "DFS");

		vector<Edge> path_DFS{};
		G.path(from, to, path_DFS);
		cout << "\nPaths from JFK to LAX via DFS can be:\n";
		for (const auto& e : path_DFS)
			if (e._dest == G.index(from)) cout << G.vertex(e._dest);
			else cout << " --" << e._cost << "--> " << G.vertex(e._dest);
		cout << "\n";

		cout << "\nG.has_path(G.find_vertex(\"JFK\"), G.find_vertex(\"LAX\"), \"BFS\")==" <<
			G.has_path(from, to, "BFS");

		vector<Edge> path_BFS{};
		G.path(from, to, path_BFS, "BFS");
		cout << "\nPaths from JFK to LAX via BFS can be:\n";
		for (const auto& e : path_BFS)
			if (e._dest == G.index(from)) cout << G.vertex(e._dest);
			else cout << " --" << e._cost << "--> " << G.vertex(e._dest);
		cout << "\n";

		vector<Edge> path_djs{};
		vector<double> dist(G.vertex_size()); vector<size_t> prev(G.vertex_size());
		G.Dijkstra(G.index(from), dist, prev);
		G.path(G.index(from), G.index(to), path_djs, prev);
		cout << "\nShortest paths from JFK to LAX via Dijkstra's algo can be:\n" 
			<< dist[G.index(to)] << ": ";
		for (const auto& e : path_djs)
			if (e._dest == G.index(from)) cout << G.vertex(e._dest);
			else cout << " --" << e._cost << "--> " << G.vertex(e._dest);
		cout << "\n";

		G.remove_edge(G.find_vertex("JFK"), G.find_vertex("MCO"));
		G.remove_vertex(G.find_vertex("PHX"));
		G.remove_vertex(G.find_vertex("HOU"));

		cout << "\nAfter removing edges between \"JFK\" & \"MCO\", and vertex \"PHX\" & \"HOU\" : \n";
		G.output_graph(cout, weighted);

		cout << "\nDFS SymbolGraph_w.txt starting at \"ORD\":\n";
		G.DFS(G.find_vertex("ORD"), print_vertex);

		cout << "\nBFS SymbolGraph_w.txt starting at \"ORD\":\n";
		G.BFS(G.find_vertex("ORD"), print_vertex);

		cout << "\ntinyG.txt: \n";
		G.read_file("tinyG.txt", !weighted);
		G.output_graph(cout, !weighted);

		cout << "\nConnected components: \n";
		cout << "via DFS:\n";
		G.connected_component("DFS");
		cout << "via BFS:\n";
		G.connected_component("BFS");

	}
	catch (const DigraphReadException& e) {
		cout << e.what() << endl;
	}
	catch (const DigraphEdgeException& e) {
		cout << e.what() << endl;
	}
	catch (const DigraphIterOutOfRange& e) {
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