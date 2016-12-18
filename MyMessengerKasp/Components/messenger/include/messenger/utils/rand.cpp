#include "rand.h"


namespace utils
{
	namespace rand
	{
		static bool RAND_INITED = false;
		int random()
		{
			if (!RAND_INITED)
			{
				RAND_INITED = true;
				srand(time(NULL));
			}
			return std::rand();
		}

		std::vector<unsigned char> getrandbytes(size_t number)
		{
			std::vector<unsigned char> res;
			/*for (size_t i = 0; i < number; ++i)
				res.push_back(random() % 256);*/
			return res;
		}
	}
}
