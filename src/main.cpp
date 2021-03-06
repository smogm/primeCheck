#include <iostream>
#include <cstring> // strlen
#include <cstdlib> // strtoul
#include <climits> // ULONG_MAX
#include <cerrno>

#include <basePrime.hpp>
#include <millerRabin.hpp>
#include <millerRabinParallelPrimes.hpp>
#include <millerRabinParallelBases.hpp>

#include <atkin.hpp>
#include <atkinthreaded.hpp>
#include <atkinthreadedvar2.hpp>
#include <atkinthreadedatomic.hpp>

static void printUsage(const char* const binname)
{
	std::cout << "usage:" << std::endl;
	std::cout << binname << " [OPTIONS] UPPPER_LIMIT" << std::endl;
	std::cout << std::endl;
	std::cout << "OPTIONS are:" << std::endl;
	std::cout << "   -p\tPrint generated prime numbers (time consuming!)" << std::endl;
}

static void runAlgorithm(BasePrime& algorithm, bool printLong)
{
	std::cout << "calculating " << algorithm << ", stand by..." << std::endl;
	algorithm.calcPrimes();
	if (printLong)
	{
		algorithm.printPrimes();
	}
	algorithm.printCount();
	algorithm.printTime();
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

	// run all Algorithms
    MillerRabin millerRabinNormal(n, 5);
    if (millerRabinNormal)
    {
		runAlgorithm(millerRabinNormal, printLong);
	}
	std::cout << "===========================" << std::endl;
	std::cout << "===========================" << std::endl;

    MillerRabinParallelPrimes millerRabinParallelPrimes(n, 5);
    if (millerRabinParallelPrimes)
    {
		runAlgorithm(millerRabinParallelPrimes, printLong);
	}
	std::cout << "===========================" << std::endl;
	std::cout << "===========================" << std::endl;


    MillerRabinParallelBases millerRabinParallelBases(n, 5);
    if (millerRabinParallelBases)
    {
		runAlgorithm(millerRabinParallelBases, printLong);
	}
	std::cout << "===========================" << std::endl;
	std::cout << "===========================" << std::endl;

	Atkin atkin(n);
	runAlgorithm(atkin, printLong);

	std::cout << "===========================" << std::endl;
	std::cout << "===========================" << std::endl;

	AtkinThreaded atkinThreaded(n);
	runAlgorithm(atkinThreaded, printLong);

	std::cout << "===========================" << std::endl;
	std::cout << "===========================" << std::endl;

	AtkinThreadedVar2 atkinThreadedvar2(n);
	runAlgorithm(atkinThreadedvar2, printLong);
	 
	std::cout << "===========================" << std::endl;
	std::cout << "===========================" << std::endl;

	AtkinThreadedAtomic atkinThreadedatomic(n);
	runAlgorithm(atkinThreadedatomic, printLong);
	 
	return 0;
}
