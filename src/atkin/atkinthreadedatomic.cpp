#include <AtkinThreadedAtomic.hpp>
#include <iostream>	
#include <thread>
#include <limits.h>
#include <math.h>
#include <cstdio>

AtkinThreadedAtomic::AtkinThreadedAtomic(unsigned long upperLimit)
    : BasePrime("AtkinThreadedAtomic")
    , primes(nullptr)
    , threads(nullptr)
	, upperLimit(upperLimit)
	, sqrtLimit(0)
    , numberOfThreads(getCoreCount())
	, initDurationMs(0)
    , lastCalcDurationMs(0)
{
	//numberOfThreads *=3;
	
	start = std::chrono::steady_clock::now();
    primes = new std::atomic_bool[upperLimit];
	threads = new std::thread*[numberOfThreads];
	sqrtLimit = static_cast<unsigned long>(sqrt(upperLimit));	
	
	unsigned long i;
	unsigned long j;
	unsigned long k;
	long stepSize = upperLimit / numberOfThreads;
	long lastStepSize = upperLimit % numberOfThreads;
	
	printf("Starting threads for init...\n");
    for(i=0, j=0; i<=upperLimit && j<numberOfThreads; i+=stepSize, j++) {
		// printf("Starting thread %i: %i - %i\n", j, i, i+stepSize-1);
		threads[j] = new std::thread(&AtkinThreadedAtomic::initForRange, this, i, i+stepSize-1);
	}
	if(lastStepSize != 0) {
		// printf("Starting last thread: %i - %i\n", i, i+lastStepSize);
		std::thread extraThread(&AtkinThreadedAtomic::initForRange, this, i, i+lastStepSize);
		extraThread.join();
	}
	for(k=0; k<numberOfThreads; k++) {
		threads[k]->join();
		delete threads[k];
	}
	
    end = std::chrono::steady_clock::now();
    initDurationMs = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

void AtkinThreadedAtomic::initForRange(unsigned long begin, unsigned long end) {
	for(unsigned long i=begin; i<=end; i++) {
		primes[i] = false;
	}
}

AtkinThreadedAtomic::~AtkinThreadedAtomic(){
    delete[] primes;
	primes = NULL;
	delete[] threads;
    threads = NULL;
}

void AtkinThreadedAtomic::calcPrimes() {
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

void AtkinThreadedAtomic::startThreads() {
	unsigned int i;
	unsigned int j;
	unsigned int k;
	int stepSize = sqrtLimit / numberOfThreads;
	int lastStepSize = sqrtLimit % numberOfThreads;
	
	printf("Starting threads for 4*x^2+y^2, 3*x^2+y^2 and 3*x^2-y^2...\n");
    for(i=0, j=0; i<=sqrtLimit && j<numberOfThreads; i+=stepSize, j++) {
		threads[j] = new std::thread(&AtkinThreadedAtomic::calcPrimesForRange, this, i, i+stepSize-1);
	}
	if(lastStepSize != 0) {
		std::thread extraThread(&AtkinThreadedAtomic::calcPrimesForRange, this, i, i+lastStepSize);
		extraThread.join();
	}
	for(k=0; k<numberOfThreads; k++) {
		threads[k]->join();
		delete threads[k];
	}
	
}
    
void AtkinThreadedAtomic::calcPrimesForRange(unsigned int begin, unsigned int end) {
    unsigned long xSquared = begin*begin;
	for(unsigned int x=begin; x<=end; xSquared = xSquared + 2*x + 1, x++) {
		const unsigned long fourTimesXSquared = 4*xSquared;
		const unsigned long threeTimesXSquared = 3*xSquared;
		
		unsigned long n = 0;
        unsigned long ySquared = 0;
		for(unsigned int y=0; y<=sqrtLimit; ySquared = ySquared + 2*y + 1, y++) {
				
			n = fourTimesXSquared+ySquared;
			const unsigned long nModTwelve = n%12;
			if (n < upperLimit && (nModTwelve == 1 || nModTwelve == 5)) {
                bool old = primes[n].load();
                while(!primes[n].compare_exchange_weak(old,!old));
			}
            
            n = threeTimesXSquared+ySquared;
			if (n < upperLimit && n%12 == 7) {
                bool old = primes[n].load();
                while(!primes[n].compare_exchange_weak(old,!old));
			}
            
            if(x>y){
                n = threeTimesXSquared-ySquared;
                if (n < upperLimit && n%12 == 11) {
                    bool old = primes[n].load();
                    while(!primes[n].compare_exchange_weak(old,!old));
                }
            }
		}
	}
}


void AtkinThreadedAtomic::printPrimes() const {
	std::cout << "2 3 ";
	for(unsigned long i=5; i<upperLimit; i++) {
		if(primes[i]) {
			std::cout << i << " ";
		}
	}
    std::cout << std::endl;
}

void AtkinThreadedAtomic::printCount() const{
    int count = 2;
    for(unsigned long i=5; i<upperLimit; i++) {
		if(primes[i]) {
			++count;
		}
	}
    std::cout << "#primes = " << count << std::endl;
}
void AtkinThreadedAtomic::printTime() const{
	std::cout << "Time spent=" << (initDurationMs+lastCalcDurationMs) << " ms. (init=" << (initDurationMs) << "ms calc=" << (lastCalcDurationMs) << "ms)" << std::endl;
}

AtkinThreadedAtomic::operator bool() const{
    return false;
}
