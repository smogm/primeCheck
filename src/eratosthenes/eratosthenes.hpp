#ifndef ERATOSTHENES_HPP
#define ERATOSTHENES_HPP

#include <basePrime.hpp>

class Eratosthenes final : public BasePrime
{
public:
    Eratosthenes(unsigned long upperLimit);
    virtual ~Eratosthenes();
    
    virtual operator bool() const;
    virtual void calcPrimes();

    virtual void printPrimes() const;
    virtual void printCount() const;
    virtual void printTime() const;
  
private:
    Eratosthenes(const Eratosthenes&);
    Eratosthenes& operator = (const Eratosthenes&); 

    bool* crossedOut;
    const unsigned long upperLimit;
    const unsigned long arrayLen;
    const unsigned long sqrtUpperLimit;
    std::chrono::milliseconds::rep initDurationMs;
    std::chrono::milliseconds::rep calcDurationMs;
};

#endif