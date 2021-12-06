
#include <time.h>

#include "math.h"

void math::genRanN(int amount, int range, std::vector<float>& v)
{
	std::random_device rd;
	std::mt19937::result_type seed = rd() ^ (
		(std::mt19937::result_type)
		std::chrono::duration_cast<std::chrono::seconds>(
			std::chrono::system_clock::now().time_since_epoch()
			).count() +
		(std::mt19937::result_type)
		std::chrono::duration_cast<std::chrono::microseconds>(
			std::chrono::high_resolution_clock::now().time_since_epoch()
			).count());

	std::mt19937 gen(seed);
	std::uniform_int_distribution<unsigned> distrib(1, range);

	for (unsigned long j = 0; j < amount; ++j)
	{
		v.push_back((float)distrib(gen) / (float)range);
	}

	std::cout << '\n';
}

math::Random::Random(void)
{
	rseed = 1;
	// safe0 start
	rseed_sp = 0;
	mti = MATH_N + 1;
	// safe0 end
}

// Returns a number from 0 to n (excluding n)
unsigned int math::Random::randomUInt(unsigned int n)
{
	unsigned long y;
	static unsigned long mag01[2] = { 0x0, MATH_MATRIX_A };

	if (n == 0)
		return(0);

	/* mag01[x] = x * MATRIX_A  for x=0,1 */

	if (mti >= MATH_N) { /* generate N words at one time */
		int kk;

		if (mti == MATH_N + 1)   /* if sgenrand() has not been called, */
			setRandomSeed(4357); /* a default initial seed is used   */

		for (kk = 0; kk < MATH_N - MATH_M; kk++) {
			y = (mt[kk] & MATH_UPPER_MASK) | (mt[kk + 1] & MATH_LOWER_MASK);
			mt[kk] = mt[kk + MATH_M] ^ (y >> 1) ^ mag01[y & 0x1];
		}
		for (; kk < MATH_N - 1; kk++) {
			y = (mt[kk] & MATH_UPPER_MASK) | (mt[kk + 1] & MATH_LOWER_MASK);
			mt[kk] = mt[kk + (MATH_M - MATH_N)] ^ (y >> 1) ^ mag01[y & 0x1];
		}
		y = (mt[MATH_N - 1] & MATH_UPPER_MASK) | (mt[0] & MATH_LOWER_MASK);
		mt[MATH_N - 1] = mt[MATH_M - 1] ^ (y >> 1) ^ mag01[y & 0x1];

		mti = 0;
	}

	y = mt[mti++];
	y ^= MATH_TEMPERING_SHIFT_U(y);
	y ^= MATH_TEMPERING_SHIFT_S(y) & MATH_TEMPERING_MASK_B;
	y ^= MATH_TEMPERING_SHIFT_T(y) & MATH_TEMPERING_MASK_C;
	y ^= MATH_TEMPERING_SHIFT_L(y);

	// ET - old engine added one to the result.
	// We almost NEVER wanted to use this function
	// like this.  So, removed the +1 to return a 
	// range from 0 to n (not including n).
	return (y % n);
}


float math::Random::randomFloat()
{
	float r = (float)randomUInt(MAXUINT);
	float divisor = (float)MAXUINT;
	return (r / divisor);
}



void math::Random::setRandomSeed(unsigned int n)
{
	/* setting initial seeds to mt[N] using         */
	/* the generator Line 25 of Table 1 in          */
	/* [KNUTH 1981, The Art of Computer Programming */
	/*    Vol. 2 (2nd Ed.), pp102]                  */
	mt[0] = n & 0xffffffff;
	for (mti = 1; mti < MATH_N; mti++)
		mt[mti] = (69069 * mt[mti - 1]) & 0xffffffff;

	rseed = n;
}

unsigned int math::Random::getRandomSeed(void)
{
	return(rseed);
}

void math::Random::randomize(void)
{
	setRandomSeed((unsigned int)time(NULL));
}