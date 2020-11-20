// write 1M randomly generated integers to file
#include <random>
#include <fstream>

int main()
{
	const int N = (int)1e6;
	static int data[N];

	std::random_device rd;  // will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd()); // standard mersenne_twister_engine seeded with rd()
	std::uniform_int_distribution<std::mt19937::result_type> dis(1, N); // distribution in range [1, 1e6]

	for (int i = 0; i < N; ++i) {
		data[i] = dis(gen);
	}

	std::ofstream ofs{ "1M.dat", std::ios::out | std::ios::binary };
	ofs.write((char*)data, sizeof(data));

	return 0;
}