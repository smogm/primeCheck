#ifndef ATKIN_HPP
#define ATKIN_HPP

#include <basePrime.hpp>

class Atkin final : public BasePrime
{
public:
    Atkin(unsigned long upperLimit);
    virtual ~Atkin();
    
    virtual operator bool() const;
    virtual void calcPrimes();

    virtual void printPrimes() const;
    virtual void printCount() const;
    virtual void printTime() const;
  
private:
    Atkin(const Atkin&);
    Atkin& operator = (const Atkin&); 

    bool* primes;
    unsigned long upperLimit;
    std::chrono::milliseconds::rep initDurationMs;
    std::chrono::milliseconds::rep lastCalcDurationMs;
};

#endif
