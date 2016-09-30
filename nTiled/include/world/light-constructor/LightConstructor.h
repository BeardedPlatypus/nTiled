/*! @file LightConstructor.h
 *  @brief LightConstructor.h contains the interface that should be used 
 *         by all LightConstructor within nTiled.
 */
#pragma once

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <string>
#include <glm\glm.hpp>


// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "world\PointLight.h"


namespace nTiled {
namespace world {

/*! @brief LightConstructor is the interface which any LightConstructors 
 *         should use within nTiled.
 */
class LightConstructor {
public:

  /*! @brief virtual destructor */
  virtual ~LightConstructor() {}

  /*! @brief Construct a pointer to a new PointLight with the given parameters
   * 
   * @param name The name of this new PointLight
   * @param position The position in world coordinates of this new PointLight
   * @param intensity The intensity of this new PointLight in rgb values [0, 1]
   * @param radius The influence radius of this new PointLight
   * @param is_emitting Wether this new PointLight is emitting any light
   */
  virtual PointLight* add(const std::string& name,
                          glm::vec4 position,
                          glm::vec3 intensity,
                          float radius,
                          bool is_emitting) = 0;
};

} // world
} // nTiled