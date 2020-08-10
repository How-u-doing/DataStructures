#ifndef UF_H
#define UF_H
#include <vector>

// union-find data structure
// see also <https://en.wikipedia.org/wiki/Disjoint-set_data_structure>
class UF {
// barefoot version, no error handling, no argument checking
public:
	UF(size_t n) : _parent(n, 0) {
		for (size_t i = 0; i < n; ++i)
			_parent[i] = i; // makes each node a root node (parent link to itself)
	}

	// using path compression
	size_t find(size_t x) {
		size_t root = x;
		// get root
		while (_parent[root] != root) root = _parent[root];

		size_t p;
		while (_parent[x] != root) {
			p = _parent[x];
			_parent[x] = root;
			x = p;
		}
		return root;
	}

	void Union(size_t p, size_t q) {
		_parent[find(p)] = find(q); // it doesn't matter if they're in the same set
	}

	bool connected(size_t p, size_t q) {
		return find(p) == find(q);
	}
private:
	std::vector<size_t> _parent;
};

#endif // !UF_H
