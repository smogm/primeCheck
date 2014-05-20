#include <millerrabinParallelBase.hpp>
#include <iostream>

MillerRabinParallelBase::MillerRabinParallelBase(std::condition_variable& cv, std::mutex& mutex, const unsigned long base) :
	mIsPrime(false),
	mWasSet(false),
	mKeepRunning(true),
	mCv(cv),
	mMutex(mutex),
	mResultCv(),
	mResultMutex(),
	mN(0),
	mBase(base)
{
	std::cout << "base: " << mBase << std::endl;
}

void MillerRabinParallelBase::setParams(unsigned long n)
{
	mN = n;
	mResultMutex.lock();
	mWasSet = true;
	mResultMutex.unlock();
}

bool MillerRabinParallelBase::getResult()
{
	std::unique_lock<std::mutex> lock(mResultMutex);
	while(mWasSet)
	{
		mResultCv.wait(lock);
	}

	return mIsPrime;
}

void MillerRabinParallelBase::termThread()
{
	mKeepRunning = false;
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

	while(mKeepRunning)
	{
		std::unique_lock<std::mutex> l1(mMutex);
		while (!mWasSet && mKeepRunning)
		{
			mCv.wait(l1);
		}
		l1.unlock();

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
		mResultMutex.lock();
		mWasSet = false;
		mResultCv.notify_one(); // unblocks getResult()
		mResultMutex.unlock();
	}
}
