#include "world\PointLight.h"

namespace nTiled {
namespace world {

PointLight::PointLight(const std::string& name,
                       glm::vec4 position,
                       glm::vec3 intensity,
                       float radius,
                       bool is_emitting,
                       std::map<std::string, Object*> debug_light_objects) :
    name(name),
    position(position),
    intensity(intensity),
    radius(radius),
    is_emitting(is_emitting),
    debug_light_objects(debug_light_objects) { }

} // world
} // nTiled