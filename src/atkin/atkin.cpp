#include <iostream>
#include <limits.h>
#include <math.h> 

#include <atkin.hpp>

// DONE check for performance issues
// TODO check for MORE performance issues

Atkin::Atkin(unsigned long upperLimit)
    : primes(NULL)
    , upperLimit(upperLimit)
    , initDurationMs(0)
    , lastCalcDurationMs(0)
{
    start = std::chrono::steady_clock::now();
    primes = new bool[upperLimit];
	for(unsigned long i=0; i<upperLimit; i++) {
		primes[i] = false;
	}
    end = std::chrono::steady_clock::now();
    initDurationMs = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

Atkin::~Atkin(){
    delete[] primes;
    primes = NULL;
}
    
void Atkin::calcPrimes() {
    start = std::chrono::steady_clock::now();
    const unsigned long sqrtLimit = static_cast<unsigned long>(sqrt(upperLimit));
	unsigned long n = 0;
	for(unsigned int x=0; x<=sqrtLimit; x++) {
        const unsigned long xSquared = x*x;
        const unsigned long threeTimesXSquared = 3*xSquared;
        const unsigned long fourTimesXSquared = 4*xSquared;
		for(unsigned int y=0; y<=sqrtLimit; y++) {
            const unsigned long ySquared = y*y;
            
			n = fourTimesXSquared+ySquared;
            const unsigned long nModTwelve = n%12;
			if (n < upperLimit && (nModTwelve == 1 || nModTwelve == 5)) {
				primes[n] = !primes[n];
			}
			
			n = threeTimesXSquared+ySquared;
			if (n < upperLimit && n%12 == 7) {
				primes[n] = !primes[n];
			}
			
			
			n = threeTimesXSquared-ySquared;
			if (x > y && n < upperLimit && n%12 == 11) {
				primes[n] = !primes[n];
			}
		}
	}

	for(unsigned int n=5; n<=sqrtLimit; n++) {
		if(primes[n]) {
            const unsigned long nSquared = n*n;
            unsigned long multipleOfNSquared = nSquared;
			while(multipleOfNSquared<upperLimit) {
				primes[multipleOfNSquared] = false;
                multipleOfNSquared += nSquared;
			}
		}
	}
    end = std::chrono::steady_clock::now();
    lastCalcDurationMs = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

void Atkin::printPrimes() const {
	std::cout << "2 3 ";
	for(unsigned long i=5; i<upperLimit; i++) {
		if(primes[i]) {
			std::cout << i << " ";
		}
	}
    std::cout << std::endl;
}

void Atkin::printCount() const{
    int count = 2;
    for(unsigned long i=5; i<upperLimit; i++) {
		if(primes[i]) {
			++count;
		}
	}
    std::cout << "#primes = " << count << std::endl;
}
void Atkin::printTime() const{
	std::cout << "Time spent=" << (initDurationMs+lastCalcDurationMs) << " ms. (init=" << (initDurationMs) << "ms calc=" << (lastCalcDurationMs) << "ms)" << std::endl;
}
Atkin::operator bool() const{
    return false;
}