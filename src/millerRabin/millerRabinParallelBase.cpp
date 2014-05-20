#include <millerrabinParallelBase.hpp>
#include <iostream>

MillerRabinParallelBase::MillerRabinParallelBase(const unsigned long base) :
	mIsPrime(false),
	mWasSet(false),
	mHasResult(false),
	mKeepRunning(true),
	mParamCv(),
	mParamMutex(),
	mResultCv(),
	mResultMutex(),
	mN(0),
	mBase(base)
{
	std::cout << "base: " << mBase << std::endl;
}

void MillerRabinParallelBase::setParams(unsigned long n)
{
	//mParamMutex.lock();
	mN = n;
	mWasSet = true;
	//mParamMutex.unlock();
	mParamCv.notify_one();
}

bool MillerRabinParallelBase::getResult()
{
	std::unique_lock<std::mutex> lock(mResultMutex);
	while(!mHasResult)
	{
		mResultCv.wait(lock);
	}
	mHasResult = false;

	return mIsPrime;
}

void MillerRabinParallelBase::termThread()
{
	mKeepRunning = false;
	mParamCv.notify_one();
}

unsigned long MillerRabinParallelBase::expModulo(const unsigned long base, const unsigned long power, const unsigned long modulus) const
{
	unsigned long result = 1; // take care of the bitsize!!

	for (int i=(64-1); i>=0; i--)
	{
		result = (result*result) % modulus;
		if (power & (1 << i))
		{
			result = (result*base) % modulus;
		}
	}

	return result;
}

void MillerRabinParallelBase::run()
{
	unsigned long a, s, d;
	//std::vector<unsigned long> primes;

	while(mKeepRunning)
	{
		std::unique_lock<std::mutex> l1(mParamMutex);
		while (!mWasSet && mKeepRunning)
		{
			mParamCv.wait(l1);
		}

		if (!mKeepRunning) // double check
		{
			return;
		}

		a = mBase;
		s = 0;
		d = mN - 1;

		// CAUTION: d has to be > 1 !!!!
		while ((d % 2) == 0)
		{
			d /= 2;
			s++;
		}

		// fermat
		a = expModulo(mBase, d, mN);
		if ((a == 1) || (a == mN - 1))
		{
			mIsPrime = true;
			goto cont;
		}

		for(unsigned long r=1; r < s; r++)
		{
			a = ((a * a) % mN);
			if (a == 1)
			{
				mIsPrime = false;
				goto cont;
			}
			else if (a == mN-1)
			{
				mIsPrime = true;
				goto cont;
			}
		}

		mIsPrime = false;
cont:
		mWasSet = false; // release setParams()
		mResultMutex.lock();
		mHasResult = true;
		mResultMutex.unlock();
		mResultCv.notify_one(); // unblocks getResult()
	}
}
