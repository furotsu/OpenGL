#pragma once

#include "glm/gtc/type_ptr.hpp"

#include <iostream>
#include <chrono>
#include <random>
#include <vector>

constexpr auto MATH_PI = 3.14159265358979323846;
constexpr auto MATH_PI_2 = 1.57079632679489661923;
constexpr auto MATH_PI_4 = 0.785398163397448309616;

constexpr auto MAXUINT = 4294967295;

/* Period parameters */
constexpr auto MATH_N = 624;
constexpr auto MATH_M = 397;
constexpr auto MATH_MATRIX_A = 0x9908b0df;   /* constant vector a */
constexpr auto MATH_UPPER_MASK = 0x80000000; /* most significant w-r bits */
constexpr auto MATH_LOWER_MASK = 0x7fffffff;/* least significant r bits */

/* Tempering parameters */
#define MATH_TEMPERING_MASK_B 0x9d2c5680
#define MATH_TEMPERING_MASK_C 0xefc60000
#define MATH_TEMPERING_SHIFT_U(y)  (y >> 11)
#define MATH_TEMPERING_SHIFT_S(y)  (y << 7)
#define MATH_TEMPERING_SHIFT_T(y)  (y << 15)
#define MATH_TEMPERING_SHIFT_L(y)  (y >> 18)

#define RADIANS_TO_DEGREES(x) ((x) * 180.0f / MATH_PI)
#define DEGREES_TO_RADIANS(x) ((x) * MATH_PI / 180.0f)

namespace math
{
    class Random
    {
    private:
        // DATA
        unsigned int rseed;
        unsigned int rseed_sp;
        unsigned long mt[MATH_N]; /* the array for the state vector  */
        int mti; /* mti==N+1 means mt[N] is not initialized */

        // FUNCTIONS
    public:
        Random(void);

        unsigned int	randomUInt(unsigned int n);
        float			randomFloat();
        void			setRandomSeed(unsigned int n);
        unsigned int	getRandomSeed(void);
        void			randomize(void);
    };

    class Ray
    {
        glm::vec3 m_position;
        glm::vec3 m_direction;
        
    public:
        Ray();
        Ray(glm::vec3 position, glm::vec3 direction);
    };

    // generates amount of normalized random numbers: 0 < r < 1
    void genRanN(int amount, int range, std::vector<float>& v);
}
