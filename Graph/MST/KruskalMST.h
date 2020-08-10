#include "../Graph.h"
#include "Heap.h"
#include "UF.h"
#include <string>
#include <tuple>
#include <vector>

struct MST_Edge : myGraph::Edge {
	friend bool operator>(const MST_Edge& lhs, const MST_Edge& rhs) {
		return lhs._cost > rhs._cost;
	}
	MST_Edge(size_t source = 0, const myGraph::Edge& e = myGraph::Edge(0, 0.0))
		: myGraph::Edge(e), _source(source) {}
	size_t _source;
};


template<typename T>
std::vector<std::tuple<T, T, double>> KruskalMST(const myGraph::Graph<T>& G) {
	myHeap::Heap<MST_Edge, std::greater<MST_Edge>> pq{}; // MinHeap
	std::vector<MST_Edge> mst{};
	UF uf(G.vertex_size());
	// put all edges into heap
	for (size_t i = 0; i < G.vertex_size(); ++i) {
		for (const auto& e : G.adj(i))
			if (e._dest > i) pq.push(MST_Edge(i, e));
	}

	while (!pq.empty() && mst.size() < G.vertex_size() - 1) {
		auto e{ pq.top() }; pq.pop();
		size_t v = e._source, w = e._dest;
		if (uf.connected(v, w)) continue;
		uf.Union(v, w);
		mst.emplace_back(e);
	}

	// convert indices to data
	std::vector<std::tuple<T, T, double>> tree;
	for (size_t i = 0; i < mst.size(); ++i) {
		tree.emplace_back(std::make_tuple(G.vertex(mst[i]._source),
			G.vertex(mst[i]._dest), mst[i]._cost));
	}
	return tree;
}
