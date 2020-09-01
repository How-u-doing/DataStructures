#include "MST.h"
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

using namespace std;
using namespace myGraph;

char sp; // separator

class City {
	// separated by '/'
	friend ifstream& operator>>(ifstream& ifs, City& c);
	friend ostream& operator<<(ostream& os, const City& c) { os << c._name; return os; }
	friend bool operator==(const City& lhs, const City& rhs) { return lhs._name == rhs._name; }
	friend bool operator>(const City& lhs, const City& rhs) { return lhs._name > rhs._name; }
public:
	City(const string& name = "") :_name(name) {}
	string& name() { return _name; }
	const string& name() const { return _name; }
private:
	string _name;
};

ifstream& operator>>(ifstream& ifs, City& c) {
	c.name() = "";
	char ch;
	ifs >> ch;
	ifs >> std::noskipws;
	while (ch != sp && !ifs.eof()) {
		c.name() += ch;
		ifs >> ch;
	}
	ifs >> std::skipws;
	return ifs;
}

template<typename T>
void print_MST(Graph<T>& G, std::vector<MST_Edge>& mst) {
	for (size_t i = 0; i < mst.size(); ++i) {
		cout << G.vertex(mst[i]._source) << " - "
			<< G.vertex(mst[i]._dest) << '\t'
			<< mst[i]._cost << '\n';
	}
}

namespace std {
	template<> struct hash<City> {
		std::size_t operator()(const City& c) const {
			return std::hash<std::string>{}(c.name());
		}
	};
}

/**
 * CLI examples: ./mst "LazyPrim" city_networks.txt "/"
 *               ./mst "Prim" city_networks.txt "/"
 *               ./mst "Kruskal" city_networks.txt "/"
 */
int main(int argc, const char* argv[])
{
	if (argc < 4) { cerr << "Too few arguments" << endl; return 1; }
	string method{ argv[1] };	
	string filename{ argv[2] };
	sp = argv[3][0];
	try {
		Graph<City> G{ filename };
		vector<MST_Edge> mst;
		double count{};
		if (method == "LazyPrim")	count = LazyPrimMST(G, mst);
		else if (method == "Prim")	count = PrimMST(G, mst);
		else if (method == "Kruskal")  count = KruskalMST(G, mst);
		else throw invalid_argument("unknown MST method");
		cout << count << '\n';
		print_MST(G, mst);
	}
	catch (const exception& e) {
		cout << e.what() << endl;
	}
	catch (...) {
		cout << "Some unknown error happened" << endl;
	}
	return 0;
}