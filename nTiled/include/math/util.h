#pragma once

namespace nTiled {
namespace math {

constexpr double kPi = 3.141592653589793238462643383279502884197169399375105820974;
/*! 
 * Convert the given value in degrees to radians
 */
inline double to_radians(double x) { return (x * kPi) / 180.0; }

/*!
 * Convert the given value in radians to degrees
 */
inline double to_degrees(double x) { return (x * (180 / kPi)); }

// http://stackoverflow.com/questions/1322510/given-an-integer-how-do-i-find-the-next-largest-power-of-two-using-bit-twiddlin
/*!
 * Get the next power of 2 of the given unsigned int.
 * 
 * param: 
 *     x (unsigned int): the input unsigned integer
 * 
 * return:
 *     the smallest power of 2 that is greater or equal to the given 
 *     integer
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


} // math
} // nTiled
