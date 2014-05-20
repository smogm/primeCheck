#ifndef ATKIN_THREADED_HPP
#define ATKIN_THREADED_HPP

#include <basePrime.hpp>
#include <mutex>

class AtkinThreaded final : public BasePrime
{
public:
    AtkinThreaded(unsigned long upperLimit);
    virtual ~AtkinThreaded();
    
    virtual operator bool() const;
    virtual void startThreads();
	virtual void calcPrimes();
	virtual void calcPrimesForRange1(unsigned int begin, unsigned int end);
	virtual void calcPrimesForRange2(unsigned int begin, unsigned int end);
	virtual void calcPrimesForRange3(unsigned int begin, unsigned int end);

    virtual void printPrimes() const;
    virtual void printCount() const;
    virtual void printTime() const;
  
private:
    AtkinThreaded(const AtkinThreaded&);
    AtkinThreaded& operator = (const AtkinThreaded&); 

    bool* primes;
	std::mutex g_lock;
	std::thread** threads;
	unsigned long upperLimit;
	unsigned long sqrtLimit;
    unsigned int numberOfThreads;
	std::chrono::milliseconds::rep initDurationMs;
    std::chrono::milliseconds::rep lastCalcDurationMs;
};

#endif
