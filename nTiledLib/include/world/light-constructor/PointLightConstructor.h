/*! @file PointLightConstructor.h
 *  @brief PointLightConstructor contains the definitions for a factory 
 *         object for PointLight
 */
#pragma once

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "world\World.h"
#include "world\light-constructor\LightConstructor.h"

namespace nTiled {
namespace world {

/*! @brief The PointLightConstructor is responsible for adding lights to the 
*          world it is constructed with.
 */
class PointLightConstructor : public LightConstructor {
public:
  /*! @brief Construct a new PointLightConstruct which will add 
   *         lights to the given world.
   * 
   * @param world The World to which this PointLightConstructor 
   *              adds lights.
   */
  PointLightConstructor(World& world);

  /*! @brief Construct a pointer to a new PointLight with the given parameters
   *         and add it to the world of this PointLightConstructor.
   * 
   * @param name The name of this new PointLight
   * @param position The position in world-coordinates of this new PointLight
   * @param intensity The colour intensity of the light emitted by this new 
   *                  new PointLight in rgb values between 0 and 1
   * @param radius The influence radius of this new PointLight
   * @param is_emitting Whether this new PointLight is emitting light
   */
  PointLight* add(const std::string& name,
                  glm::vec4 position,
                  glm::vec3 intensity,
                  float radius,
                  bool is_emitting);

private:
  /*! @brief the World to which this PointLightConstructor adds lights. */
  World& world;
};

} // world
} // nTiled
