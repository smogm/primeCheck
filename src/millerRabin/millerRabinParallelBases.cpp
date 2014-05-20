#include <millerRabinParallelBases.hpp>
#include <iostream>
#include <future>
#include <vector>

MillerRabinParallelBases::MillerRabinParallelBases(const unsigned long n, const unsigned long numberOfBases) :
	BasePrime("MillerRabinParallelBases"),
	mIsValid(false),
	mCheckLimit(n),
	mNumberOfBases(numberOfBases)
{
	std::cout << "check against " << mNumberOfBases << " bases" << std::endl;

	// nothing to do yet?
	if (n > 2 && numberOfBases > 2)
	{
		mIsValid = true; // object created correctly
	}
}

MillerRabinParallelBases::operator bool() const
{
	return mIsValid;
}

void MillerRabinParallelBases::calcPrimes()
{
	std::future<std::vector<unsigned long>*> f[mNumberOfBases];
	std::vector<unsigned long>* primeArray[mNumberOfBases];

	start = std::chrono::steady_clock::now();
	for (size_t i = 0; i < mNumberOfBases; i++)
	{
		unsigned long base = rand() % (mCheckLimit - 1) + 1;
		f[i] = std::async(std::launch::async, [=]() {
			std::vector<unsigned long>* primes = new std::vector<unsigned long>;

			for (unsigned int n = 3; n < mCheckLimit; n+=2)
			{
				if (check(base, n))
				{
					primes->push_back(n);
				}
			}

			return primes;
		});
	}

	for (size_t i = 0; i < mNumberOfBases; i++)
	{
		primeArray[i] = f[i].get();
		std::cout << "thread found " << primeArray[i]->size() << " primes" << std::endl;
		delete primeArray[i];
	}
	end = std::chrono::steady_clock::now();
	std::cout << "TODO: find a way to accumulate/adjust them!" << std::endl;
}

void MillerRabinParallelBases::printPrimes() const
{
	
}

void MillerRabinParallelBases::printCount() const
{
	
}

void MillerRabinParallelBases::printTime() const
{
	std::cout << *this << " took " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms." << std::endl;
}
