#ifndef BASEPRIME_HPP
#define BASEPRIME_HPP

#include <chrono>
#include <thread>

class BasePrime
{
	protected:
		std::chrono::steady_clock::time_point start, end;

		unsigned int getCoreCount() const { return std::thread::hardware_concurrency(); }
	public:
			BasePrime() : start(), end() {};
			virtual ~BasePrime() {};

			/* pure virtuals */
			// implement as explicit to be safe-bool !!!
			virtual operator bool() const = 0;
			virtual void calcPrimes() = 0;

			virtual void printPrimes() const = 0;
			virtual void printCount() const = 0;
			virtual void printTime() const = 0;
};

#endif
