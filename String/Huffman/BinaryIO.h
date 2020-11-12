/**
 *  C++ versions of BinaryIn and BinaryOut from 'algs4'
 *  see also:
 *  https://algs4.cs.princeton.edu/code/edu/princeton/cs/algs4/BinaryIn.java.html and
 *  https://algs4.cs.princeton.edu/code/edu/princeton/cs/algs4/BinaryOut.java.html
 */
#ifndef BINARYIO_H
#define BINARYIO_H
#include <bitset>
#include <string>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <cassert>

namespace bio {

typedef std::bitset<8> byte;

template<typename InStream = std::ifstream>
class BinaryIn {
	InStream& is;		  // or we can use std::istream and dynamically dispatch is.read()
	unsigned char buffer; // 8-bit buffer of bits to read
	int n;				  // # of bits left in buffer

	void fill_buffer() {
		if (eof()) throw std::runtime_error("EOF: no character to read");
		is.read((char*)&buffer, 1);
		n = 8;
	}

public:
	BinaryIn(InStream& in) : is(in), 
		buffer(0), n(0) { fill_buffer(); }

	bool eof() const {
		return is.eof();
	}

	bool read_bool() {
		--n;
		bool bit = ((buffer >> n) & 1) == 1;
		if (n == 0) fill_buffer();
		return bit;
	}

	unsigned char read_char() {
		// special case when aligned byte
		if (n == 8) {
			unsigned char x = buffer;
			fill_buffer();
			return x;
		}

		// otherwise combine last n bits of current
		// buffer with first 8-n bits of new buffer
		unsigned char x = buffer;
		x <<= (8 - n);
		int oldN = n;
		fill_buffer();
		n = oldN;
		x |= (buffer >> n); // add 8-n bits of new buffer
		return x;
	}

	// read the next r (0<= r <=8) bits from input
	// stream and return as an r-bit character
	unsigned char read_char(int r) {
		if (r < 0 || r > 8) throw std::invalid_argument("invalid argument r = " + std::to_string(r));
		
		// optimize r = 8 case
		if (r == 8) return read_char();

		unsigned char x = 0;
		for (int i = 0; i < r; ++i) {
			x <<= 1;
			bool bit = read_bool();
			if (bit) x |= 1;
		}
		return x;
	}

	// read the next 32 bits from input stream and return as a 32-bit int
	int read_int() {
		int x = 0;
		for (int i = 0; i < 4; ++i) {
			unsigned char c = read_char(); // DON'T write as CHAR, as if top bit's
			x <<= 8;					   // present, c would be a negative number
			x |= c;
		}
		return x;
	}

	std::string read_string() {
		std::string s;
		unsigned char x;
		while (!eof())
			if((x = read_char()) == ' '); // skip whitespace
			else { s.push_back(x); break; }
		while (!eof() && (x = read_char()) != ' ') s.push_back(x);

		if (!eof()) {
			is.seekg(-2, std::ios_base::cur); // move 2 positions back since we've read in the next
			unsigned char c;				  // buffer when reading the extra stopping whitespace
			is.read((char*)&c, 1); // after reading the char, the reading position is on the right track
			buffer = c & ((1 << n) - 1);	  // only use last n bits
		}
		return s;
	}
};

template<typename OutStream = std::ofstream>
class BinaryOut {
	OutStream& os;		  // or we can use std::ostream and dynamically dispatch os.write()
	unsigned char buffer; // 8-bit buffer of bits to write
	int n;				  // # of bits remaining in buffer

	// write the specified bit to output stream
	void write_bit(bool bit) {
		// add bit to buffer
		buffer <<= 1;
		if (bit) buffer |= 1;

		// if buffer is full (8 bits), write out as a single byte
		++n;
		if (n == 8) clear_buffer();
	}

    // write the 8-bit byte to output stream
    void write_byte(int x) {
        assert(x >= 0 && x < 256);

        // optimized if byte-aligned
        if (n == 0) {
			os.write((char*)&x, 1);
            return;
        }

        // otherwise write one bit at a time
        for (int i = 0; i < 8; ++i) {
			bool bit = ((x >> (8 - i - 1)) & 1) == 1;
			write_bit(bit);
        }
    }

    // write out any remaining bits in buffer to standard output, padding with 0s
    void clear_buffer() {
        if (n == 0) return;
        if (n > 0) buffer <<= (8 - n);
		os.write((char*)&buffer, 1);
		n = 0;
        buffer = 0;
    }

public:
	BinaryOut(OutStream& out)
		: os(out), buffer(0), n(0) {}

	~BinaryOut() { flush(); }

	// write the specified bit to output stream
	void write(bool x) {
		write_bit(x);
	}

	// write the 8-bit char to output stream
	void write(char x) {
		write_byte(x);
	}
	
	// write the 8-bit unsigned char to output stream
	void write(unsigned char x) {
		write_byte(x);
	}

	// write the 8-bit byte to output stream
	void write(byte x) {
		write_byte(x.to_ulong() & 0xff);
	}

	// write the 32-bit int to output stream
	void write(int x) {
		write_byte((x >> 24) & 0xff);
		write_byte((x >> 16) & 0xff);
		write_byte((x >>  8) & 0xff);
		write_byte((x >>  0) & 0xff);
	}
	
	// write the 32-bit unsigned int to output stream
	void write(size_t x) {
		write(static_cast<int>(x));
	}

	// write the r-bit int to output stream
	void write(int x, int r) {
		if (r == 32) {
			write(x);
			return;
		}
		if (r < 1 || r > 32)		throw std::invalid_argument("invalid argument r = " + std::to_string(r));
		if (x < 0 || x >= (1 << r)) throw std::invalid_argument("invalid " + std::to_string(r) + "-bit char = " + std::to_string(x));
		for (int i = 0; i < r; ++i) {
			bool bit = ((x >> (r - i - 1)) & 1) == 1;
			write_bit(bit);
		}
	}

	// write the string of 8-bit characters to output stream
	void write(const std::string& s) {
		for (char c : s) write(c);
	}

	// write the c string of 8-bit characters to output stream
	void write(const char* s) {
		while (*s) write(*s++);
	}

	void flush() {
		clear_buffer();
		os.flush();
	}
};

} // namespace bio

#endif // !BINARYIO_H

