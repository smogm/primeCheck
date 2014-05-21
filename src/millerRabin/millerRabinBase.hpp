#ifndef MILLERRABINBASE_HPP
#define MILLERRABINBASE_HPP

class MillerRabinBase
{
	unsigned long mul(unsigned long a, unsigned long factor, unsigned long modulus) const;
	unsigned long expModulo(const unsigned long, const unsigned long, const unsigned long) const;

	protected:
				virtual bool check(const unsigned long, const unsigned long) const;
				virtual ~MillerRabinBase() {};
};

#endif
