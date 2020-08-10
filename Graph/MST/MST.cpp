#include "MST.h"
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

using namespace std;

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
void print_MST(const T& mst) {
	for (size_t i = 0; i < mst.size(); ++i) {
		cout << std::get<0>(mst[i]) << " - "
			<< std::get<1>(mst[i]) << '\t' <<
			std::get<2>(mst[i]) << '\n';
	}
}

namespace std {
	template<> struct hash<City> {
		std::size_t operator()(const City& c) const {
			return std::hash<size_t>{}(c.name().length());
		}
	};
}

// Linux CLI example: ./mst "Prim" city_networks.txt "/"
// Linux CLI example: ./mst "Kruskal" city_networks.txt "/"
int main(int argc, const char* argv[])
{
	using namespace myGraph;
	if (argc < 4) { cerr << "Too few arguments" << endl; return 1; }
	string method{ argv[1] };
	if (method != "Prim" && method != "Kruskal") {
		cerr << "Unknown MST method" << endl; return 2;
	}
	string filename{ argv[2] };
	sp = argv[3][0];
	try {
		Graph<City> G{ filename };
		vector<std::tuple<City, City, double>> mst;
		if (method == "Prim")	mst = PrimMST(G);
		else mst = KruskalMST(G);
		print_MST(mst);
	}
	catch (const exception& e) {
		cout << e.what() << endl;
	}
	catch (...) {
		cout << "Some unknown error happened" << endl;
	}
	return 0;
}