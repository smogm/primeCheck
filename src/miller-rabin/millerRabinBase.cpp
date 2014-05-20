#include <millerRabinBase.hpp>

unsigned long MillerRabinBase::expModulo(const unsigned long base, const unsigned long power, const unsigned long modulus) const
{
	unsigned long result = 1; // take care of the bitsize!!

	for (int i=(sizeof(unsigned long)*8 -1); i>=0; i--)
	{
		result = (result*result) % modulus;
		if (power & (1 << i))
		{
			result = (result*base) % modulus;
		}
	}

	return result;
}

bool MillerRabinBase::check(const unsigned long base, const unsigned long pprime) const
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

    for(unsigned long r=1; r < s; r++)
    {
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
