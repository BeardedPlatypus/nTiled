#include "world\light-constructor\PointLightConstructor.h"

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------

namespace nTiled {
namespace world {

PointLightConstructor::PointLightConstructor(World& world) : world(world) { }

PointLight* PointLightConstructor::add(const std::string& name,
                                       glm::vec4 position,
                                       glm::vec3 intensity,
                                       float radius,
                                       bool is_emitting) {
  std::map<std::string, Object*> debug_objects = std::map<std::string,
                                                          Object*>();

  PointLight* p_light = this->world.constructPointLight(name,
                                                        position,
                                                        intensity,
                                                        radius,
                                                        is_emitting,
                                                        debug_objects);
  return p_light;
}


} // world
} // nTiled