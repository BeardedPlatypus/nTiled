#pragma once

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "world\World.h"
#include "world\light-constructor\LightConstructor.h"

namespace nTiled {
namespace world {

class PointLightConstructor : public LightConstructor {
public:
  PointLightConstructor(World& world);

  PointLight* add(const std::string& name,
                  glm::vec4 position,
                  glm::vec3 intensity,
                  float radius,
                  bool is_emitting);

private:
  World& world;
};



}
}
