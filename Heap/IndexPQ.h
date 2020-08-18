#ifndef INDEXPQ_H
#define INDEXPQ_H
#include <vector>
#include <climits> // std::UINT_MAX

namespace myIndexPQ {

template <typename T, typename Compare = std::greater<T>>
class IndexPQ {
public:
	// _comp must be initialized in initializer list (i.e. : _comp(comp){})
	// when Compare is a decltype(lambda_function)
	IndexPQ(size_t capacity, Compare comp = Compare{})
		: _N{0}, _keys{ std::vector<T>(capacity, T{}) }, _comp{ comp },
		_pq{ std::vector<size_t>(capacity, UINT_MAX) },
		_qp{ std::vector<size_t>(capacity, UINT_MAX) } {}
		
	~IndexPQ() { clear(); }

	void clear() { _keys.clear(); _pq.clear(); _qp.clear(); }

	bool empty() const noexcept { return _N == 0; }
	size_t size() const noexcept { return _N; }
	bool contains(size_t k) const { return _qp.at[k] != UINT_MAX; }

	const T& top() const { return _keys[_pq[0]]; }
	size_t top_index() const { return _pq[0]; }

	void insert(size_t k, const T& key) {
		_keys.at(k) = key; ++_N;
		_pq[_N - 1] = k;
		_qp[k] = _N - 1;
		sift_up(_N - 1);
	}
	
	void change(size_t k, const T& key) {
		_keys[k] = key;
		sift_up(_qp[k]);	// either sift_up or sift_down
		sift_down(_qp[k]);
	}

	void erase(size_t k) {
		size_t del = _qp.at(k);
		exch(del, --_N);
		sift_up(del);
		sift_down(del);
		// _keys[k] <-- null;  still a placeholder
		_qp[k] = UINT_MAX;
	}

	void pop() { erase(_pq[0]); }

protected:
	size_t _N; 
	std::vector<T> _keys;	 // keys[k] stores the key value of node with index k
	Compare _comp;
	std::vector<size_t> _pq; // pos->index: pq[i] gets the content (index) in pos i 
	std::vector<size_t> _qp; // index->pos: qp[i] locates the pos of node with index i
							 // pq[qp[i]] = qp[pq[i]] = i

	void exch(size_t i, size_t j) {
		size_t pqi = _pq[i];
		_pq[i] = _pq[j]; _qp[_pq[j]] = i;
		_pq[j] = pqi; _qp[pqi] = j;
	}

	bool compare(size_t i, size_t j) {
		return _comp(_keys[_pq[i]], _keys[_pq[j]]);
	}

	void sift_down(size_t pos) {		
		size_t j{};
		while ((j = (pos << 1) + 1) < _N) {
			if (j + 1 < _N && compare(j, j + 1)) ++j;
			if (compare(j, pos)) return;
			exch(pos, j);
			pos = j;
		}
	}

	void sift_up(size_t pos) {
		size_t parent{};
		while (pos > 0 && compare(parent = (pos - 1) >> 1, pos)) {
			exch(parent, pos);
			pos = parent;
		}
	}
};

} // namespace myHeap

#endif // !INDEXPQ_H
