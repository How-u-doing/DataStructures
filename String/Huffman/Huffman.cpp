#include "Heap.h"
#include "BinaryIO.h"
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

const size_t R = 256; // extended ASCII

// Huffman tree node
struct Node {
	unsigned char ch;
	int freq;
	Node* left, * right;
	Node(unsigned char ch = '\0', int freq = -1, Node* left = nullptr, Node* right = nullptr)
		: ch(ch), freq(freq), left(left), right(right) {}
};

// precondition: x != nullptr
bool is_leaf(Node* x) { return x->left == nullptr && x->right == nullptr; }

Node* build_trie(int freq[]) {
	auto greater = [](const Node* lhs, const Node* rhs) { return lhs->freq > rhs->freq; };
	myHeap::Heap<Node*, decltype(greater)> pq(R, greater); // min heap
	for (size_t c = 0; c < R; ++c) {
		if (freq[c] > 0)
			pq.push(new Node((char)c, freq[c]));
	}
	Node* left, * right, * parent;
	while (pq.size() > 1) {
		left  = pq.top(); pq.pop();
		right = pq.top(); pq.pop();
		parent = new Node('\0', left->freq + right->freq, left, right);
		pq.push(parent);
	}
	return pq.top();
}

// precondition: x != nullptr
void build_code(Node* x, std::string code[], const std::string& curr) {
	if (is_leaf(x)) { // require at leat 2 bytes to build code
		code[x->ch] = curr;
	}
	else {
		build_code(x->left , code, curr + '0');
		build_code(x->right, code, curr + '1');
	}
}

// write header for decoder
// preorder traverse the trie, and mark if it's a leaf node
void write_trie(Node* x, bio::BinaryOut<std::ofstream>& bout) {
	if (is_leaf(x)) {
		bout.write(true);
		bout.write(x->ch);
	}
	else {
		bout.write(false);
		write_trie(x->left , bout);
		write_trie(x->right, bout);
	}
}

// read decoding header
Node* read_trie(bio::BinaryIn<std::ifstream>& bin) {
	bool leaf = bin.read_bool();
	if (leaf) {
		return new Node(bin.read_char(), -1);
	}
	else {
		// since the order of evaluation is unspecified (see:
		// https://en.cppreference.com/w/cpp/language/eval_order),
		// we'd better not use the following approach:
		// return new Node('\0', -1, read_trie(bin), read_trie(bin));
		Node* left_subtrie  = read_trie(bin);
		Node* right_subtrie = read_trie(bin);
		return new Node('\0', -1, left_subtrie, right_subtrie);
	}
}

void clear_trie(Node* x) {
	if (x != nullptr) {
		clear_trie(x->left);
		clear_trie(x->right);
		delete x;
	}
}

void compress(const std::string& src_file, const std::string& dest_file) {
	std::ifstream ifs{ src_file, std::ios::in | std::ios::ate | std::ios::binary };
	if (!ifs.is_open()) {
		throw std::runtime_error("Error opening file \"" + src_file + "\" for compression");
	}

	/* one possible approach to read all contents: */
	// std::stringstream strs;
	// strs << ifs.rdbuf();
	// std::string buffer = strs.str();
	
	// another efficient way
	size_t sz = ifs.tellg();
	if (sz == 0) throw std::runtime_error("File \"" + src_file + "\" is null");
	if (sz == 1) throw std::runtime_error("Compression requires at least 2 bytes, while \"" + src_file + "\" has only 1 byte");
	std::string buffer(sz, '\0');
	ifs.seekg(0);
	ifs.read(&buffer[0], sz);

	int freq[R]{};	
	for (unsigned char ch : buffer)	++freq[ch];

	Node* root = build_trie(freq);

	std::string code[R];
	build_code(root, code, "");

	std::ofstream ofs{ dest_file, std::ios::out | std::ios::binary };
	bio::BinaryOut<std::ofstream> bout(ofs);

	// write header for decoding
	write_trie(root, bout);

	// write no. of bytes of original data
	// If we don't do this, handling the padding 0s in
	// decoding phase could be annoying & inefficient.
	bout.write(sz);

	// encoding
	std::string word;
	for (unsigned char ch : buffer) {
		word = code[ch];
		for (char c : word) { // write bitwise 0s and 1s
			if (c == '0') bout.write(false);
			else		  bout.write(true);
		}
	}
	
	clear_trie(root);
}

void decompress(const std::string& src_file, const std::string& dest_file) {
	std::ifstream ifs{ src_file, std::ios::in | std::ios::binary };
	if (!ifs.is_open()) {
		throw std::runtime_error("Error opening file \"" + src_file + "\" for decompression");
	}
	bio::BinaryIn<std::ifstream> bin(ifs);

	// read decoing header, rebuild trie
	Node* root = read_trie(bin);

	// read no. of bytes to write
	size_t length = bin.read_int();

	std::string buffer(length, '\0'); // write buffer to hold all contents

	for (size_t i = 0; i < length; ++i) {
		Node* x = root;
		while (!is_leaf(x)) {
			bool bit = bin.read_bool();
			if (bit) x = x->right;
			else	 x = x->left;
		}
		buffer[i] = x->ch;
	}

	// dump all contents to file
	std::ofstream ofs{ dest_file, std::ios::out | std::ios::binary };
	ofs.write(&buffer[0], length);

	clear_trie(root);
}

void print_help() {
	std::string exe = "./Huffman";
#if defined (_WIN32) || defined (_WIN64)	
	exe = "Huffman.exe";
#endif
	std::cout << "Usage:\n";
	std::cout << "compress: " << exe << " -z filename.xxx  [-o filename.hzip]\n";
	std::cout << "extract:  " << exe << " -x filename.hzip  -o filename.xxx" << std::endl;
}

// return the stem (without filename extension) of the filename
std::string parse_stem(const std::string& filename) {
	int i = filename.length() - 1;
	for (; i >= 0; --i) {
		if (filename[i] == '.') break;
	}
	if (i == 0) { // e.g. ".bashrc", filename stem is itself
		return filename;
	}
	return filename.substr(0, i);
}

int main(int argc, const char* argv[])
{
	using namespace std;
	ios_base::sync_with_stdio(false);
	if (argc < 3) { cout << "Too few arguments\n"; print_help(); return 1; }
	string src_file{ argv[2] };
	
	try {
		string option = argv[1];
		if (option == "-z") {
			if (argc == 3)	compress(src_file, parse_stem(argv[2]) + ".hzip");
			else if (argc == 4) { cout << "Lack of output file\n"; print_help(); return 1; }
			else {
				if (string(argv[3]) != "-o") { cout << "Syntax error\n"; print_help(); return 1; }
				compress(src_file, argv[4]);
			}
		}
		else if (option == "-x") {
			if (argc == 3 || argc == 4) { cout << "Lack of output file\n"; print_help(); return 1; }
			if (string(argv[3]) != "-o") { cout << "Syntax error\n"; print_help(); return 1; }
			decompress(src_file, argv[4]);
		}
		else { cout << "Invalid option = \"" << option << "\"\n"; print_help(); return 1; }
	}
	catch (std::exception& e) {
		cout << e.what() << endl;
	}
	catch (...) {
		cout << "unknown error" << endl;
	}
	
	return 0;
}