/*! @file PointLight.h
 *  @brief The PointLight.h contains the definition of the PointLight struct.
 */
#pragma once

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <glm\glm.hpp>

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "world\Object.h"

namespace nTiled {
namespace world {

/*! @Brief PointLight defines a single point light, with a given colour
 *         intensity, position and radius.
 */
struct PointLight {
public:
  // --------------------------------------------------------------------------
  //  Constructor
  // --------------------------------------------------------------------------
  /*! @brief Construct a new PointLight with the given parameters
   * 
   * @param name The name of this new PointLight
   * @param position The position in world coordinates of this new PointLight
   * @param intensity The intensity of this new PointLight in rgb values 
   *                  between [0..1]
   * @param radius The influence radius of this new PointLight.
   * @param is_emitting Whether this new PointLight is emitting light.
   * @param debug_light_objects The objects used to display the light when 
   *                            in debug mode.
   */
  PointLight(const std::string& name,
             glm::vec4 position,
             glm::vec3 intensity,
             float radius,
             bool is_emitting,
             std::map<std::string, Object*> debug_light_objects);

  // --------------------------------------------------------------------------
  //  Variables
  // --------------------------------------------------------------------------
  /*! @brief The name of this PointLight. */
  const std::string name;
  /*! @brief The position of this PointLight in world coordinates. */
  glm::vec4 position;
  /*! @brief The colour intensity of this PointLight in rgb values. */
  glm::vec3 intensity;
  /*! @brief The radius of the light sphere of this point light*/
  float radius;
  /*! @brief Whether this light is emitting */
  bool is_emitting;

  // --------------------------------------------------------------------------
  //  Debug attributes
  // --------------------------------------------------------------------------
  /*! @brief Map of objects to visualise this PointLight in debug mode */
  std::map<std::string, Object*> debug_light_objects;
};

} // world
} // nTiled