#include <iostream>
#include <future>

#include <millerRabinParallelPrimes.hpp>

MillerRabinParallelPrimes::MillerRabinParallelPrimes(unsigned long n, unsigned long numberOfBases) :
	BasePrime("MillerRabinParallelPrimes"),
	mIsValid(false),
	mCheckLimit(n),
	mNumberOfBases(numberOfBases),
	mNumberOfThreads(getCoreCount()+1),
	mBase(new unsigned long[mNumberOfThreads]),
	mNumberOfPrimes(0)
{
	std::cout << "number of concurrent jobs: " << mNumberOfThreads << std::endl;
	std::cout << "checking against " << mNumberOfBases << " bases" << std::endl;

	// nothing to do yet?
	if (n > 2 && numberOfBases > 2)
	{
		mIsValid = true; // object created correctly
	}
}

MillerRabinParallelPrimes::~MillerRabinParallelPrimes()
{
	delete[] mBase;
}

size_t MillerRabinParallelPrimes::threadFunction(const unsigned long mStart, const unsigned long mStop)
{
	char output[500];
	size_t dummy;
	bool isPrime = false;
	size_t i = 0;
	size_t numberOfPrimes = 1;

#if defined(DEBUG)
	sprintf(output, "mStart: %lu\n", mStart);
	dummy = write(STDOUT_FILENO, output, strlen(output));
	sprintf(output, "mStop: %lu\n", mStop);
	dummy = write(STDOUT_FILENO, output, strlen(output));
#endif

	unsigned long n = mStart;
	// be sure we only check odd numbers
	if (n == 0)
	{
		n = 3;
	}
	else if (n % 2 == 0)
	{
		n++;
	}

	for (; n <= mStop; n+=2)
	{
		isPrime = false;
		i = 0;

		// iterate the bases
		do
		{
			isPrime = check(mBase[i]+mStart, n);
			i++;
		} while (i < mNumberOfBases && isPrime);

		if (isPrime) // n passed the check and seems to be prime
		{
			numberOfPrimes++;
		}
	}

#if defined(DEBUG)
	sprintf(output, "thread terminating normaly, found %lu primes in the range: %lu - %lu\n", numberOfPrimes, mStart, mStop);
	dummy = write(STDOUT_FILENO, output, strlen(output));
#endif
	(void)dummy;
	(void)output;
	return numberOfPrimes;
}

void MillerRabinParallelPrimes::calcPrimes()
{
	if (mNumberOfThreads > 1)
	{
		std::future<size_t> f[mNumberOfThreads];
		size_t blockSize = (mCheckLimit / mNumberOfThreads);
		size_t remaining = (mCheckLimit - (blockSize * mNumberOfThreads));

		unsigned long low = 0, high = blockSize;
		size_t t = 0;

		// prepare bases:
		for (size_t i = 0; i < mNumberOfBases; i++)
		{
			srand(i);
			mBase[i] = (rand() % (blockSize - 1)) + 1;
			std::cout << "base: " << mBase[i] << std::endl;
		}

		start = std::chrono::steady_clock::now();
		do
		{
			f[t] = std::async(std::launch::async, &MillerRabinParallelPrimes::threadFunction, this, low, high);

			t++;
			// prepare for next loop:
			low = high + 1;
			high = low + blockSize -1;

			if (t == (mNumberOfThreads - 1)) // is this the next thread the last thread?
			{
				// if it's the last, give it the remaining numbers
				high += remaining;
			}
		} while(t < mNumberOfThreads);

		std::cout << "waiting for threads..." << std::endl;
		for (size_t i = 0; i < mNumberOfThreads; i++)
		{
			mNumberOfPrimes += f[i].get();
		}
		end = std::chrono::steady_clock::now();
	}
	else
	{
		// prepare bases:
		for (size_t i = 0; i < mNumberOfBases; i++)
		{
			srand(i);
			mBase[i] = (rand() % (mCheckLimit - 1)) + 1;
		}

		std::cout << "running single threaded!" << std::endl;
		start = std::chrono::steady_clock::now();
		threadFunction(0, mCheckLimit);
		end = std::chrono::steady_clock::now();
	}
}

MillerRabinParallelPrimes::operator bool() const
{
	return mIsValid;
}

void MillerRabinParallelPrimes::printPrimes() const
{
}

void MillerRabinParallelPrimes::printCount() const
{
	std::cout << "found " << mNumberOfPrimes << " probably prime numbers." << std::endl;
}

void MillerRabinParallelPrimes::printTime() const
{
	std::cout << *this << " took " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms." << std::endl;
}