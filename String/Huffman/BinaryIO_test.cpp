#include "BinaryIO.h"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;
using namespace bio;

void write_file(const string& filename)
{
	ofstream ofs(filename, ios::out | ios::binary);
	BinaryOut<ofstream> bout(ofs);
	//BinaryOut<ostream> bout(cout);

	bout.write(65);  // "   A" (3 whitespace + A)
	bout.write('a');
	bout.write("hello ");
	
	bout.write(false);
	bout.write(true);
	bout.write(false);
	bout.write(true);
	bout.write(true);
	bout.write(" from ");

	// 3 bits to complete former remaining bits
	// 111 == '\a', generating a warning sound
	// when writing to cout
	bout.write(true);
	bout.write(true);
	bout.write(true);

	// 'X' : 01011000, padding with 0s
	bout.write(false);
	bout.write(true);
	bout.write(false);
	bout.write(true);
	bout.write(true);

	// total 18 bytes 5 bits
	// write 19 bytes to disk
}

void read_file(const string& filename)
{
	ifstream ifs(filename, ios::in | ios::binary);
	BinaryIn<ifstream> bin(ifs);

	int A = bin.read_int();
	char a = bin.read_char();
	string hello = bin.read_string();
	char ws1 = bin.read_char();
	bool b[5] = { bin.read_bool(), bin.read_bool(),
		bin.read_bool(), bin.read_bool(), bin.read_bool() };
	char ws2 = bin.read_char();
	string from = bin.read_string();
	char ws3 = bin.read_char();
	// discard 3 bits
	bin.read_bool();
	bin.read_bool();
	bin.read_bool();
	char X = bin.read_char();

	cout << A << a << hello << ws1 << b[0] << b[1] << b[2]
		<< b[3] << b[4] << ws2 << from << ws3 << X << '\n';	
}

int main()
{
	ios_base::sync_with_stdio(false);
	try {
		write_file("bio.dat");		
		read_file("bio.dat");
	}
	catch (const exception& e) {
		cout << e.what() << endl;
	}
	catch (...) {
		cout << "unknown error" << endl;
	}

	return 0;
}