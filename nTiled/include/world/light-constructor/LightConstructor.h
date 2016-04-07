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

class LightConstructor {
public:
  virtual ~LightConstructor() {}
  virtual PointLight* add(const std::string& name,
                          glm::vec4 position,
                          glm::vec3 intensity,
                          float radius,
                          bool is_emitting) = 0;
};

}
}
