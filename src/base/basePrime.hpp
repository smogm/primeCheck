#ifndef BASEPRIME_HPP
#define BASEPRIME_HPP

class BasePrime
{
	public:
			virtual ~BasePrime() {};

			// implement as explicit to be safe-bool !!!
			virtual operator bool() const = 0;
};

#endif
