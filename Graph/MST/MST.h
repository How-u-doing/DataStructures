#include "../Graph.h"
#include "Heap.h"
#include "UF.h"
#include "../IndexPQ.h"
#include <vector>

struct MST_Edge : myGraph::Edge {
	friend bool operator>(const MST_Edge& lhs, const MST_Edge& rhs) {
		return lhs._cost > rhs._cost;
	}
	MST_Edge(size_t source = 0, const myGraph::Edge& e = myGraph::Edge(0, 0.0))
		: myGraph::Edge(e), _source(source) {}
	size_t _source;
};

// space: O(|E|), time: O(|E|log|E|) 
template<typename T>
double KruskalMST(const myGraph::Graph<T>& G, std::vector<MST_Edge>& mst) {
	myHeap::Heap<MST_Edge, std::greater<MST_Edge>> pq{}; // MinHeap
	double count{ 0.0 };
	UF uf(G.vertex_size());
	// add all edges to heap
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
		count += e._cost;
	}
	return count;
}

// space: O(|E|), time: O(|E|log|E|) 
template<typename T>
double LazyPrimMST(const myGraph::Graph<T>& G, std::vector<MST_Edge>& mst) {
	myHeap::Heap<MST_Edge, std::greater<MST_Edge>> pq{}; // MinHeap
	std::vector<bool> marked(G.vertex_size(), false); // vertices, on mst tree or not on
	double count{ 0.0 };
	add_edges_from(0, G, pq, marked);
	while (!pq.empty()) {
		auto e{ pq.top() }; pq.pop();
		size_t v = e._source, w = e._dest;
		if (marked[v] && marked[w]) continue; // already added these 2 vertices to mst tree
		mst.emplace_back(e); count += e._cost;
		if (!marked[v])  add_edges_from(v, G, pq, marked);
		if (!marked[w])  add_edges_from(w, G, pq, marked);
	}
	return count;
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

// space: O(|V|), time: O(|E|log|V|) 
template<typename T>
double PrimMST(const myGraph::Graph<T>& G, std::vector<MST_Edge>& mst) {
	myIndexPQ::IndexPQ<double, std::greater<double>> pq(G.vertex_size()); // IndexMinPQ
	std::vector<bool> marked(G.vertex_size(), false); // vertices, on mst tree or not on	
	std::vector<double> dist(G.vertex_size(), DBL_MAX);
	std::vector<MST_Edge> prev(G.vertex_size(), MST_Edge(UINT_MAX));
	double count{ 0.0 };
	dist[0] = 0.0;
	pq.insert(0, 0.0);
	while (!pq.empty()) {
		size_t u = pq.top_index(); count += pq.top(); pq.pop();
		marked[u] = true;
		for (const auto& e : G.adj(u)) {
			size_t v = e._dest;
			if (marked[v]) continue;
			if (dist[v] > e._cost) {
				dist[v] = e._cost;
				prev[v] = MST_Edge(u, e);  // new best connection from mst tree to v
				if (pq.contains(v)) pq.change(v, dist[v]);
				else				pq.insert(v, dist[v]);
			}
		}
	}
	// if it's a connected component:
	//mst.assign(prev.begin() + 1, prev.end());
	for (const auto& e : prev) {
		if (e._source == UINT_MAX) continue; // skip 0 and vertices not in the connected component
		mst.emplace_back(e);
	}
	// Note that mst is stored in index order, not showing
	// the edge-adding progress, however, LazyPrimMST does.
	return count;
}