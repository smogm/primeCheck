#include <iostream>	
#include <thread>
#include <limits.h>
#include <math.h>
#include <cstdio>
#include <atkinthreadedvar2.hpp>


AtkinThreadedVar2::AtkinThreadedVar2(unsigned long upperLimit)
    : BasePrime("AtkinThreadedVar2")
    , primes(nullptr)
    , threads(nullptr)
	, upperLimit(upperLimit)
	, sqrtLimit(static_cast<unsigned long>(sqrt(upperLimit)))
    , numberOfThreads(getCoreCount())
	, initDurationMs(0)
    , lastCalcDurationMs(0)
    , stepSize(sqrtLimit / numberOfThreads)
    , lastStepSize(sqrtLimit % numberOfThreads)
{
	start = std::chrono::steady_clock::now();
    primes = new bool[upperLimit];
	threads = new std::thread*[numberOfThreads];
    
	for(unsigned long i=0; i<upperLimit; i++) {
		primes[i] = false;
	}
	
    end = std::chrono::steady_clock::now();
    initDurationMs = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

AtkinThreadedVar2::~AtkinThreadedVar2(){
    delete[] primes;
	primes = NULL;
	delete[] threads;
    threads = NULL;
}

void AtkinThreadedVar2::calcPrimes() {
	start = std::chrono::steady_clock::now();
	
	printf("Starting calcPrimes... (sqrtLimit=%li)\n", sqrtLimit);
	startThreads();
	
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

void AtkinThreadedVar2::startThreads() {	
	printf("Starting threads for 4*x^2+y^2...\n");
    calcRange1();
	
	printf("Starting threads for 3*x^2+y^2 and 3*x^2-y^2...\n");
    calcRange2();
}
    
void AtkinThreadedVar2::calcRange1() {
	unsigned int i;
	unsigned int j;
	unsigned int k;
    
	for(unsigned int x=0; x<=sqrtLimit; x++) {
		const unsigned long xSquared = x*x;
		const unsigned long fourTimesXSquared = 4*xSquared;
        
        for(i=0, j=0; i<=sqrtLimit && j<numberOfThreads; i+=stepSize, j++) {
            threads[j] = new std::thread(&AtkinThreadedVar2::calcPrimesForRange1, this, fourTimesXSquared, i, i+stepSize-1);
        }
        if(lastStepSize != 0) {
            std::thread extraThread(&AtkinThreadedVar2::calcPrimesForRange1, this, fourTimesXSquared, i, i+lastStepSize);
            extraThread.join();
        }
        for(k=0; k<numberOfThreads; k++) {
            threads[k]->join();
            delete threads[k];
        }
        
    }
}
void AtkinThreadedVar2::calcPrimesForRange1(const unsigned long fourTimesXSquared, const unsigned int begin, const unsigned int end) {
    unsigned long n = 0;
    for(unsigned int y=begin; y<=end; y++) {
        const unsigned long ySquared = y*y;
            
        n = fourTimesXSquared+ySquared;
        const unsigned long nModTwelve = n%12;
        if (n < upperLimit && (nModTwelve == 1 || nModTwelve == 5)) {
            primes[n] = !primes[n];
        }
    }
}

void AtkinThreadedVar2::calcRange2() {
	unsigned int i;
	unsigned int j;
	unsigned int k;
    
	for(unsigned int x=0; x<=sqrtLimit; x++) {
		const unsigned long xSquared = x*x;
		const unsigned long threeTimesXSquared = 3*xSquared;
        
        for(i=0, j=0; i<=sqrtLimit && j<numberOfThreads; i+=stepSize, j++) {
            threads[j] = new std::thread(&AtkinThreadedVar2::calcPrimesForRange2, this, x, threeTimesXSquared, i, i+stepSize-1);
        }
        if(lastStepSize != 0) {
            std::thread extraThread(&AtkinThreadedVar2::calcPrimesForRange2, this, x, threeTimesXSquared, i, i+lastStepSize);
            extraThread.join();
        }
        for(k=0; k<numberOfThreads; k++) {
            threads[k]->join();
            delete threads[k];
        }
        
    }
}
void AtkinThreadedVar2::calcPrimesForRange2(const unsigned int x, const unsigned long threeTimesXSquared, const unsigned int begin, const unsigned int end) {
		
    unsigned long n = 0;
    for(unsigned int y=begin; y<=end; y++) {
        const unsigned long ySquared = y*y;
            
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


void AtkinThreadedVar2::printPrimes() const {
	std::cout << "2 3 ";
	for(unsigned long i=5; i<upperLimit; i++) {
		if(primes[i]) {
			std::cout << i << " ";
		}
	}
    std::cout << std::endl;
}

void AtkinThreadedVar2::printCount() const{
    int count = 2;
    for(unsigned long i=5; i<upperLimit; i++) {
		if(primes[i]) {
			++count;
		}
	}
    std::cout << "#primes = " << count << std::endl;
}
void AtkinThreadedVar2::printTime() const{
	std::cout << "Time spent=" << (initDurationMs+lastCalcDurationMs) << " ms. (init=" << (initDurationMs) << "ms calc=" << (lastCalcDurationMs) << "ms)" << std::endl;
}

AtkinThreadedVar2::operator bool() const{
    return false;
}
