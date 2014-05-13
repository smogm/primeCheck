#include <iostream>
#include <cstring> // strlen
#include <cstdlib> // strtoul
#include <climits> // ULONG_MAX

#include <basePrime.hpp>
#include <millerrabin.hpp>

static void printUsage(const char* const binname)
{
	std::cout << "usage:" << std::endl;
	std::cout << binname << " [OPTIONS] UPPPER_LIMIT" << std::endl;
	std::cout << std::endl;
	std::cout << "OPTIONS are:" << std::endl;
	std::cout << "   -p\tPrint generated prime numbers (time consuming!)" << std::endl;
}

int main(int argc, char** argv)
{
	bool printLong = false;
	int i=1;
	if (argc < 2)
	{
		std::cerr << "missing arguments!" << std::endl;
		printUsage(argv[0]);
		return 1;
	}

	for (; i < argc; i++)
	{
		if ((argv[i][0] == '-') && (strlen(argv[i]) > 1))
		{
			switch (argv[i][1])
			{
				case 'p':
					std::cout << "[enabled]: printing found prime numers" << std::endl;
					printLong = true;
				break;
			
				default:
					std::cerr << "invalid argument " << argv[i] << std::endl;
					printUsage(argv[0]);
				break;
			}
		}
	}

	// get the upper limit for prime checking:
	unsigned long n = strtoul(argv[argc-1], nullptr, 10);
	if (n == ULONG_MAX)
	{
		std::cerr << strerror(errno) << std::endl;
		return 1;
	}

	// example use case:
	MillerRabin mr(n);
	std::cout << "calculating MillerRabin, stand by..." << std::endl;
	mr.calcPrimes();
	if (printLong)
	{
		mr.printPrimes();
	}
	mr.printCount();
	mr.printTime();

	return 0;
}
