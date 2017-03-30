#pragma once

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <glm\glm.hpp>
#include <algorithm>


// ----------------------------------------------------------------------------
//  Functions related to points
// ----------------------------------------------------------------------------
namespace nTiled {
namespace math {

inline double f_max(double a, double b) {
  if (a > b) return a;
  else       return b;
}


inline double f_min(double a, double b) {
  if (a < b) return a;
  else       return b;
}

/*! @brief clamp value x between the smallest and largest value
 *
 * @param smallest The lower bound to which the value x is clamped
 * @param x The value that is being clamped
 * @param largest The upper bound to which the value x is clamped
 *
 * @returns the clamped value of x
 */
inline double clamp(double smallest,
                    double x,
                    double largest) {
  return f_max(smallest, f_min(x, largest));
}


/*! @brief Get the point between the lower and upper bound that is closest to
 *         the goal.
 *
 * @param lower_bound The lowerbound of the cube within the closest point is
 *                    calculated.
 * @param goal The goal value to which the closest point is calculated.
 * @param upper_bound The upperbound of the cube within the closest point is
 *                    calculated.
 */
inline glm::vec3 getClosestPoint(glm::vec3 lower_bound,
                                 glm::vec3 goal,
                                 glm::vec3 upper_bound) {
  return glm::vec3(clamp(lower_bound.x, goal.x, upper_bound.x),
                   clamp(lower_bound.y, goal.y, upper_bound.y),
                   clamp(lower_bound.z, goal.z, upper_bound.z));
}


/*! @brief Get the furthest point away from x, between lower_bound and
 *         upper_bound.
 *
 * @param lower_bound The lower bound value of which the maximum distance
 *                    to x is found
 * @param x The value of which the furthest point is found
 * @param upper_bound The upper bound value of which the maximum distance to x
 *                    is found
 *
 * @returns The furthest value from x between lower and upper bound.
 */
inline double getFurthest(double lower_bound,
                          double x,
                          double upper_bound) {
  if (abs(lower_bound - x) > abs(upper_bound - x)) return lower_bound;
  else                                             return upper_bound;
}


/*! @brief Get the furthest point from goal between lower_bound and upper_bound
 *
 * @param lower_bound the smallest value of the cube in which the furthest point
 *                    is found.
 * @param goal The goal point, of which the furthest point within the cube is
 *             retrieved.
 * @param upper_bound The greatest value of the cube in which the furthest point
 *                    is found.
 *
 * @returns The furthest point from goal between lower_bound and upper_bound
 */
inline glm::vec3 getFurthestPoint(glm::vec3 lower_bound,
                                  glm::vec3 goal,
                                  glm::vec3 upper_bound) {
  return glm::vec3(getFurthest(lower_bound.x, goal.x, upper_bound.x),
                   getFurthest(lower_bound.y, goal.y, upper_bound.y),
                   getFurthest(lower_bound.z, goal.z, upper_bound.z));
}


/*! @brief Calculate whether the specified point falls within the specified
 *         sphere
 *
 * @param point The point which is tested
 * @param sphere_origin The origin of the sphere
 * @param sphere_radius The radius of the sphere
 *
 * @return True if the point falls within the sphere, false otherwise.
 */
inline bool pointInSphere(glm::vec3 point,
                          glm::vec3 sphere_origin,
                          double sphere_radius) {
  glm::vec3 p = point - sphere_origin;
  return ((p.x * p.x + p.y * p.y + p.z * p.z) < (sphere_radius * sphere_radius));
}


} // math
} // nTiled