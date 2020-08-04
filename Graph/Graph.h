#ifndef GRAPH_H
#define GRAPH_H
#include <vector>
#include <unordered_map>
#include <queue>
#include <utility> // std::swap
#include <string>
#include <fstream>
#include <iostream>
#include <stdexcept>

// undirected graph
namespace myGraph {

struct GraphReadException : std::runtime_error {
	GraphReadException(const char* mesg) : std::runtime_error(mesg) {}
	GraphReadException(const std::string& mesg) : std::runtime_error(mesg) {}
};

struct GraphEdgeException : std::runtime_error {
	GraphEdgeException(const char* mesg) : std::runtime_error(mesg) {}
	GraphEdgeException(const std::string& mesg) : std::runtime_error(mesg) {}
};

struct GraphInternalException : std::runtime_error {
	GraphInternalException(const char* mesg) : std::runtime_error(mesg) {}
	GraphInternalException(const std::string& mesg) : std::runtime_error(mesg) {}
};

struct GraphIterOutOfRange : std::out_of_range {
	GraphIterOutOfRange(const char* mesg) : std::out_of_range(mesg) {}
	GraphIterOutOfRange(const std::string& mesg) : std::out_of_range(mesg) {}
};

struct Edge {
	Edge(size_t dest, double cost = 0.0) :_dest(dest), _cost(cost) {}
	size_t _dest;	// pos of the other vertex in adjacent list
	double _cost;	// weight
};

template<typename T = std::string>
class Graph {
public:
	using iterator = typename std::unordered_map<T, size_t>::iterator;
	using const_iterator = typename std::unordered_map<T, size_t>::const_iterator;

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
			add_vertex(v);	add_vertex(w);	// do nothing if exist
			auto p1{ find_vertex(v) }, p2{ find_vertex(w) };
			if (!has_edge_unchecked(p1, p2))
				add_edge_by_index_unchecked(p1->second, p2->second, cost);
			else // or we may just need to overwrite it & print a message
				throw GraphReadException("Found edge duplicated while reading \"" + filename + "\"");
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

	void clear() { _adj.clear(); _map.clear(); _keys.clear();  _E = 0; }

	void output_graph(std::ostream& os, bool weighted = true) {
		for (size_t i = 0; i < vertex_size(); ++i) {
			os << _keys[i] << '\n';
			for (const auto& x : _adj[i]) {
				os << "   " << _keys[x._dest];
				if (weighted)
					os << "\t" << x._cost;
				os << '\n';
			}
		}
	}
private:
	size_t index(iterator i) const {
		if (i == end()) throw GraphIterOutOfRange("Cannot get vertex to end()");
		return i->second;
	}
	size_t index(const T& v) const {
		return _map.at(v);
	}
public:
	T vertex(iterator i) const {
		if(i==end()) throw GraphIterOutOfRange("Cannot get vertex to end()");
		return i->first;
	}
	T vertex(size_t i) const {
		return _keys.at(i);
	}
	std::vector<Edge> adj(size_t i) const {
		return _adj.at(i);
	}
	
	size_t vertex_size() const noexcept { return _adj.size(); }
	size_t edge_size() const noexcept { return _E; }

	iterator begin() noexcept { return _map.begin(); };
	iterator end() noexcept { return _map.end(); };
	const_iterator begin() const noexcept { return _map.begin(); };
	const_iterator end() const noexcept { return _map.end(); };

	iterator find_vertex(const T& v) { return _map.find(v); }
	const_iterator find_vertex(const T& v) const { return _map.find(v); }

	// since this is an undirected graph, from/to are equivalent
	bool has_edge(const_iterator from, const_iterator to) const {
		if (from == end() || to == end()) return false;
		return has_edge_unchecked(from, to);
	}

	void add_edge(iterator from, iterator to, double cost = 0.0) {
		if (from == end() || to == end())
			throw GraphIterOutOfRange("Cannot add edge from/to vertex to end()");
		if (has_edge_unchecked(from, to))
			throw GraphEdgeException("Cannot add edge since it already existed");
		else
			add_edge_by_index_unchecked(from->second, to->second, cost);
	}

	void modify_weight(iterator v, iterator w, double cost) {
		if (v == end() || w == end())
			throw GraphIterOutOfRange("Cannot modify edge from/to vertex to end()");
		auto p{ find_dest(v->second, w->second) }, q{ find_dest(w->second, v->second) };
		if (p != _adj[w->second].end() && q != _adj[v->second].end()) {
			p->_cost = cost; q->_cost = cost;
		}
		else if (p == _adj[w->second].end() && q == _adj[v->second].end())
			throw GraphEdgeException("Cannot modify edge since it doesn't exist");
		else // should never happen
			throw GraphInternalException("Graph internal structure damaged");
	}
	
	void remove_edge(iterator v, iterator w) {
		if (v == end() || w == end())
			throw GraphIterOutOfRange("Cannot remove edge from/to vertex to end()");
		auto p{ find_dest(v->second, w->second) }, q{ find_dest(w->second, v->second) };
		if (p != _adj[w->second].end() && q != _adj[v->second].end()) {
			_adj[w->second].erase(p); _adj[v->second].erase(q);
		}
		else if (p == _adj[w->second].end() && q == _adj[v->second].end())
			throw GraphEdgeException("Cannot remove edge since it doesn't exist");
		else
			throw GraphInternalException("Graph internal structure damaged");
	}
	
	// add vertex, if it dosen't exist
	void add_vertex(const T& v) {
		if (find_vertex(v) == end())
			add_vertex_unchecked(v);
	}

	// remove vertex, if any
	void remove_vertex(iterator v) {
		if (v == end()) throw GraphIterOutOfRange("Cannot remove vertex to end()");
		size_t vi{ v->second };	// index of v
		// remove all edges point to v
		for (const auto& w : _adj[vi])
			remove_edge_v_in_w(vi, w._dest);

		size_t last{ vertex_size() - 1 };
		// swap vertex v with last vertex & change relative edges
		if (vi != last) {
			_adj[vi].swap(_adj[last]);
			// modify all edges that have a destination to last with vi
			for (const auto& x : _adj[vi]) {
				auto it = find_dest(last, x._dest);
				if (it != _adj[x._dest].end())	it->_dest = vi;
				else throw GraphEdgeException("Cannot add edge since it already existed");
			}
			// swap vertices in associated containers
			std::swap(_map.find(_keys[last])->second, v->second);
			std::swap(_keys[last], _keys[vi]);
		}
		// remove vertex with index 'last'
		_adj.pop_back();
		_map.erase(v);
		_keys.pop_back();			
	}

	bool has_path(iterator from, iterator to, const std::string& mode = "DFS") {
		std::vector<bool> marked(vertex_size(), false);
		if (mode == "DFS") DFS(from, marked);
		else BFS(from, marked);
		return marked[index(to)];
	}

	void path(iterator from, iterator to, std::vector<T>& path, const std::string& mode = "DFS") {
		if (from == end() || to == end())
			throw GraphIterOutOfRange("DFS_path iterator out of range");
		std::vector<bool> marked(vertex_size(), false);
		std::vector<size_t> edge_to(vertex_size(), 0);
		size_t s = from->second, v = to->second;
		if (mode == "DFS")	DFS_path(s, marked, edge_to);
		else BFS_path(s, marked, edge_to);
		if (!marked[index(to)]) return;	// no path
		// add paths in reverse order
		for (size_t x = v; x != s; x = edge_to[x])
			path.push_back(_keys[x]);
		path.push_back(_keys[s]); // add source to 'beginning' (after reverse)
		// reverse back
		for (auto p1 = path.begin(), p2 = path.end() - 1; p1 < p2;) {
			std::swap(*p1++, *p2--);
		}
		// note that we can also use an auxiliary integer vector for
		// storing the vertex indices, then copy back in reverse order
		/*for (auto p = ipath.rbegin(); p != ipath.rend(); ++p)
			path.push_back(_keys[*p]);*/
		// where ipath is a vector<size_t> replaced path before
	}

	void connected_component(const std::string& mode = "DFS") {
		// A good way to reduce the times of tell if visited is to get a vertex
		// from each connected component of the graph, the overhead of which is,
		// in general, regardless of time complexity or algorithm complexity, 
		// greater than each vertex traversal. So let's just do it :)
		std::vector<bool> marked(vertex_size(), false);
		size_t count{ 0 };
		std::unordered_map<T, size_t> ump;
		for (auto it = _map.begin(); it != _map.end(); ++it) {
			if (!marked[it->second]) {
				if (mode == "DFS")	DFS_cc(it->second, marked, count, ump);
				else BFS_cc(it->second, marked, count, ump);
				++count;
			}
		}
		auto components = new std::vector<T>[count] {};
		// print connected components
		for (auto it : ump)
			components[it.second].push_back(it.first);
		std::cout << count << " components\n";
		for (size_t i = 0; i < count; ++i) {
			for (const auto& x : components[i])
				std::cout << x << " ";
			std::cout << '\n';
		}
		delete[] components;
	}
private:
	void DFS_path(size_t s, std::vector<bool>& marked, std::vector<size_t>& edge_to) {
		marked[s] = true;
		for (const auto& x : _adj[s])
			if (!marked[x._dest]) {
				edge_to[x._dest] = s; // mark source vertex to destination vertex
				DFS_path(x._dest, marked, edge_to);
			}
	}

	void BFS_path(size_t s, std::vector<bool>& marked, std::vector<size_t>& edge_to) {
		std::queue<size_t> q{};
		q.push(s);
		marked[s] = true;
		while (!q.empty()) {
			s = q.front(); q.pop();
			for (const auto& x : _adj[s])
				if (!marked[x._dest]) {
					edge_to[x._dest] = s; // mark source vertex to destination vertex
					q.push(x._dest);
					marked[x._dest] = true;
				}
		}
	}

	void DFS_cc(size_t s, std::vector<bool>& marked, size_t count, std::unordered_map<T, size_t>& ump) {
		marked[s] = true;
		ump[_keys[s]] = count;
		for (const auto& x : _adj[s])
			if (!marked[x._dest])
				DFS_cc(x._dest, marked, count, ump);
	}

	void BFS_cc(size_t s, std::vector<bool>& marked, size_t count, std::unordered_map<T, size_t>& ump) {
		std::queue<size_t> q{};
		q.push(s);
		marked[s] = true;
		ump[_keys[s]] = count;
		while (!q.empty()) {
			s = q.front(); q.pop();
			for (const auto& x : _adj[s])
				if (!marked[x._dest]) {
					ump[_keys[x._dest]] = count;
					q.push(x._dest);
					marked[x._dest] = true;
				}
		}
	}

	void DFS(size_t s, std::vector<bool>& marked,
		void(*visit)(Graph<T>& G, iterator i) = [](Graph<T>& G, iterator i) {/*dummy*/}) {
		visit(*this, find_vertex(_keys[s]));
		// we can also set a counter for the number of vertices connected to source s
		// i.e. the size of this connected component |CC|
		// ++count;
		marked[s] = true;
		for (const auto& x : _adj[s])
			if (!marked[x._dest])
				DFS(x._dest, marked, visit);
	}
public:
	// traverse a connected component from a source vertex
	void DFS(iterator start, std::vector<bool>& marked, 
		void(*visit)(Graph<T>& G, iterator i) = [](Graph<T>& G, iterator i) {/*dummy*/}) {
		if (start == end()) throw GraphIterOutOfRange("DFS starting iterator out of range");
		DFS(start->second, marked, visit);
	}

	void BFS(iterator start, std::vector<bool>& marked,
		void(*visit)(Graph<T>& G, iterator i) = [](Graph<T>& G, iterator i) {/*dummy*/}) {
		if (start == end()) throw GraphIterOutOfRange("BFS starting iterator out of range");
		std::queue<size_t> q{};
		size_t s{ start->second };
		q.push(s);
		marked[s] = true;
		while (!q.empty()) {
			s = q.front(); q.pop();
			visit(*this, find_vertex(_keys[s]));
			for (const auto& x : _adj[s])
				if (!marked[x._dest]) {
					q.push(x._dest);
					marked[x._dest] = true;
				}
		}
	}

private:
	// data member default initialization
	std::vector<std::vector<Edge>> _adj{};	// adjacent list
	std::unordered_map<T, size_t> _map{};	// vertex -> index
	std::vector<T> _keys{};					// index -> vertex
	size_t _E{ 0 };	// number of edges

	// require that from and to must be prior to
	// end() so that they are dereferenceable
	bool has_edge_unchecked(const_iterator from, const_iterator to) const {
		return find_dest(to->second, from->second) != _adj[from->second].end();
	}

	// no check if the edge already exists
	void add_edge_by_index_unchecked(size_t v, size_t w, double cost = 0.0) {
		_adj[v].push_back(Edge(w, cost));
		_adj[w].push_back(Edge(v, cost));
		++_E;
	}

	// no check if v exists
	void add_vertex_unchecked(const T& v) {
		_adj.push_back(std::vector<Edge>{});
		_map[v] = _adj.size() - 1;
		_keys.emplace_back(v);
	}

	void remove_edge_v_in_w(size_t v, size_t w) {
		auto p{ find_dest(v, w) };
		if (p != _adj[w].end()) {
			_adj[w].erase(p);	--_E;
		}
	}

	// get the iterator in _adj[source] that has an edge dest value
	// return _adj[source].end() if not found
	std::vector<Edge>::const_iterator find_dest(size_t dest, size_t source) const {
		auto it = _adj[source].begin();
		for (; it != _adj[source].end(); ++it)
			if (it->_dest == dest) break;
		return it;
	}

	std::vector<Edge>::iterator find_dest(size_t dest, size_t source) {
		auto it = _adj[source].begin();
		for (; it != _adj[source].end(); ++it)
			if (it->_dest == dest) break;
		return it;
	}

	void modify_weight_unchecked(iterator from, iterator to, double cost) {
		find_dest(to->second, from->second)->_cost = cost;
		find_dest(from->second, to->second)->_cost = cost;
	}
};

}// namespace myGraph

#endif // !GRAPH_H


