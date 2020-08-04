#include "../Graph.h"
#include "Heap.h"
#include <string>
#include <tuple>
#include <vector>

struct MST_Edge : myGraph::Edge{
	friend bool operator>(const MST_Edge& lhs, const MST_Edge& rhs) {
		return lhs._cost > rhs._cost;
	}
	MST_Edge(size_t source = 0, const myGraph::Edge& e = myGraph::Edge(0, 0.0))
		: myGraph::Edge(e), _source(source){}
	size_t _source;
};


template<typename T>
std::vector<std::tuple<T, T, double>> PrimMST(const myGraph::Graph<T>& G) {
	myHeap::Heap<MST_Edge, std::greater<MST_Edge>> pq{}; // MinHeap
	std::vector<MST_Edge> mst{};
	std::vector<bool> marked(G.vertex_size(), false); // mst vertices, in or not
	add_edges_from(0, G, pq, marked);
	while (!pq.empty()) {
		auto e{ pq.top() }; pq.pop();
		size_t v = e._source, w = e._dest;
		if (marked[v] && marked[w]) continue; // already added these 2 vertices to mst tree
		mst.emplace_back(e);
		if (!marked[v])  add_edges_from(v, G, pq, marked);
		if (!marked[w])  add_edges_from(w, G, pq, marked);
	}

	// convert indices to data
	std::vector<std::tuple<T, T, double>> tree;
	for (size_t i = 0; i < mst.size(); ++i) {
		tree.emplace_back(std::make_tuple(G.vertex(mst[i]._source),
			G.vertex(mst[i]._dest), mst[i]._cost));
	}
	return tree;
}

template<typename T>
void add_edges_from(size_t source, const myGraph::Graph<T>& G,
	myHeap::Heap<MST_Edge, std::greater<MST_Edge>>& pq, std::vector<bool>& marked)
{
	marked[source] = true;	// add source vertex to mst tree
	for (const auto& e : G.adj(source))
		if (!marked[e._dest])
			pq.push(MST_Edge(source, e));
}
