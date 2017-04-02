/*! @file util.h
 *  @brief util.h contains utility math functions
 */
#pragma once


#include <glm/glm.hpp>

namespace nTiled {
namespace math {

/*! @brief Constant value of pi. */
constexpr double kPi = 3.141592653589793238462643383279502884197169399375105820974;

/*! @brief Convert the given value in degrees to radians.
 *  
 *  @param x The value in degrees to be converted to radians.
 *  @return The value of x in radians.
 */
inline double to_radians(double x) { return (x * kPi) / 180.0; }

/*! @brief Convert the given value in radians to degrees.
 * 
 * @param x The value in radians to be converted in degrees.
 * @return The value of x in degrees.
 */
inline double to_degrees(double x) { return (x * (180 / kPi)); }

// http://stackoverflow.com/questions/1322510/given-an-integer-how-do-i-find-the-next-largest-power-of-two-using-bit-twiddlin
/*! @brief Get the next power of 2 of the given unsigned int.
 * 
 * @param x the input unsigned integer.
 * 
 * @return the smallest power of 2 that is greater or equal to the given 
 *         integer.
 */
inline unsigned int getNextPow2(unsigned int x) {
  --x;
  x |= x >> 1;   // Divide by 2^k for consecutive doublings of k up to 32,
  x |= x >> 2;   // and then or the results.
  x |= x >> 4;
  x |= x >> 8;
  x |= x >> 16;
  ++x;
  return x;
}

inline unsigned int gcd(unsigned int a, unsigned int b) {
  unsigned int t;
  while (b != 0) {
    t = b;
    b = a % b;
    a = t;
  }
  return a;
}


/*! @brief Convert point p to an index within a single array given 
 *          dimension dim.
 * 
 * @param p A 3d point which should be converted to a single 
 *          index within an array.
 * @param dim The dimension in which the index resides.
 * 
 * @returns The index within the one dimensional array that corresponds with
 *          p
 */
inline unsigned int toIndex(glm::uvec3 p,
                            unsigned int dim) {
  return p.x + p.y * dim + p.z * dim * dim;
}


/*! @brief Extract the kth bit from val 
 * 
 * @param val The value of which the bit is extracted
 * @param k_bit The index of the bit to be extracted
 *
 * @returns the kth bit of val as a boolean value.
 */
inline bool extractBit(glm::u8 val, unsigned int k_bit) {
  return ((val & (1 << k_bit)) >> k_bit) == 1;
}

} // math
} // nTiled
