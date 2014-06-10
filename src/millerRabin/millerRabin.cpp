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
    mNumberOfPrimes(1),     // test starts with 3, so count the prime 2 here
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

    int bases[6] = { 1, 2, 3, 5, 7, 11 };

	// iterater all numbers we want to check
#if defined(USE_OPENMP)
#pragma omp parallel for
#endif
    for (unsigned long n = 3; n <= mCheckLimit; n+=2)
	{
        bool isPrime = true;

        for (unsigned long i = mNumberOfBases; i > 0; i--)
        {
            unsigned long base = 0;

            if (isPrime)
            {
                //srand(i);
                //base = (rand() % (n - 1)) + 1;

                base = bases[i];

                isPrime =  check(base, n);

                if (isPrime == false)
                    break;
            }
        }

        if (isPrime) // n passed the check and seems to be prime
        {
			mNumberOfPrimes++;
        }

	}
	end = std::chrono::steady_clock::now();
}
