#ifndef BASEPRIME_HPP
#define BASEPRIME_HPP

#include <chrono>

class BasePrime
{
	protected:
		std::chrono::steady_clock::time_point start, end;
	public:
			BasePrime() : start(), end() {};
			virtual ~BasePrime() {};

			// implement as explicit to be safe-bool !!!
			virtual operator bool() const = 0;
			virtual void printPrimes() const = 0;
			virtual void calcPrimes() = 0;
};

#endif
