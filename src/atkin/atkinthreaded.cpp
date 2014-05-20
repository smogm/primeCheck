#include <iostream>	
#include <thread>
#include <limits.h>
#include <math.h>
#include <cstdio>
#include <atkinthreaded.hpp>

// TODO check for performance issues
// MUTEXES ARE BAD!!!!

AtkinThreaded::AtkinThreaded(unsigned long upperLimit)
    : BasePrime("AtkinThreaded")
    , primes(nullptr)
	, g_lock()
    , threads(nullptr)
	, upperLimit(upperLimit)
	, sqrtLimit(0)
    , numberOfThreads(getCoreCount())
	, initDurationMs(0)
    , lastCalcDurationMs(0)
{
	//numberOfThreads *=3;
	
	start = std::chrono::steady_clock::now();
    primes = new bool[upperLimit];
	threads = new std::thread*[numberOfThreads];
	sqrtLimit = static_cast<unsigned long>(sqrt(upperLimit));	
	
	for(unsigned long i=0; i<upperLimit; i++) {
		primes[i] = false;
	}
	
    end = std::chrono::steady_clock::now();
    initDurationMs = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

AtkinThreaded::~AtkinThreaded(){
    delete[] primes;
	primes = NULL;
	delete[] threads;
    threads = NULL;
}

void AtkinThreaded::calcPrimes() {
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

void AtkinThreaded::startThreads() {
	unsigned int i;
	unsigned int j;
	unsigned int k;
	int stepSize = sqrtLimit / numberOfThreads;
	int lastStepSize = sqrtLimit % numberOfThreads;
	
	printf("Starting threads for 4*x^2+y^2...\n");
	for(i=0, j=0; i<=sqrtLimit && j<numberOfThreads; i+=stepSize, j++) {
		printf("Starting thread %i: %i - %i\n", j, i, i+stepSize-1);
		threads[j] = new std::thread(&AtkinThreaded::calcPrimesForRange1, this, i, i+stepSize-1);
	}
	if(lastStepSize != 0) {
		printf("Starting last thread: %i - %i\n", i, i+lastStepSize);
		std::thread extraThread(&AtkinThreaded::calcPrimesForRange1, this, i, i+lastStepSize);
		extraThread.join();
	}
	for(k=0; k<numberOfThreads; k++) {
		threads[k]->join();
		delete threads[k];
	}
	
	
	
	printf("Starting threads for 3*x^2+y^2...\n");
	for(i=0, j=0; i<=sqrtLimit && j<numberOfThreads; i+=stepSize, j++) {
		printf("Starting thread %i: %i - %i\n", j, i, i+stepSize-1);
		threads[j] = new std::thread(&AtkinThreaded::calcPrimesForRange2, this, i, i+stepSize-1);
	}
	if(lastStepSize != 0) {
		printf("Starting last thread: %i - %i\n", i, i+lastStepSize);
		std::thread extraThread(&AtkinThreaded::calcPrimesForRange2, this, i, i+lastStepSize);
		extraThread.join();
	}
	for(k=0; k<numberOfThreads; k++) {
		threads[k]->join();
		delete threads[k];
	}
	
	printf("Starting threads for 3*x^2-y^2...\n");
	for(i=0, j=0; i<=sqrtLimit && j<numberOfThreads; i+=stepSize, j++) {
		printf("Starting thread %i: %i - %i\n", j, i, i+stepSize-1);
		threads[j] = new std::thread(&AtkinThreaded::calcPrimesForRange3, this, i, i+stepSize-1);
	}
	if(lastStepSize != 0) {
		printf("Starting last thread: %i - %i\n", i, i+lastStepSize);
		std::thread extraThread(&AtkinThreaded::calcPrimesForRange3, this, i, i+lastStepSize);
		extraThread.join();
	}
	for(k=0; k<numberOfThreads; k++) {
		threads[k]->join();
		delete threads[k];
	}
}
    
void AtkinThreaded::calcPrimesForRange1(unsigned int begin, unsigned int end) {
	for(unsigned int x=begin; x<=end; x++) {
		const unsigned long xSquared = x*x;
		const unsigned long fourTimesXSquared = 4*xSquared;
		
		unsigned long n = 0;
		for(unsigned int y=0; y<=sqrtLimit; y++) {
			const unsigned long ySquared = y*y;
				
			n = fourTimesXSquared+ySquared;
			const unsigned long nModTwelve = n%12;
			if (n < upperLimit && (nModTwelve == 1 || nModTwelve == 5)) {
				g_lock.lock();
				primes[n] = !primes[n];
				g_lock.unlock();
			}
		}
	}
}

void AtkinThreaded::calcPrimesForRange2(unsigned int begin, unsigned int end) {
	for(unsigned int x=begin; x<=end; x++) {
		const unsigned long xSquared = x*x;
		const unsigned long threeTimesXSquared = 3*xSquared;
		
		unsigned long n = 0;
		for(unsigned int y=0; y<=sqrtLimit; y++) {
			const unsigned long ySquared = y*y;
				
			n = threeTimesXSquared+ySquared;
			if (n < upperLimit && n%12 == 7) {
				g_lock.lock();
				primes[n] = !primes[n];
				g_lock.unlock();
			}
		}
	}
}

void AtkinThreaded::calcPrimesForRange3(unsigned int begin, unsigned int end) {
	for(unsigned int x=begin; x<=end; x++) {
		const unsigned long xSquared = x*x;
		const unsigned long threeTimesXSquared = 3*xSquared;
		
		unsigned long n = 0;
		for(unsigned int y=0; y<=sqrtLimit; y++) {
			const unsigned long ySquared = y*y;
				
			n = threeTimesXSquared-ySquared;
			if (x > y && n < upperLimit && n%12 == 11) {
				g_lock.lock();
				primes[n] = !primes[n];
				g_lock.unlock();
			}
		}
	}
}

void AtkinThreaded::printPrimes() const {
	std::cout << "2 3 ";
	for(unsigned long i=5; i<upperLimit; i++) {
		if(primes[i]) {
			std::cout << i << " ";
		}
	}
    std::cout << std::endl;
}

void AtkinThreaded::printCount() const{
    int count = 2;
    for(unsigned long i=5; i<upperLimit; i++) {
		if(primes[i]) {
			++count;
		}
	}
    std::cout << "#primes = " << count << std::endl;
}
void AtkinThreaded::printTime() const{
	std::cout << "Time spent=" << (initDurationMs+lastCalcDurationMs) << " ms. (init=" << (initDurationMs) << "ms calc=" << (lastCalcDurationMs) << "ms)" << std::endl;
}

AtkinThreaded::operator bool() const{
    return false;
}
