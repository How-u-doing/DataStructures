#include "Digraph.h"
#include "Graph.h"
#include <string>
#include <iostream>

using namespace std;

template<typename Graph>
void print_cycles(const Graph& G, const vector<size_t>& cycle) {
	if (!cycle.empty()) {
		cout << "\nFound cycles:\n";
		bool is_begin = true;
		size_t begin{};
		for (size_t i = 0; i < cycle.size(); ++i) {
			if (is_begin) {
				begin = cycle[i];
				is_begin = false;
			}
			else {
				if (cycle[i] == begin) {
					cout << G.vertex(cycle[i]) << '\n';
					is_begin = true;
					continue;
				}
			}
			cout << G.vertex(cycle[i]) << " --> ";
		}
	}
	else cout << "\nThis directed is acyclic\n";
}

int main()
{
	using namespace myDigraph;
	using namespace myGraph;

	bool weighted = true;
	try {
		//Digraph<string> G("SymbolGraph_w.txt", weighted); // acyclic
		Digraph<string> G("SymbolGraph_w2.txt", weighted);
		cout << "Orginal digraph: \n";
		G.output_graph(cout, weighted);

		std::vector<size_t> cycle{};
		G.has_cycle(cycle);

		print_cycles(G, cycle);
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

	try {
		Graph<string> G("cycle.txt", !weighted);
		//Graph<string> G("SymbolGraph.txt", !weighted);
		cout << "\nOrginal graph: \n";
		G.output_graph(cout, !weighted);

		std::vector<size_t> cycle{};
		G.has_cycle(cycle);

		print_cycles(G, cycle);
	}
	catch (const GraphReadException& e) {
		cout << e.what() << endl;
	}
	catch (const GraphEdgeException& e) {
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