#ifndef GRAPH_H
#define GRAPH_H
#include <vector>
#include <map>
#include <fstream>
#include <stdexcept>

// undirected graph
namespace myGraph {

struct GraphReadException : std::runtime_error {
	GraphReadException(const char* mesg) : std::runtime_error(mesg) {}
	GraphReadException(const std::string& mesg) : std::runtime_error(mesg) {}
};

struct Edge {
	Edge(size_t dist, double cost = 0.0) :_dist(dist), _cost(cost) {}
	size_t _dist;	// pos of the other vertex in adjacent list
	double _cost;	// weight
};

template<typename T = std::string>
class Graph {
public:
	Graph() {}

	Graph(const std::string& filename, bool weighted = true) {
		std::ifstream ifs(filename, std::ios_base::in);		
		if (!ifs.is_open())
			throw std::runtime_error("Error opening file \"" + filename + "\"");

		double cost{ 0.0 };
		T v, w;
		while(!ifs.eof()) {
			if (!(ifs >> v >> w)) throw GraphReadException("Error reading vertex from \"" + filename + "\"");
			if (weighted) {
				if (!(ifs >> cost)) throw GraphReadException("Error reading edge weight from \"" + filename + "\"");
			}
			add_edge(v, w, cost);
		}
		// file will be closed when leaving scope (regardless of exception),
		// guaranteed by RAII (Resource Acquisition Is Initialization)
	}

	void read_file(const std::string& filename, bool weighted = true) {
		clear();
		Graph G(filename, weighted);
		std::swap(G, *this);
	}

	~Graph() {}

	void clear() { _adj.clear(); _map.clear(); _E = 0; }

	void output_graph(std::ostream& os, bool weighted = true) {
		// invert indices to keys
		T* keys = new T[_map.size()]{};
		for (const auto& pair: _map)
			keys[pair.second] = pair.first;

		for (size_t i = 0; i < vertex_size(); ++i) {
			os << keys[i] << '\n';
			for (const auto& x : _adj[i]) {
				os << "   " << keys[x._dist];
				if (weighted)
					os << "\t" << x._cost;
				os << '\n';
			}
		}
		delete[] keys;
	}

	size_t vertex_size() const noexcept { return _adj.size(); }
	size_t edge_size() const noexcept { return _E; }

	// if vertex v, w are not present, add them; then add edge
	void add_edge(const T& v, const T& w, double cost = 0.0) {
		add_vertex(v);	add_vertex(w);	// do nothing if exist
		add_edge_by_indices(_map[v], _map[w], cost);
		++_E;
	}

	// add vertex if it dosen't exist
	void add_vertex(const T& v) {
		if (!has_vertex(v))
			add_vertex_unchecked(v);
	}

	bool has_vertex(const T& v) {
		return _map.find(v) != _map.end();
	}


private:
	std::vector<std::vector<Edge>> _adj{};
	std::map<T, size_t> _map{};	// vertex -> indices
	size_t _E{ 0 };	// number of edges

	// no check for the validity of v, w
	// at() checks it but with time overhead
	void add_edge_by_indices(int v, int w, double cost) {
		_adj[v].push_back(Edge(w, cost));
		_adj[w].push_back(Edge(v, cost));
	}

	// no check if v exists
	void add_vertex_unchecked(const T& v) {
		_adj.push_back(std::vector<Edge>{});
		_map[v] = _adj.size() - 1;
	}
};

}// namespace myGraph

#endif // !GRAPH_H


