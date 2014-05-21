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
    mNumberOfPrimes(0),
    mNumberOfPrimesMutex()
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

}

void MillerRabin::printCount() const
{
    std::cout << "found " << mNumberOfPrimes << " probably prime numbers." << std::endl;
}

void MillerRabin::printTime() const
{
	std::cout << *this << " took " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms." << std::endl;
}

void MillerRabin::calcPrimes()
{
	start = std::chrono::steady_clock::now();

	// iterater all numbers we want to check
#if defined(USE_OPENMP)
#pragma omp parallel for
#endif
    for (unsigned long n = 3; n <= mCheckLimit; n+=2)
	{
        // inner declaration makes it private for each thread
        bool isPrime = true;

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
                //srand(i);
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
            mNumberOfPrimesMutex.lock();
                mNumberOfPrimes++;
            mNumberOfPrimesMutex.unlock();
        }

	}
	end = std::chrono::steady_clock::now();
}
