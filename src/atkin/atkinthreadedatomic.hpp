#ifndef ATKIN_THREADED_ATOMIC_HPP
#define ATKIN_THREADED_ATOMIC_HPP

#include <basePrime.hpp>
#include <atomic>

class AtkinThreadedAtomic final : public BasePrime
{
public:
    AtkinThreadedAtomic(unsigned long upperLimit);
    virtual ~AtkinThreadedAtomic();
    
    virtual operator bool() const;
    virtual void startThreads();
	virtual void calcPrimes();
	virtual void initForRange(unsigned long begin, unsigned long end);
	virtual void calcPrimesForRange(unsigned int begin, unsigned int end);

    virtual void printPrimes() const;
    virtual void printCount() const;
    virtual void printTime() const;
  
private:
    AtkinThreadedAtomic(const AtkinThreadedAtomic&);
    AtkinThreadedAtomic& operator = (const AtkinThreadedAtomic&); 

    std::atomic_bool* primes;
	std::thread** threads;
	unsigned long upperLimit;
	unsigned long sqrtLimit;
    unsigned int numberOfThreads;
	std::chrono::milliseconds::rep initDurationMs;
    std::chrono::milliseconds::rep lastCalcDurationMs;
};

#endif
