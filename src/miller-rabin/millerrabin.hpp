#ifndef MILLERRABIN_HPP
#define MILLERRABIN_HPP

#include <basePrime.hpp>

class MillerRabin final : public BasePrime
{
	bool mIsPrime;
	// preparation for bigger numbers:
	//const unsigned char* mPPrime; // number to check whether it's possibly prime
	//const unsigned char* mBase; // base
	const unsigned long mPPrime;

	unsigned long expModulo(unsigned long, unsigned long, unsigned long);
	void pass(unsigned long);

	public:
			MillerRabin(unsigned long);
			virtual ~MillerRabin();

			virtual explicit operator bool() const;
};

#endif
