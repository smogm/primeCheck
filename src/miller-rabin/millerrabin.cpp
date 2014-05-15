#include <iostream>

#include <millerrabin.hpp>
#include <cstdlib> // rand
#include <math.h>

MillerRabin::MillerRabin(unsigned long n, unsigned long numberOfBases) :
	mIsValid(false),
	mCheckLimit(n),
	mNumberOfBases(numberOfBases),
	mTypeBitSize(sizeof(unsigned long)*8), // !!!!!!!!
	mPrimeListMutex(),
	mPrimeList()
{
	std::cout << "available cores for concurrent jobs: " << getCoreCount() << std::endl;
	std::cout << "check against " << mNumberOfBases << " bases" << std::endl;
	std::cout << "ulong size: " << mTypeBitSize << " bits" << std::endl;

	// nothing to do yet?
	if (n > 2 && numberOfBases > 2)
	{
		mIsValid = true; // object created correctly
	}
}

MillerRabin::~MillerRabin()
{
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

	for(unsigned long r=1; r < s; r++) {
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
	bool isPrime = false;
	unsigned long i = 0;
	unsigned long base = 0;
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
	for (unsigned long n = mPrimeList[mPrimeList.size()-1]/*3*/; n <= mCheckLimit; n+=2)
	{
		isPrime = false;
		i = 0;

		if ((n % 3 != 0) && (n % 5 != 0) && (n % 7 != 0) && (n % 11 != 0) && (n % 13 != 0) && (n % 17 != 0) && (n % 19 !=0)
			&& (n % 23 != 0) && (n %29 != 0))
		{
			// iterate the bases
			do
			{
				srand(i);
				base = (rand() % (n - 1)) + 1;
				isPrime = check(base, n);
				i++;
			} while (i < mNumberOfBases && isPrime);

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

std::string MillerRabin::getName() const
{
    return "MillerRabin";
}
