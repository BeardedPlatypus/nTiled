#pragma once

#include <algorithm>

namespace nTiled {
namespace math {

/*! @brief Clamp x between the values smallest and largest. 
 * 
 * @param smallest The value which should be returned if x is smaller
 * @param x The value evaluated
 * @param largest The value which should be returned if x is larger
 *
 * @return The clamped value of x
 */
inline float clamp(float smallest, 
                   float x, 
                   float largest) {
  return std::max(smallest, std::min(x, largest));
}

}
}
