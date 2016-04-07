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

struct PointLight {
public:
  // --------------------------------------------------------------------------
  //  Constructor
  // --------------------------------------------------------------------------
  PointLight(const std::string& name,
             glm::vec4 position,
             glm::vec3 intensity,
             float radius,
             bool is_emitting,
             std::map<std::string, Object*> debug_light_objects);

  // --------------------------------------------------------------------------
  //  Variables
  // --------------------------------------------------------------------------
  /*! name of this PointLight */
  const std::string name;
  /*! position of this PointLight in world coordinates */
  glm::vec4 position;
  /*! Colour intensity of this PointLight in rgb values */
  glm::vec3 intensity;
  /*! The radius of the light sphere of this point light*/
  float radius;
  /*! Wtether this light is emitting */
  bool is_emitting;

  // --------------------------------------------------------------------------
  //  Debug attributes
  // --------------------------------------------------------------------------
  /*! Map of objects to visualise this PointLight in debug mode */
  std::map<std::string, Object*> debug_light_objects;
};

} // world
} // nTiled

