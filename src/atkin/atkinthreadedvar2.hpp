#ifndef ATKIN_THREADED_VAR2_HPP
#define ATKIN_THREADED_VAR2_HPP

#include <basePrime.hpp>

class AtkinThreadedVar2 final : public BasePrime
{
public:
    AtkinThreadedVar2(unsigned long upperLimit);
    virtual ~AtkinThreadedVar2();
    
    virtual operator bool() const;
    virtual void startThreads();
	virtual void calcPrimes();
    void calcRange1();
	void calcPrimesForRange1(const unsigned long fourTimesXSquared, const unsigned int begin, const unsigned int end);
    void calcRange2();
	void calcPrimesForRange2(const unsigned int x, const unsigned long threeTimesXSquared, const unsigned int begin, const unsigned int end);

    virtual void printPrimes() const;
    virtual void printCount() const;
    virtual void printTime() const;
  
private:
    AtkinThreadedVar2(const AtkinThreadedVar2&);
    AtkinThreadedVar2& operator = (const AtkinThreadedVar2&); 

    bool* primes;
	std::thread** threads;
	const unsigned long upperLimit;
	const unsigned long sqrtLimit;
    const unsigned int numberOfThreads;
	std::chrono::milliseconds::rep initDurationMs;
    std::chrono::milliseconds::rep lastCalcDurationMs;
	const unsigned long stepSize;
	const unsigned long lastStepSize;
};

#endif
