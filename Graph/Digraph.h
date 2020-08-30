#ifndef DIGRAPH_H
#define DIGRAPH_H
#include "IndexPQ.h"
#include <cfloat> // DBL_MAX for Dijkstra's algo
#include <vector>
#include <unordered_map>
#include <queue>
#include <utility> // std::swap
#include <string>
#include <fstream>
#include <iostream>
#include <stdexcept>

// undirected graph
namespace myDigraph {

struct DigraphReadException : std::runtime_error {
	DigraphReadException(const char* mesg) : std::runtime_error(mesg) {}
	DigraphReadException(const std::string& mesg) : std::runtime_error(mesg) {}
};

struct DigraphEdgeException : std::runtime_error {
	DigraphEdgeException(const char* mesg) : std::runtime_error(mesg) {}
	DigraphEdgeException(const std::string& mesg) : std::runtime_error(mesg) {}
};

struct DigraphIterOutOfRange : std::out_of_range {
	DigraphIterOutOfRange(const char* mesg) : std::out_of_range(mesg) {}
	DigraphIterOutOfRange(const std::string& mesg) : std::out_of_range(mesg) {}
};

struct Edge {
	Edge(size_t dest, double cost = 0.0) :_dest(dest), _cost(cost) {}
	size_t _dest;	// pos of the other vertex in adjacent list
	double _cost;	// weight
};

// directed graph
template<typename T = std::string>
class Digraph {
public:
	using iterator = typename std::unordered_map<T, size_t>::iterator;
	using const_iterator = typename std::unordered_map<T, size_t>::const_iterator;

	Digraph() {}

	Digraph(const std::string& filename, bool weighted = true) {
		std::ifstream ifs(filename, std::ios_base::in);
		if (!ifs.is_open())
			throw std::runtime_error("Error opening file \"" + filename + "\"");

		double cost{ 0.0 };
		T v, w;
		while (!ifs.eof()) {
			if (!(ifs >> v >> w)) throw DigraphReadException("Error reading vertex from \"" + filename + "\"");
			if (weighted) {
				if (!(ifs >> cost)) throw DigraphReadException("Error reading edge weight from \"" + filename + "\"");
			}
			add_vertex(v);	add_vertex(w);	// do nothing if exist
			auto p1{ find_vertex(v) }, p2{ find_vertex(w) };
			if (!has_edge_unchecked(p1, p2))
				add_edge_unchecked(p1->second, p2->second, cost);
			else // or we may just need to overwrite it & print a message
				throw DigraphReadException("Found edge duplicated while reading \"" + filename + "\"");
		}
		// file will be closed when leaving scope (regardless of exception),
		// guaranteed by RAII (Resource Acquisition Is Initialization)
	}

	void read_file(const std::string& filename, bool weighted = true) {
		clear();
		Digraph G(filename, weighted);
		std::swap(G, *this);
	}

	~Digraph() {}

	void clear() { _adj.clear(); _map.clear(); _keys.clear();  _E = 0; }

	void output_graph(std::ostream& os, bool weighted = true) {
		for (size_t i = 0; i < vertex_size(); ++i) {
			os << _keys[i] << '\n';
			for (const auto& e : _adj[i]) {
				os << "   " << _keys[e._dest];
				if (weighted)
					os << "\t" << e._cost;
				os << '\n';
			}
		}
	}
	size_t vertex_size() const noexcept { return _adj.size(); }
	size_t edge_size() const noexcept { return _E; }

	iterator begin() noexcept { return _map.begin(); };
	iterator end() noexcept { return _map.end(); };
	const_iterator begin() const noexcept { return _map.begin(); };
	const_iterator end() const noexcept { return _map.end(); };

	iterator find_vertex(const T& v) { return _map.find(v); }
	const_iterator find_vertex(const T& v) const { return _map.find(v); }
	bool has_vertex(const T& v) const { return find_vertex(v) != end(); }

	size_t index(iterator i) const {
		verify_iterator(i, "@index iterator out of range");
		return i->second;
	}
	size_t index(const T& v) const {
		return _map.at(v);
	}
	T vertex(iterator i) const {
		verify_iterator(i, "@vertex iterator out of range");
		return i->first;
	}
	T vertex(size_t i) const {
		return _keys.at(i);
	}
	std::vector<Edge> adj(size_t i) const {
		return _adj.at(i);
	}
private:
	void verify_iterator(iterator from, iterator to, const std::string& msg) const {
		if (from == end() || to == end()) throw DigraphIterOutOfRange(msg);
	}
	void verify_iterator(iterator it, const std::string& msg) const {
		if (it == end()) throw DigraphIterOutOfRange(msg);
	}
public:
	bool has_edge(iterator from, iterator to) const {
		verify_iterator(from, to, "@has_edge iterator out of range");
		return has_edge_unchecked(from, to);
	}

	void add_edge(iterator from, iterator to, double cost = 0.0) {
		verify_iterator(from, to, "@add_edge iterator out of range");
		if (has_edge_unchecked(from, to))
			throw DigraphEdgeException("Cannot add edge since it already existed");
		else
			add_edge_unchecked(from->second, to->second, cost);
	}

	double weight(iterator from, iterator to) const {
		verify_iterator(from, to, "@weight iterator out of range");
		return weight_unchecked(from->second, to->second);
	}
	Edge edge(iterator from, iterator to) const {
		verify_iterator(from, to, "@edge iterator out of range");
		return edge_unchecked(from->second, to->second);
	}
private:
	double weight_unchecked(size_t from, size_t to) const { return find_dest(to, from)->_cost; }
	Edge edge_unchecked(size_t from, size_t to) const  { return *find_dest(to, from); }

public:
	void modify_weight(iterator from, iterator to, double cost) {
		verify_iterator(from, to, "@modify_weight iterator out of range");
		auto p{ find_dest(to->second, from->second) };
		if (p != _adj[from->second].end()) p->_cost = cost;
		else throw DigraphEdgeException("Cannot modify edge since it doesn't exist");
	}

	void remove_edge(iterator from, iterator to) {
		verify_iterator(from, to, "@remove_edge iterator out of range");
		auto p{ find_dest(to->second, from->second) };
		if (p != _adj[from->second].end()) { _adj[from->second].erase(p); --_E; }
		else throw DigraphEdgeException("Cannot remove edge since it doesn't exist");
	}

	// add vertex, if it dosen't exist
	void add_vertex(const T& v) {
		if (!has_vertex(v)) add_vertex_unchecked(v);
	}

	// remove vertex, if any
	void remove_vertex(iterator v) {
		verify_iterator(v, "@remove_vertex iterator out of range");
		size_t vi{ v->second };	// index of v
		// remove all edges point to v
		for (size_t i = 0; i < vertex_size(); ++i) {
			if (i == vi) continue;
			for (auto j = _adj[i].begin(); j != _adj[i].end(); ++j)
				if (j->_dest == vi) { _adj[i].erase(j); --_E; break; }
		}
		size_t last{ vertex_size() - 1 };
		// swap vertex from with last vertex & change relative edges
		if (vi != last) {
			_adj[vi].swap(_adj[last]);
			// redirect all edges that pointed to last to vi
			for (size_t i = 0; i < vertex_size(); ++i) {
				if (i == vi) continue;
				for (auto j = _adj[i].begin(); j != _adj[i].end(); ++j)
					if (j->_dest == last) j->_dest = vi;
			}
			// swap vertices in associated containers
			std::swap(_map.find(_keys[last])->second, v->second);
			std::swap(_keys[last], _keys[vi]);
		}
		_E -= _adj[last].size();
		// remove vertex with index 'last'
		_adj.pop_back();
		_map.erase(v);
		_keys.pop_back();
	}

	bool has_path(iterator from, iterator to, const std::string& mode = "DFS") {
		verify_iterator(from, to, "@has_path iterator out of range");
		std::vector<bool> marked(vertex_size(), false);
		if (mode == "DFS") DFS(from->second, marked);
		else			   BFS(from->second, marked);
		return marked[index(to)];
	}

	void path(iterator from, iterator to, std::vector<Edge>& paths, const std::string& mode = "DFS") {
		verify_iterator(from, to, "@path iterator out of range");
		std::vector<bool> marked(vertex_size(), false);
		std::vector<size_t> edge_to(vertex_size(), 0);
		size_t s = from->second, v = to->second;
		if (mode == "DFS")	DFS_path(s, marked, edge_to);
		else				BFS_path(s, marked, edge_to);
		if (!marked[index(to)]) return;	// no path
		path(s, v, paths, edge_to);
	}

	void path(size_t from, size_t to, std::vector<Edge>& paths, std::vector<size_t>& prev) {
		// add paths in reverse order
		for (size_t x = to; x != from; x = prev[x])
			paths.push_back(edge_unchecked(prev[x], x));
		paths.push_back(Edge(from, 0.0)); // add source to 'beginning' (after reverse)
		// reverse back
		reverse(paths.begin(), paths.end());
	}

	void has_cycle(std::vector<size_t>& cycle) {
		size_t N{ vertex_size() };
		std::vector<bool> marked(N, false);
		std::vector<bool> on_stack(N, false); // is a vertex on a call stack (by dfs)
		std::vector<size_t> edge_to(N); // record the cycle(s)
		for (size_t i = 0; i < N; ++i)
			if (!marked[i])
				DFS_hc(i, marked, on_stack, edge_to, cycle);
	}
private:
	void DFS_hc(size_t s, std::vector<bool>& marked, std::vector<bool>& on_stack,
		std::vector<size_t>& edge_to, std::vector<size_t>& cycle)
	{
		marked[s] = true;
		on_stack[s] = true; // this vertex s is now on an active call stack
		for (const auto& e : _adj[s]) {
			size_t w = e._dest;
			if (!marked[w]) {
				edge_to[w] = s; DFS_hc(w, marked, on_stack, edge_to, cycle);
			}
			else if (on_stack[w]) {// found a cycle; each vertex will be only active once
				auto offset{ cycle.size() }; // starting position of this cycle
				for (size_t x = s; x != w; x = edge_to[x])
					cycle.push_back(x);
				cycle.push_back(w);
				cycle.push_back(s); // s->w->...->s
				reverse(cycle.begin() + offset, cycle.end());
			}
		}
		on_stack[s] = false; // pop off call stack s (inactive now)
	}

	template<typename It>
	void reverse(It first, It last) {
		--last; // locates to last element
		while (first != last && (last + 1) != first) // while (first < last)
			std::swap(*first++, *last--);
	}
public:
	void Dijkstra(iterator from, std::vector<double>& dist, std::vector<size_t>& prev)
	{
		verify_iterator(from, "@Dijkstra iterator out of range");
		size_t s{ from->second };
		myIndexPQ::IndexPQ<double, std::greater<double>> pq(vertex_size()); // IndexMinPQ
		for (size_t v = 0; v != vertex_size(); ++v)
			dist[v] = DBL_MAX;
		dist[s] = 0.0;

		pq.insert(s, 0.0);
		while (!pq.empty()) {
			size_t u = pq.top_index(); pq.pop();
			for (const auto& e : _adj[u]) {
				size_t v = e._dest;
				if (dist[v] > dist[u] + e._cost) {// Note that now dist[u] is the largest among the known sp nodes,
					dist[v] = dist[u] + e._cost;  // hence, the if condition indicates v is not visited, otherwise
					prev[v] = u;                  // we would have dist[v]<dist[u]. So, no need for a marked array.
					if (pq.contains(v)) pq.change(v, dist[v]);
					else				pq.insert(v, dist[v]);
				}
			}
		}
	}
	void connected_component(const std::string& mode = "DFS") {
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
		for (const auto& e : _adj[s])
			if (!marked[e._dest]) {
				edge_to[e._dest] = s; // mark source vertex to destination vertex
				DFS_path(e._dest, marked, edge_to);
			}
	}

	void BFS_path(size_t s, std::vector<bool>& marked, std::vector<size_t>& edge_to) {
		std::queue<size_t> q{};
		q.push(s);
		marked[s] = true;
		while (!q.empty()) {
			s = q.front(); q.pop();
			for (const auto& e : _adj[s])
				if (!marked[e._dest]) {
					edge_to[e._dest] = s; // mark source vertex to destination vertex
					q.push(e._dest);
					marked[e._dest] = true;
				}
		}
	}

	void DFS_cc(size_t s, std::vector<bool>& marked, size_t count, std::unordered_map<T, size_t>& ump) {
		marked[s] = true;
		ump[_keys[s]] = count;
		for (const auto& e : _adj[s])
			if (!marked[e._dest])
				DFS_cc(e._dest, marked, count, ump);
	}

	void BFS_cc(size_t s, std::vector<bool>& marked, size_t count, std::unordered_map<T, size_t>& ump) {
		std::queue<size_t> q{};
		q.push(s);
		marked[s] = true;
		ump[_keys[s]] = count;
		while (!q.empty()) {
			s = q.front(); q.pop();
			for (const auto& e : _adj[s])
				if (!marked[e._dest]) {
					ump[_keys[e._dest]] = count;
					q.push(e._dest);
					marked[e._dest] = true;
				}
		}
	}

	void DFS(size_t s, std::vector<bool>& marked,
		void(*visit)(Digraph<T>& G, iterator i) = [](Digraph<T>& G, iterator i) {/*dummy*/}) {
		visit(*this, find_vertex(_keys[s]));
		// we can also set a counter for the number of vertices connected to source s
		// i.e. the size of this connected component |CC|
		// ++count;
		marked[s] = true;
		for (const auto& e : _adj[s])
			if (!marked[e._dest])
				DFS(e._dest, marked, visit);
	}

	void BFS(size_t s, std::vector<bool>& marked,
		void(*visit)(Digraph<T>& G, iterator i) = [](Digraph<T>& G, iterator i) {/*dummy*/}) {
		std::queue<size_t> q{}; // use stack instead for iterative DFS
		q.push(s);
		marked[s] = true;
		while (!q.empty()) {
			s = q.front(); q.pop();
			visit(*this, find_vertex(_keys[s]));
			for (const auto& e : _adj[s])
				if (!marked[e._dest]) {
					q.push(e._dest);
					marked[e._dest] = true;
				}
		}
	}
public:
	// traverse a connected component from a source vertex
	void DFS(iterator start,
		void(*visit)(Digraph<T>& G, iterator i) = [](Digraph<T>& G, iterator i) {/*dummy*/}) {
		verify_iterator(start, "@DFS iterator out of range");
		std::vector<bool> marked(vertex_size(), false);
		DFS(start->second, marked, visit);
	}

	void BFS(iterator start,
		void(*visit)(Digraph<T>& G, iterator i) = [](Digraph<T>& G, iterator i) {/*dummy*/}) {
		verify_iterator(start, "@BFS iterator out of range");
		std::vector<bool> marked(vertex_size(), false);
		BFS(start->second, marked, visit);
	}

private:
	// data member default initialization
	std::vector<std::vector<Edge>> _adj{};	// adjacent list
	std::unordered_map<T, size_t> _map{};	// vertex -> index
	std::vector<T> _keys{};					// index -> vertex
	size_t _E{ 0 };	// number of edges

	// require that from and to must be prior to end() so that they are dereferenceable
	bool has_edge_unchecked(iterator from, iterator to) const {
		return find_dest(to->second, from->second) != _adj[from->second].end();
	}

	// no check if the edge already exists
	void add_edge_unchecked(size_t from, size_t to, double cost = 0.0) {
		_adj[from].push_back(Edge(to, cost));
		++_E;
	}

	void add_vertex_unchecked(const T& v) {
		_adj.push_back(std::vector<Edge>{});
		_map[v] = _adj.size() - 1;
		_keys.push_back(v);
	}

	void remove_edge_unchecked(size_t from, size_t to) {
		_adj[from].erase(find_dest(to, from));	--_E;
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
	}
};

}// namespace myDigraph

#endif // !DIGRAPH_H

