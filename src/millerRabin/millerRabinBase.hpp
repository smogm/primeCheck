#ifndef MILLERRABINBASE_HPP
#define MILLERRABINBASE_HPP

class MillerRabinBase
{
	unsigned long expModulo(unsigned long, unsigned long, unsigned long) const;

	protected:
				virtual bool check(const unsigned long, const unsigned long) const;
				virtual ~MillerRabinBase() {};
};

#endif
