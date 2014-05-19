#include <iostream>

#include <millerrabin.hpp>
#include <cstdlib> // rand

MillerRabin::MillerRabin(unsigned long n, unsigned long numberOfBases) :
	BasePrime("Miller-Rabin"),
	mIsValid(false),
	mCheckLimit(n),
	mNumberOfBases(numberOfBases),
	mTypeBitSize(sizeof(unsigned long)*8), // !!!!!!!!
	mNumberOfThreads(getCoreCount()+1),
	mThread(nullptr),
	mPrimeListMutex(),
	mPrimeList()
{
	std::cout << "available cores for concurrent jobs: " << getCoreCount() << std::endl;
	std::cout << "check against " << mNumberOfBases << " bases" << std::endl;
    std::cout << "ulong size: " << mTypeBitSize << " bits" << std::endl << std::endl;

	// TODO: check a condition for doing concurrent jobs?
	mThread = new std::thread*[mNumberOfThreads];

	// nothing to do yet?
	if (n > 2 && numberOfBases > 2)
	{
		mIsValid = true; // object created correctly
	}
}

MillerRabin::~MillerRabin()
{
	if (mThread)
	{
		for (size_t i = 0; i < mNumberOfThreads; i++)
		{
			// TODO
			if (mThread[i])
			{
				mThread[i]->join();
				delete mThread[i];
			}
		}
		delete[] mThread;
		mThread = nullptr;
	}
}

unsigned long MillerRabin::expModulo(const unsigned long base, const unsigned long power, const unsigned long modulus) const
{
	unsigned long result = 1; // take care of the bitsize!!

	for (int i=(mTypeBitSize-1); i>=0; i--)
	{
		result = (result*result) % modulus;
		if (power & (1 << i))
		{
			result = (result*base) % modulus;
		}
	}

	return result;
}

bool MillerRabin::check(const unsigned long base, const unsigned long pprime) const
{
	unsigned long a = base, s = 0, d = pprime - 1;
	while ((d % 2) == 0)
	{
		d /= 2;
		s++;
	}

	// fermat
	a = expModulo(base, d, pprime);
	if ((a == 1) || (a == pprime - 1))
	{
		return true;
	}

    for(unsigned long r=1; r < s; r++)
    {
		a = ((a * a) % pprime);
		if (a == 1)
		{
			return false;
		}
		else if (a == pprime-1)
		{
			return true;
		}
	}

	return false;
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
	std::cout << "MillerRabin took " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms." << std::endl;
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
                mPrimeListMutex.lock();
				mPrimeList.push_back(n);
                mPrimeListMutex.unlock();
			}
		}
	}
	end = std::chrono::steady_clock::now();
}
