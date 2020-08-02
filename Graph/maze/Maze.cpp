#include "../Graph.h"
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

// according to matrix coordinate
class Point {
	friend std::ostream& operator<<(std::ostream& os, const Point& p) {
		os << '(' << p._m << ',' << p._n << ')';
		return os;
	}
	friend bool operator==(const Point& lhs, const Point& rhs) {
		return lhs._m == rhs._m && lhs._n == rhs._n;
	}	
public:
	Point(int m, int n) : _m(m), _n(n) {}
	int m() const { return _m; }
	int n() const { return _n; }
	
private:
	int _m, _n;
};

// see <https://en.cppreference.com/w/cpp/container/unordered_map/unordered_map>
namespace std {
	template<> struct hash<Point> {
		std::size_t operator()(const Point& p) const {
			return std::hash<size_t>{}(p.m() + p.n());
		}
	};
}

class Maze {
public:
	Maze(const std::string& filename) {
		std::ifstream ifs{ filename };
		if (!ifs.is_open())
			throw std::runtime_error("Error opening file \"" + filename + "\"");
		int x;
		ifs >> _m >> _n;
		for (int i = 0; i < _m; ++i)
			for (int j = 0; j < _n; ++j) {
				ifs >> x;
				if (x == 0)	G.add_vertex(Point(i, j));
			}
		// add edges
		auto curr{ G.begin() }, next{ curr };
		for (int i = 0; i < _m; ++i)
			for (int j = 0; j < _n; ++j) {
				curr = G.find_vertex(Point(i, j));
				if (curr == G.end()) continue;
				else {// Point(i,j)==0
					if (j < _n - 1) {// actually we don't need this if, but with one more find operation
						next = G.find_vertex(Point(i, j + 1));
						if (next != G.end()) G.add_edge(curr, next);
					}
					if (i < _m - 1) {
						next = G.find_vertex(Point(i + 1, j));
						if (next != G.end()) G.add_edge(curr, next);
					}
				}
			}
	}

	std::vector<Point> shortest_path(const Point& start, const Point& end) {
		std::vector<Point> path{};
		G.path(G.find_vertex(start), G.find_vertex(end), path, "BFS");
		return path;
	}

private:
	int _m, _n; // m x n matrix
	myGraph::Graph<Point> G;
};


// Linux CLI example: ./maze maze7x6.txt \(6,0\) \(0,5\)
int main(int argc, const char* argv[]) {
	using namespace std;
	if (argc < 4) { cerr << "Too few arguments" << endl; return 1; }
	string filename{ argv[1] };
	string str{ string(argv[2]) + string(argv[3]) };
	istringstream ss{ str };
	int sm, sn, em, en;
	char ch;
	//    (      6     ,     0     )    (      0     ,     5     )
	ss >> ch >> sm >> ch >> sn >> ch >> ch >> em >> ch >> en >> ch;

	Point start(sm, sn), end(em, en);

	try {
		Maze maze(filename);

		vector<Point> path{ maze.shortest_path(start,end) };
		
		for (const auto& p : path)
			if (p == start) cout << p;
			else cout << "-->" << p;

		cout << "\n";
	}
	catch (const std::exception& e) {
		cout << e.what() << endl;
	}
	catch (...) {
		cout << "Some unknown error happened" << endl;
	}

	return 0;
}