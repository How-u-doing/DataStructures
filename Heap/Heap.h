#pragma once
#ifndef HEAP_H
#define HEAP_H
#include <cassert>
#include <cstring>    // std::memcpy
#include <utility>    // std::move
#include <functional> // std::less<T>

namespace myHeap {
// implementation of stl-like func: make_heap, push_heap, etc. 
// note that it's simplified (no error handling)
// see <https://en.cppreference.com/w/cpp/algorithm/push_heap>
// see relative algorithms at CLRS-3e - Heapsort
// see also <https://en.wikipedia.org/wiki/Heapsort>

template <typename T>
inline void swap(T& a, T& b) {
	T tmp{ std::move(a) };
	a = std::move(b);
	b = std::move(tmp);
}

template <typename RandIt, typename Compare>
void sift_down(size_t pos, RandIt A, size_t n, Compare comp) {
	size_t j{};	// largest or smallest between {left, right}
	while ((j = (pos << 1) + 1) < n) {
		if (j + 1 < n && comp(*(A + j), *(A + j + 1))) ++j;
		if (comp(*(A + j), *(A + pos))) return;
		myHeap::swap(*(A + pos), *(A + j));
		pos = j;
	}
}

// Construct a heap in the range [first, last)
template <typename RandIt, typename Compare = std::less<typename std::iterator_traits<RandIt>::value_type>>
void make_heap(RandIt first, RandIt last, Compare comp = Compare{}) {
	//========================================================================================================
	//
	// About the template argument deduction see
	// <https://en.cppreference.com/w/cpp/language/template_argument_deduction> &
	// <https://stackoverflow.com/questions/24277974/couldnt-deduce-template-parameter-from-function-parameters-default-argument>
	// When we want to deduce template arg for default arg,  set the template arg (i.e. type) default 
	// rather than function parameter, as there might be numerious template arg types can attain such
	// goal. e.g. Compare_type_1 = long,   comp = std::less<int>,
	//            Compare_type_2 = string, comp = std::less<int>, 
	//            ...	   (don't forget template specialization)
	// the compiler has so many choices to select, and it's getting confused!
	//
	//========================================================================================================

	auto n{ last - first };	// number of nodes
	auto i{ (n - 1) >> 1 }; // parent node of last leaf
	while (i >= 0) {
		myHeap::sift_down(i--, first, n, comp);
	}
}

template <typename RandIt, typename Compare>
void sift_up(size_t pos, RandIt A, size_t n, Compare comp) {
	size_t parent{};
	while (pos > 0 && comp(*(A + (parent = (pos - 1) >> 1)), *(A + pos))) {
		myHeap::swap(*(A + parent), *(A + pos));
		pos = parent;
	}
}

// Insert the element at the position last-1 into the
// heap defined by the range [first, last-1)
template <typename RandIt, typename Compare = std::less<typename std::iterator_traits<RandIt>::value_type>>
inline void push_heap(RandIt first, RandIt last, Compare comp = Compare{}) {
	myHeap::sift_up(last - first - 1, first, last - first, comp);
}

// Swap the value in the pos first and the value in the pos
// last-1 and make the subrange [first, last-1) into a heap
template <typename RandIt, typename Compare = std::less<typename std::iterator_traits<RandIt>::value_type>>
inline void pop_heap(RandIt first, RandIt last, Compare comp = Compare{}) {
	myHeap::swap(*first, *(last - 1));
	myHeap::sift_down(0, first, last - first - 1, comp);
}

// Convert the heap [first, last) into a sorted range in ascending/descending order
template <typename RandIt, typename Compare = std::less<typename std::iterator_traits<RandIt>::value_type>>
void sort_heap(RandIt first, RandIt last, Compare comp = Compare{}) {
	// move root node (the largest/smallest) to the end
	while (last - first > 1) {
		myHeap::pop_heap(first, last--, comp);
	}
}

template <typename RandIt, typename Compare = std::less<typename std::iterator_traits<RandIt>::value_type>>
void heapsort(RandIt first, RandIt last, Compare comp = Compare{}) {
	// build a heap
	myHeap::make_heap(first, last, comp);

	// move root node (the largest/smallest) to the end
	while (last - first > 1) {
		myHeap::pop_heap(first, last--, comp);
	}
}

template <typename T, typename Compare = std::less<T>>
class Heap {
public:
	// _comp must be initialized in initializer list (i.e. : _comp(comp){})
	// when Compare is a decltype(lambda_function)
	Heap(Compare comp = Compare{}) : _arr(nullptr), _size(0), _capacity(0), _comp(comp) {}

	Heap(size_t capacity, Compare comp = Compare{})
		: _arr(new T[capacity]), _size(0),
		_capacity(capacity), _comp(comp) {}

	Heap(T data[], size_t n, Compare comp = Compare{})
		: _size(n), _capacity(n), _comp(comp)
	{
		_arr = new T[_capacity]; // assert(_arr);
		std::memcpy(_arr, data, _size * sizeof(T));
		myHeap::make_heap(_arr, _arr + _size, _comp);
	}

	Heap(const Heap& heap)
		: _size(heap._size), _capacity(heap._capacity), _comp(heap._comp)
	{
		_arr = new T[_capacity]; // assert(_arr);
		std::memcpy(_arr, heap._arr, _size * sizeof(T));
	}

	Heap& operator=(const Heap& heap) {
		if (&heap == this) return *this;
		Heap tmp{ heap };
		myHeap::swap(tmp, *this);
		return *this;
	}

	~Heap() { clear_heap(); }

	void clear_heap() {
		if (_arr == nullptr) return;
		delete[] _arr;	_arr = nullptr;
		_size = 0;	_capacity = 0;
	}

	bool empty() const noexcept { return _size == 0; }
	size_t size() const noexcept { return _size; }

	const T& top() const { assert(!empty()); return _arr[0]; }

	void push(const T& value) {
		if (_size == _capacity) expand_capacity();
		_arr[_size++] = value;
		myHeap::push_heap(_arr, _arr + _size, _comp);
	}

	void push(T&& value) {
		if (_size == _capacity) expand_capacity();
		_arr[_size++] = std::move(value);
		myHeap::push_heap(_arr, _arr + _size, _comp);
	}

	void pop() {
		assert(!empty());
		myHeap::pop_heap(_arr, _arr + _size, _comp);
		--_size;
	}

protected:
	// we can indeed implement by std::vector<T>  :-)
	T* _arr;
	size_t _size;
	size_t _capacity;

	Compare _comp{};

	void expand_capacity() {
		_capacity = 2 * _capacity + 1;
		T* new_arr = new T[_capacity]{};  // assert(new_arr);
		std::memcpy(new_arr, _arr, _size * sizeof(T));
		delete[] _arr;
		_arr = new_arr;
	}
};

} // namespace myHeap

#endif // !HEAP_H

