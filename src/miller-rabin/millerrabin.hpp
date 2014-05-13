#ifndef MILLERRABIN_HPP
#define MILLERRABIN_HPP

#include <basePrime.hpp>
#include <mutex>
#include <vector>

class MillerRabin final : public BasePrime
{
	bool mIsValid;
	// preparation for bigger numbers:
	//const unsigned char* mCheckLimit; // limit
	//const unsigned char* mBase; // base
	const unsigned long mCheckLimit;
	
	std::mutex mPrimeListMutex; // has to be locked for adding primes
	std::vector<unsigned long> mPrimeList;

	unsigned long expModulo(unsigned long, unsigned long, unsigned long) const;
	bool check(const unsigned long, const unsigned long) const;

	public:
			MillerRabin(unsigned long);
			virtual ~MillerRabin();

			virtual explicit operator bool() const;
			virtual void printPrimes() const;
			virtual void calcPrimes();
};

#endif
