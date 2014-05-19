#ifndef MILLERRABINWORKER_HPP
#define MILLERRABINWORKER_HPP

#include <unistd.h> // write
#include <cstdlib> // rand
#include <cstring> // strlen

#include <vector>

template<typename T>
class MillerRabinWorker final
{
	std::vector<T> mPrimeNumbers; // vector to store found primes
	const T mStart; // number to start
	const T mStop; // limit
	const size_t mTypeBitSize;
	const T* mBase;
	const size_t mNumberOfBases;
	size_t primes;

	bool check(const T, const T) const;
	T expModulo(const T, const T, const T) const;

	//MillerRabinWorker(const MillerRabinWorker&) = delete;
	const MillerRabinWorker& operator=(const MillerRabinWorker&) = delete;

	public:
		MillerRabinWorker(const T, const T, const T*, const size_t);
		void run();

		size_t getCount() const;
		void printPrimes() const;
};

/***********************************************************************
 * START OF WORKER IMPLEMENTATION                                      *
 **********************************************************************/
template<typename T>
MillerRabinWorker<T>::MillerRabinWorker(const T start, const T stop, const T* bases, const size_t numBases) :
	mPrimeNumbers(),
	mStart(start),
	mStop(stop),
	mTypeBitSize(sizeof (T) * 8),
	mBase(bases),
	mNumberOfBases(numBases),
	primes(0)
{
}

template<typename T>
T MillerRabinWorker<T>::expModulo(const T base, const T power, const T modulus) const
{
	T result = 1; // take care of the bitsize!!

	for (int i=(mTypeBitSize-1); i>=0; i--)
	{
		result = (result*result) % modulus;
		if (power & (1 << i))
		{
			result = (result*base) % modulus;
		}
	}

	return result;
}

template<typename T>
bool MillerRabinWorker<T>::check(const T base, const T pprime) const
{
	T a = base, s = 0, d = pprime - 1;

	// CAUTION: for this check d has to be > 1 !!!!
	while ((d % 2) == 0)
	{
		d /= 2;
		s++;
	}

	// fermat
	a = expModulo(base, d, pprime);
	if ((a == 1) || (a == pprime - 1))
	{
		return true;
	}

	for(T r=1; r < s; r++) {
		a = ((a * a) % pprime);
		if (a == 1)
		{
			return false;
		}
		else if (a == pprime-1)
		{
			return true;
		}
	}

	return false;
}

template<typename T>
void MillerRabinWorker<T>::run()
{
	char output[500];
	size_t dummy;
	sprintf(output, "mStart: %lu\n", mStart);
	dummy = write(STDOUT_FILENO, output, strlen(output));
	sprintf(output, "mStop: %lu\n", mStop);
	dummy = write(STDOUT_FILENO, output, strlen(output));

	bool isPrime = false;
	size_t mNumberOfBases = 7;
	size_t i = 0;

	mPrimeNumbers.clear();
	T n = mStart;
	// be sure we only check odd numbers
	if (n == 0)
	{
		mPrimeNumbers.push_back(2);
		n = 3;
	}
	else if (n % 2 == 0)
	{
		n++;
	}

	for (; n <= mStop; n+=2)
	{
		isPrime = false;
		i = 0;

		// iterate the bases
		do
		{
			isPrime = check(mBase[i]+mStart, n);
			i++;
		} while (i < mNumberOfBases && isPrime);

		if (isPrime) // n passed the check and seems to be prime
		{
			mPrimeNumbers.push_back(n);
		}
	}

	primes = mPrimeNumbers.size();
	sprintf(output, "thread terminating normaly, found %lu primes in the range: %lu - %lu\n", primes, mStart, mStop);
	dummy = write(STDOUT_FILENO, output, strlen(output));
	(void)dummy;
}

template<typename T>
size_t MillerRabinWorker<T>::getCount() const
{
	return primes;
}

/***********************************************************************
 * END OF WORKER IMPLEMENTATION                                        *
 **********************************************************************/

#endif
