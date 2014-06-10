#include <climits>
#include <cstdio>
#include <cmath>
#include <iostream>
#include <cstring>

#include <eratosthenes.hpp>

Eratosthenes::Eratosthenes(const unsigned long upperLimit)
    : BasePrime("Eratosthenes")
    , crossedOut(nullptr)
    , upperLimit(upperLimit)
    , arrayLen ((unsigned long int) ceil(upperLimit/2))
    , sqrtUpperLimit ((int) ceil(sqrt(limitN)))
    , initDurationMs(0)
    , calcDurationMs(0)
{
    start = std::chrono::steady_clock::now();
    
    crossedOut = new bool[arrayLen];    
    memset (crossedOut, 0, arrayLen);
	
    end = std::chrono::steady_clock::now();
    initDurationMs = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

Eratosthenes::~Eratosthenes(){
    delete[] crossedOut;
    crossedOut = NULL;
}

void Eratosthenes::calcPrimes() {
    start = std::chrono::steady_clock::now();
    unsigned long int n = 3;
    for (unsigned long int i = 0; n < sqrtUpperLimit; i++)
    {
        n = i*2+3;
        if (!crossedOut[i])
        {
            for (unsigned long int j = n*n; j < upperLimit; j += n)
            {
                if (j%2 != 0)
                {
                    crossedOut[(j-3)/2] = true;
                }
            }
        }
    }
    end = std::chrono::steady_clock::now();
    calcDurationMs = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

void Eratosthenes::printPrimes() const {
	 std::cout << "2 ";
    for (unsigned long i=0; (i*2+3)<upperLimit; i++) {
		  if (!crossedOut[i]) {
            std::cout << i*2+3 << " ";
		  }
	 }
    std::cout << std::endl;
}

void Eratosthenes::printCount() const{
    int count = 1;
    for (unsigned long i=0; (i*2+3)<upperLimit; i++) {
		  if (!crossedOut[i]) {
			   count++;
		  }
	 }
    std::cout << "#primes = " << count << std::endl;
}

void Eratosthenes::printTime() const{
	std::cout << "Time spent=" << (initDurationMs+calcDurationMs) << " ms. (init=" << (initDurationMs) << "ms calc=" << (calcDurationMs) << "ms)" << std::endl;
}

Eratosthenes::operator bool() const{
    return false;
}
