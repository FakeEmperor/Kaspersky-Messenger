#pragma once
#include <vector>
#include <random>
#include <ctime>

namespace utils 
{
	namespace rand
	{
		int random();
		std::vector<unsigned char> getrandbytes(size_t number);
	}
}
