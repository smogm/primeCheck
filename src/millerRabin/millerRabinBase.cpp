#include <millerRabinBase.hpp>

#if defined(__x86_64__)
unsigned long MillerRabinBase::mul(unsigned long a, unsigned long factor, unsigned long modulus) const
{
	/*	resulting code:
	 *    	48 89 f0             	mov    %rsi,%rax
	 *    	48 f7 e2             	mul    %rdx
	 *    	48 f7 f1             	div    %rcx
	 *    	48 89 d0             	mov    %rdx,%rax
	 *    	c3                  	retq
	 */
	unsigned long q; // q = ⌊a b / m⌋
	unsigned long r; // r = a b mod m
	asm volatile(
	  "mulq %[factor];" // quad word multiplication rax = rax * factor
	  "divq %[modulus];"
	  : "=a"(q) /*rax output*/, "=d"(r) /*rdx output*/
	  : "a"(a) /*rax input*/, [factor]"rm"(factor), [modulus]"rm"(modulus));
	return r;
}

unsigned long MillerRabinBase::expModulo(const unsigned long base, const unsigned long power, const unsigned long modulus) const
{
   unsigned long r = 1;
   unsigned long e = power;
   unsigned long a = base;
   for(; e; e >>= 1) {
      if(e & 1)
         r = mul(r, a, modulus);
      a = mul(a, a, modulus);
   }
   return r;
}
#else
#warning "not using asm multiplication!"
unsigned long MillerRabinBase::mul(unsigned long a, unsigned long factor, unsigned long modulus) const
{
	/*	resulting code:
	 *    	48 0f af f2          	imul   %rdx,%rsi
	 *    	31 d2                	xor    %edx,%edx
	 *    	48 89 f0             	mov    %rsi,%rax
	 *    	48 f7 f1             	div    %rcx
	 *    	48 89 d0             	mov    %rdx,%rax
	 *    	c3                   	retq
	 */
	return a * factor % modulus;
}

unsigned long MillerRabinBase::expModulo(const unsigned long base, const unsigned long power, const unsigned long modulus) const
{
	unsigned long result = 1; // take care of the bitsize!!

	for (int i=(sizeof(unsigned long)*8 -1); i>=0; i--)
	{
		result = (result*result) % modulus;
		if (power & (1 << i))
		{
			result = (result*base) % modulus;
		}
	}

	return result;
}
#endif

bool MillerRabinBase::check(const unsigned long base, const unsigned long pprime) const
{
	unsigned long a = base, s = 0, d = pprime - 1;
	while ((d % 2) == 0)
	{
		d /= 2;
		s++;
	}

	// fermat
	//a = expModulo(base, d, pprime);
	a = expModulo(base % pprime, d, pprime);
	if ((a == 1) || (a == pprime - 1))
	{
		return true;
	}

	while(s-- > 1)
	{
		//a = (a * a % pprime);
		a = mul(a, a, pprime);
		if (a == 1)
		{
			return false;
		}
		
		if (a == pprime-1)
		{
			return true;
		}
	}

	return false;
}
