#ifndef MILLERRABINPARALLELPRIMES_HPP
#define MILLERRABINPARALLELPRIMES_HPP

#include <basePrime.hpp>
#include <millerRabinBase.hpp>

class MillerRabinParallelPrimes final : public BasePrime, public MillerRabinBase
{
	bool mIsValid;

	const unsigned long mCheckLimit;
	const size_t mNumberOfBases;
	const size_t mNumberOfThreads;
	unsigned long* const mBase;
	size_t mNumberOfPrimes;

	MillerRabinParallelPrimes(const MillerRabinParallelPrimes&) = delete;
	const MillerRabinParallelPrimes& operator=(const MillerRabinParallelPrimes&) = delete;

	size_t threadFunction(const unsigned long mStart, const unsigned long mStop);

	public:
			MillerRabinParallelPrimes(unsigned long, unsigned long);
			virtual ~MillerRabinParallelPrimes();

			virtual explicit operator bool() const;
			virtual void calcPrimes();
			virtual void printPrimes() const;
			virtual void printCount() const;
			virtual void printTime() const;
};

#endif
