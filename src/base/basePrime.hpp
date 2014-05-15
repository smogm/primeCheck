#ifndef BASEPRIME_HPP
#define BASEPRIME_HPP

#include <chrono>
#include <thread>
#include <string>

class BasePrime
{
	protected:
		std::chrono::steady_clock::time_point start, end;
		const std::string className;

		unsigned int getCoreCount() const { return std::thread::hardware_concurrency(); }
	public:
			BasePrime(const std::string className) : start(), end(), className(className) {};
			virtual ~BasePrime() {};

			/* pure virtuals */
			// implement as explicit to be safe-bool !!!
			virtual operator bool() const = 0;
			virtual void calcPrimes() = 0;

			virtual void printPrimes() const = 0;
			virtual void printCount() const = 0;
			virtual void printTime() const = 0;
            //virtual std::string getName() const = 0;

            friend std::ostream& operator<<(std::ostream&, const BasePrime&);
};

#endif
