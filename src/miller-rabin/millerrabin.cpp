#if defined(DEBUG)
#include <iostream>
#endif

#include <millerrabin.hpp>

const unsigned int base[] = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43 };

MillerRabin::MillerRabin(unsigned long n) :
	mIsPrime(false),
	mPPrime(n)
{
	// first check if n is odd
	if ((mPPrime & 1) == 1)
	{
		for (int i = 0; i < 14; i++)
		{
			pass(base[i]); // start to parallelize here!
		}
	}
	else if (mPPrime == 2) // the only even prime number is 2
	{
		mIsPrime = true;
	}
}

MillerRabin::~MillerRabin()
{
}

unsigned long MillerRabin::expModulo(unsigned long base, unsigned long power, unsigned long modulus) {
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

void MillerRabin::pass(unsigned long base)
{
	unsigned long a_to_power, s, d, i;
	s = 0;
	d = mPPrime - 1;
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

	a_to_power = expModulo(base, d, mPPrime);
	if (a_to_power == 1)
	{
		mIsPrime = true;
		return;
	}

	for(i=0; i < s-1; i++) {
		if (a_to_power == mPPrime-1)
		{
			mIsPrime = true;
			return;
		}
		a_to_power = expModulo(a_to_power, 2, mPPrime);
	}

	if (a_to_power == mPPrime-1)
	{
		mIsPrime = true;
	}

	return;
}

MillerRabin::operator bool() const
{
	return mIsPrime;
}
