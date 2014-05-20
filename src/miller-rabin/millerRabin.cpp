#include <iostream>
#include <future>

#include <millerRabin.hpp>
#include <millerRabinParallelBase.hpp>

#include <cstdlib> // rand

MillerRabin::MillerRabin(unsigned long n, unsigned long numberOfBases) :
	BasePrime("Miller-Rabin"),
	mIsValid(false),
	mCheckLimit(n),
	mNumberOfBases(numberOfBases),
	mTypeBitSize(sizeof(unsigned long)*8), // !!!!!!!!
	mNumberOfThreads(getCoreCount()+1),
	mBase(nullptr),
	mWorker(nullptr),
	mPrimeList()
{
	std::cout << "available cores for concurrent jobs: " << getCoreCount() << std::endl;
	std::cout << "check against " << mNumberOfBases << " bases" << std::endl;
    std::cout << "ulong size: " << mTypeBitSize << " bits" << std::endl << std::endl;

	// TODO: check a condition for doing concurrent jobs?
	mWorker = new MillerRabinWorker<unsigned long>*[mNumberOfThreads];

	// nothing to do yet?
	if (n > 2 && numberOfBases > 2)
	{
		mIsValid = true; // object created correctly
	}
}

MillerRabin::~MillerRabin()
{
	if (mWorker)
	{
		for (size_t i = 0; i < mNumberOfThreads; i++)
		{
			// TODO
			delete mWorker[i];
		}
		delete[] mWorker;
	}
	delete[] mBase;
}

MillerRabin::operator bool() const
{
	return mIsValid;
}

void MillerRabin::printPrimes() const
{
	for (auto prime : mPrimeList)
	{
		std::cout << prime << "\tis probably prime" << std::endl;
	}

	std::cout << std::endl;
}

void MillerRabin::printCount() const
{
	std::cout << "found " << mPrimeList.size() << " probably prime numbers." << std::endl;
}

void MillerRabin::printTime() const
{
	std::cout << "MillerRabin took " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms." << std::endl;
}

/*
 * parallelize bases
 */
void MillerRabin::calcPrimesParallelBase()
{
	std::future<std::vector<unsigned long>*> f[mNumberOfBases];
	std::vector<unsigned long>* primeArray[mNumberOfBases];

	start = std::chrono::steady_clock::now();
	for (size_t i = 0; i < mNumberOfBases; i++)
	{
		unsigned long base = rand() % (mCheckLimit - 1) + 1;
		f[i] = std::async(std::launch::async, [=]() {
			std::vector<unsigned long>* primes = new std::vector<unsigned long>;

			for (unsigned int n = 3; n < mCheckLimit; n+=2)
			{
				if (check(base, n))
				{
					//primes->push_back(n);
				}
			}

			return primes;
		});
	}

	for (size_t i = 0; i < mNumberOfBases; i++)
	{
		primeArray[i] = f[i].get();
		std::cout << "found " << primeArray[i]->size() << " primes" << std::endl;
		delete primeArray[i];
	}
	end = std::chrono::steady_clock::now();
	std::cout << "calculation finished in " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms, stopping threads..." << std::endl;
	
	/*std::thread** thread = new std::thread*[mNumberOfBases];
	MillerRabinParallelBase** mrParallel = new MillerRabinParallelBase*[mNumberOfBases];
	size_t primes = 0;
	size_t b = 0;
	bool isPrime = false;

	// prepare bases:
	srand(mNumberOfBases);
	// start the threads:
	for (size_t i = 0; i < mNumberOfBases; i++)
	{
		mrParallel[i] = new MillerRabinParallelBase(rand() % (mCheckLimit - 1) + 1);
		thread[i] = new std::thread(&MillerRabinParallelBase::run, mrParallel[i]);
		// threads should initialize, start and immediately going to sleep
	}
    std::this_thread::sleep_for(std::chrono::milliseconds(200)); // be sure threads are ready

	primes = 10; // for prime numbers 2 ... 29
	start = std::chrono::steady_clock::now();
	for (unsigned long n = 19; n <= mCheckLimit; n+=2)
	{
		// load all threads:
		for (b = 0; b < mNumberOfBases; b++)
		{
			mrParallel[b]->setParams(n);
		}

		isPrime = true;
		b = 0;
		do
		{
			if (!mrParallel[b]->getResult()) // this blocks!
			{
				isPrime = false;
			}

			b++;
		} while (b < mNumberOfBases);

		if (isPrime)
		{
			primes++;
		}
	}
	end = std::chrono::steady_clock::now();

	std::cout << "calculation finished in " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms, stopping threads..." << std::endl;
	// terminate threads and clean up:
	for (size_t i = 0; i < mNumberOfBases; i++)
	{
		mrParallel[i]->termThread();
	}

	for (size_t i = 0; i < mNumberOfBases; i++)
	{
		thread[i]->join();
		delete mrParallel[i];
		delete thread[i];
	}

	delete[] thread;
	delete[] mrParallel;

	std::cout << "found primes: " << primes << std::endl;*/
}

void MillerRabin::calcPrimeParallel()
{
	if (mNumberOfThreads > 1) {
		std::thread** thread = new std::thread*[mNumberOfThreads];
		size_t blockSize = (mCheckLimit / mNumberOfThreads);
		size_t remaining = (mCheckLimit - (blockSize * mNumberOfThreads));

		unsigned long low = 0, high = blockSize;
		size_t t = 0;

		// prepare bases:
		for (size_t i = 0; i < mNumberOfBases; i++)
		{
			srand(i);
			mBase[i] = (rand() % (blockSize - 1)) + 1;
			std::cout << "base: " << mBase[i] << std::endl;
		}

		start = std::chrono::steady_clock::now();
		do
		{
			mWorker[t] = new MillerRabinWorker<unsigned long>(low, high, mBase, mNumberOfBases);
			thread[t] = new std::thread(&MillerRabinWorker<unsigned long>::run, mWorker[t]);

			t++;
			// prepare for next loop:
			low = high + 1;
			high = low + blockSize -1;

			if (t == (mNumberOfThreads - 1)) // is this the next thread the last thread?
			{
				// if it's the last, give it the remaining numbers
				high += remaining;
			}
		} while(t < mNumberOfThreads);

		std::cout << "waiting for threads..." << std::endl;
		for (size_t i = 0; i < mNumberOfThreads; i++)
		{
			thread[i]->join();
			std::cout << "thread returned!" << std::endl;
		}
		end = std::chrono::steady_clock::now();
	}
	else
	{
		// prepare bases:
		for (size_t i = 0; i < mNumberOfBases; i++)
		{
			srand(i);
			mBase[i] = (rand() % (mCheckLimit - 1)) + 1;
		}

		std::cout << "running single threaded!" << std::endl;
		mWorker[0] = new MillerRabinWorker<unsigned long>(0, mCheckLimit, mBase, mNumberOfBases);
		start = std::chrono::steady_clock::now();
		mWorker[0]->run();
		end = std::chrono::steady_clock::now();
		// TODO...
	}
}

void MillerRabin::calcPrimes()
{
	calcPrimesParallelBase();
	mPrimeList.clear();

	// TODO: check whether mCheckLimit is greater than this:
	mPrimeList.push_back(2);
	mPrimeList.push_back(3);
	mPrimeList.push_back(5);
	mPrimeList.push_back(7);
	mPrimeList.push_back(11);
	mPrimeList.push_back(13);
	mPrimeList.push_back(17);
	mPrimeList.push_back(19);
	mPrimeList.push_back(23);
	mPrimeList.push_back(29); // <--

	start = std::chrono::steady_clock::now();

	// iterater all numbers we want to check
#if defined(USE_OPENMP)
#pragma omp parallel for
#endif
    for (unsigned long n = mPrimeList[mPrimeList.size()-1]; n <= mCheckLimit; n+=2)
	{
        // inner declaration makes it private for each thread
        bool isPrime = true;

        if ((n % 3  != 0) && (n % 5  != 0) && (n % 7  != 0) && (n % 11 != 0) &&
            (n % 13 != 0) && (n % 17 != 0) && (n % 19 != 0) && (n % 23 != 0) && (n %29 != 0))
		{

			// iterate the bases
            #if defined(USE_OPENMP)
            #pragma omp parallel for
            #endif
            for (unsigned long i = 0; i < mNumberOfBases; i++)
			{
                unsigned long base = 0;
                bool running = true;
                bool isPrimeToBase;

                #if defined(USE_OPENMP)
                #pragma omp critical(isPrimeVariable)
                #endif
                {
                    // we use isPrime as indicator if we should go ahead with our base checks
                    // if one thread found that n is not prime, running will be false
                    // so all other threads would simply go through the remaining loop iterations without doing any work
                    running = isPrime;
                }

                if (running)
                {
                    srand(i);
                    base = (rand() % (n - 1)) + 1;

                    isPrimeToBase = check(base, n);

                    if (isPrimeToBase == false)
                    {
                        #if defined(USE_OPENMP)
                        #pragma omp critical(isPrimeVariable)
                        #endif
                        {
                            #if defined(DEBUG)
                            std::cout << n << " is not prime to base " << base << " so, we're setting isPrime = false / base iteration i was: " << i << std::endl;
                            #endif

                            isPrime = false;
                        }

                    }
                }
                else
                {
                    #if defined(DEBUG)
                    std::cout << n << " was already found not prime, so we're don't do any check in base iteration " << i << std::endl;
                    #endif
                }

            }

			if (isPrime) // n passed the check and seems to be prime
			{
				// lock for thread safety
				mPrimeList.push_back(n);
			}
		}
	}
	end = std::chrono::steady_clock::now();
}
