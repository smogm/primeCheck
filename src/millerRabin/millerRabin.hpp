#ifndef MILLERRABIN_HPP
#define MILLERRABIN_HPP

#include <basePrime.hpp>
#include <millerrabinWorker.hpp>
#include <millerRabinBase.hpp>

#include <thread>
#include <vector>
#include <iostream>

//#define USE_OPENMP

class MillerRabin final : public BasePrime, public MillerRabinBase
{
	bool mIsValid;
	// preparation for bigger numbers:
	//const unsigned char* mCheckLimit; // limit
	//const unsigned char* mBase; // base
	const unsigned long mCheckLimit;

	const size_t mNumberOfBases;
	const size_t mTypeBitSize;

	const size_t mNumberOfThreads;
	unsigned long* const mBase;

	MillerRabinWorker<unsigned long>** mWorker;

	std::vector<unsigned long> mPrimeList;

	// disable copy and assignment
	MillerRabin(const MillerRabin&) = delete;
	const MillerRabin& operator=(const MillerRabin&) = delete;

	public:
			MillerRabin(unsigned long checkLimit, unsigned long numberOfBases);
			virtual ~MillerRabin();

			virtual explicit operator bool() const;
			void calcPrimesParallelBase();
			void calcPrimeParallel();
			virtual void calcPrimes();

			virtual void printPrimes() const;
			virtual void printCount() const;
			virtual void printTime() const;
};

#endif
