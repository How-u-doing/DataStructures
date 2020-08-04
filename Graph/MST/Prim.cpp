#include "PrimMST.h"
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

// Linux CLI example: ./prim city_networks.txt "/"
int main(int argc, const char* argv[])
{
	using namespace myGraph;
	if (argc < 3) { cerr << "Too few arguments" << endl; return 1; }
	string filename{ argv[1] };
	sp = argv[2][0];
	try {
		Graph<City> G{ filename };
		auto mst{ PrimMST(G) };
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