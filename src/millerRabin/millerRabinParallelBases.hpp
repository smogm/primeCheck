#ifndef MILLERRABINPARALLELBASE_HPP
#define MILLERRABINPARALLELBASE_HPP

#include <basePrime.hpp>
#include <millerRabinBase.hpp>

class MillerRabinParallelBases final : public BasePrime, public MillerRabinBase
{
	bool mIsValid;

	const unsigned long mCheckLimit;
	const size_t mNumberOfBases;

	public:
			MillerRabinParallelBases(const unsigned long, const unsigned long);

			virtual explicit operator bool() const;
			
			virtual void calcPrimes();
			virtual void printPrimes() const;
			virtual void printCount() const;
			virtual void printTime() const;
};

#endif
