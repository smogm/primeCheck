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

	// get the upper limit for prime checking:
	unsigned long n = atol(argv[1]);

	// example use case:
	MillerRabin mr(n);
	mr.calcPrimes();
	mr.printPrimes();

	return 0;
}
