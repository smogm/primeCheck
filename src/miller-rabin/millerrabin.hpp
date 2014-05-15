#ifndef MILLERRABIN_HPP
#define MILLERRABIN_HPP

#include <basePrime.hpp>
#include <thread>
#include <mutex>
#include <vector>
#include <iostream>

class MillerRabin final : public BasePrime
{
	bool mIsValid;
	// preparation for bigger numbers:
	//const unsigned char* mCheckLimit; // limit
	//const unsigned char* mBase; // base
	const unsigned long mCheckLimit;

	const size_t mNumberOfBases;
	const size_t mTypeBitSize;

	const size_t mNumberOfThreads;
	std::thread** mThread;

	std::mutex mPrimeListMutex; // has to be locked for adding primes
	std::vector<unsigned long> mPrimeList;

	unsigned long expModulo(unsigned long, unsigned long, unsigned long) const;
	bool check(const unsigned long, const unsigned long) const;

	// disable copy and assignment
	MillerRabin(const MillerRabin&) = delete;
	const MillerRabin& operator=(const MillerRabin&) = delete;

	public:
			MillerRabin(unsigned long checkLimit, unsigned long numberOfBases);
			virtual ~MillerRabin();

			virtual explicit operator bool() const;
			virtual void calcPrimes();

			virtual void printPrimes() const;
			virtual void printCount() const;
			virtual void printTime() const;
};

#endif
