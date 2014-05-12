#include <iostream>

#include <basePrime.hpp>
#include <millerrabin.hpp>

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		std::cerr << "missing arguments!" << std::endl;
		return 1;
	}

	unsigned long n = atol(argv[1]);

	// example use case:
	BasePrime* primeCheck = new MillerRabin(n);
	if (*primeCheck)
	{
		std::cout << "is prime" << std::endl;
		return 0;
	}

	delete primeCheck;

	std::cout << "is NOT prime!" << std::endl;
	return 1;
}
