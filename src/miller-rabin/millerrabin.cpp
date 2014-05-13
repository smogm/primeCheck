#include <iostream>

#include <millerrabin.hpp>

const int NUM_BASES = 14;
const unsigned int base[NUM_BASES] = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43 };

MillerRabin::MillerRabin(unsigned long n) :
	mIsValid(false),
	mCheckLimit(n),
	mPrimeListMutex(),
	mPrimeList()
{
	// nothing to do yet?
	if (n > 2)
	{
		mIsValid = true; // object created correctly
	}
}

MillerRabin::~MillerRabin()
{
}

unsigned long MillerRabin::expModulo(const unsigned long base, const unsigned long power, const unsigned long modulus) const
{
	unsigned long result = 1;

	for (int i=15; i>=0; i--)
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
	unsigned long a, s, d, i;
	s = 0;
	d = pprime - 1;
	while ((d % 2) == 0)
	{
		d /= 2;
		s++;
	}

#if defined(DEBUG)
	std::cout << "base: " << std::dec << base << std::endl;
	std::cout << "s: " << std::dec << s << std::endl;
	std::cout << "d: " << std::dec << d << std::endl;
#endif

	a = expModulo(base, d, pprime);
	if (a == 1)
	{
		return true;
	}

	for(i=0; i < s-1; i++) {
		if (a == pprime-1)
		{
			return true;
		}
		a = expModulo(a, 2, pprime);
	}

	if (a == pprime-1)
	{
		return true;
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
		printf("%lu\tis probably prime\n", prime);
	}
	std::cout << "found " << mPrimeList.size() << " probably prime numbers." << std::endl;
	std::cout << "checked with prime bases: ";
	for (auto a : base)
	{
		printf("%u ", a);
	}
	std::cout << std::endl;
	std::cout << "MillerRabin took " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " µs." << std::endl;
}

void MillerRabin::calcPrimes()
{
	bool isPrime = false;
	int i = 0;
	mPrimeList.clear();
	mPrimeList.push_back(1);
	mPrimeList.push_back(2);

	start = std::chrono::steady_clock::now();
	for (unsigned long n = 3; n <= mCheckLimit; n++)
	{
		// first check if n is odd
		if ((n & 1) == 1)
		{
			isPrime = false;
			i = 0;

			// iterate the bases
			do
			{
				isPrime = check(base[i++], n);
			} while (i < NUM_BASES && isPrime);

			if (isPrime) // n passed the check
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
