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
	bool mKeepRunning;
	std::condition_variable& mCv;
	std::mutex& mMutex;
	std::condition_variable mResultCv;
	std::mutex mResultMutex;
	unsigned long mN;
	const unsigned long mBase;

	public:
			MillerRabinParallelBase(std::condition_variable&, std::mutex&, const unsigned long base);
			void setParams(unsigned long n);
			bool getResult(); // not const because of mutex locking
			void termThread();

			unsigned long expModulo(const unsigned long base, const unsigned long power, const unsigned long modulus) const;

			void run();
};

#endif
