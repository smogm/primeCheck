#include <iostream>
#include <future>

#include <millerRabin.hpp>
//#include <millerRabinParallelBase.hpp>

#include <cstdlib> // rand

MillerRabin::MillerRabin(unsigned long n, unsigned long numberOfBases) :
	BasePrime("MillerRabin"),
	mIsValid(false),
	mCheckLimit(n),
	mNumberOfBases(numberOfBases),
	mTypeBitSize(sizeof(unsigned long)*8), // !!!!!!!!
	mNumberOfThreads(getCoreCount()+1),
	mPrimeList()
{
	std::cout << "check against " << mNumberOfBases << " bases" << std::endl;

	// nothing to do yet?
	if (n > 2 && numberOfBases > 2)
	{
		mIsValid = true; // object created correctly
	}
}

MillerRabin::~MillerRabin()
{

}

MillerRabin::operator bool() const
{
	return mIsValid;
}

void MillerRabin::printPrimes() const
{
	for (auto prime : mPrimeList)
	{
		std::cout << prime << "\tis probably prime" << std::endl;
	}

	std::cout << std::endl;
}

void MillerRabin::printCount() const
{
	std::cout << "found " << mPrimeList.size() << " probably prime numbers." << std::endl;
}

void MillerRabin::printTime() const
{
	std::cout << *this << " took " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms." << std::endl;
}

void MillerRabin::calcPrimes()
{
	mPrimeList.clear();

	// TODO: check whether mCheckLimit is greater than this:
	mPrimeList.push_back(2);
	mPrimeList.push_back(3);
	mPrimeList.push_back(5);
	mPrimeList.push_back(7);
	mPrimeList.push_back(11);
	mPrimeList.push_back(13);
	mPrimeList.push_back(17);
	mPrimeList.push_back(19);
	mPrimeList.push_back(23);
	mPrimeList.push_back(29); // <--

	start = std::chrono::steady_clock::now();

	// iterater all numbers we want to check
#if defined(USE_OPENMP)
#pragma omp parallel for
#endif
    for (unsigned long n = mPrimeList[mPrimeList.size()-1]; n <= mCheckLimit; n+=2)
	{
        // inner declaration makes it private for each thread
        bool isPrime = true;

        if ((n % 3  != 0) && (n % 5  != 0) && (n % 7  != 0) && (n % 11 != 0) &&
            (n % 13 != 0) && (n % 17 != 0) && (n % 19 != 0) && (n % 23 != 0) && (n %29 != 0))
		{

			// iterate the bases
            #if defined(USE_OPENMP)
            #pragma omp parallel for
            #endif
            for (unsigned long i = 0; i < mNumberOfBases; i++)
			{
                unsigned long base = 0;
                bool running = true;
                bool isPrimeToBase;

                #if defined(USE_OPENMP)
                #pragma omp critical(isPrimeVariable)
                #endif
                {
                    // we use isPrime as indicator if we should go ahead with our base checks
                    // if one thread found that n is not prime, running will be false
                    // so all other threads would simply go through the remaining loop iterations without doing any work
                    running = isPrime;
                }

                if (running)
                {
                    srand(i);
                    base = (rand() % (n - 1)) + 1;

                    isPrimeToBase = check(base, n);

                    if (isPrimeToBase == false)
                    {
                        #if defined(USE_OPENMP)
                        #pragma omp critical(isPrimeVariable)
                        #endif
                        {
                            #if defined(DEBUG)
                            std::cout << n << " is not prime to base " << base << " so, we're setting isPrime = false / base iteration i was: " << i << std::endl;
                            #endif

                            isPrime = false;
                        }

                    }
                }
                else
                {
                    #if defined(DEBUG)
                    std::cout << n << " was already found not prime, so we're don't do any check in base iteration " << i << std::endl;
                    #endif
                }

            }

			if (isPrime) // n passed the check and seems to be prime
			{
				// lock for thread safety
				mPrimeList.push_back(n);
			}
		}
	}
	end = std::chrono::steady_clock::now();
}
