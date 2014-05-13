#include <stdio.h>
#include <limits.h>
#include <stdbool.h>
#include <math.h> 

#include <atkin.hpp>

// TODO build class (see atkin.hpp)
// TODO check for performance issues

#define LIMIT 2500 //ULLONG_MAX
#define LIMITSQRT 50 // ULONG_MAX

bool primes[LIMIT];

void initPrimes() { // TODO move to c'tor
	for(unsigned long i=0; i<LIMIT; i++) {
		primes[i] = false;
	}
}

void calcPrimes() { // interface method 1
	unsigned long n = 0;
	for(unsigned int x=0; x<LIMITSQRT; x++) {
		for(unsigned int y=0; y<LIMITSQRT; y++) {
			n = 4*x*x+y*y;
			if (n < LIMIT && (n%12 == 1 || n%12 == 5)) {
				primes[n] = !primes[n];
			}
			
			n = 3*x*x+y*y;
			if (n < LIMIT && n%12 == 7) {
				primes[n] = !primes[n];
			}
			
			
			n = 3*x*x-y*y;
			if (x > y && n < LIMIT && n%12 == 11) {
				primes[n] = !primes[n];
			}
		}
	}
	
	for(unsigned int n=5; n<LIMITSQRT; n++) {
		if(primes[n]) {
			for(unsigned long i=0, j=1; i<LIMIT; i=(j*pow(n, 2.0)), j++) {
				primes[i] = false;
			}
		}
	}
}

void printPrimes() { // interface method 2
	printf("%i %i ", 2, 3);
	for(unsigned long i=5; i<LIMIT; i++) {
		if(primes[i]) {
			printf("%lu ", i);
		}
	}
}

int main(int argc, char** argv) {
	initPrimes();
	calcPrimes();
	printPrimes();
	return 0;
}
