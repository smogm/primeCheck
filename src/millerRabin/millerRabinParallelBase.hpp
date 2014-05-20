#ifndef MILLERRABINPARALLELBASE_HPP
#define MILLERRABINPARALLELBASE_HPP

#include <unistd.h> // write
#include <cstdlib> // rand
#include <cstring> // strlen

#include <condition_variable>
#include <mutex>

class MillerRabinParallelBase final
{
	bool mIsPrime;
	bool mWasSet;
	bool mHasResult;
	bool mKeepRunning;
	std::condition_variable mParamCv;
	std::mutex mParamMutex;
	std::condition_variable mResultCv;
	std::mutex mResultMutex;
	unsigned long mN;
	const unsigned long mBase;

	public:
			MillerRabinParallelBase(const unsigned long base);
			void setParams(unsigned long n);
			bool getResult(); // not const because of mutex locking
			void termThread();

			unsigned long expModulo(const unsigned long base, const unsigned long power, const unsigned long modulus) const;

			void run();
};

#endif
