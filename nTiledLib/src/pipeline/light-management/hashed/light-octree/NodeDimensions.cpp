#include "pipeline\light-management\hashed\light-octree\NodeDimensions.h"

namespace nTiled {
namespace pipeline {
namespace hashed {

NodeDimensions::NodeDimensions(glm::vec3 origin,
                               double size) :
    origin(origin),
    size(size) { 
}


glm::bvec3 NodeDimensions::getNextIndex(glm::vec3 point) const {
  glm::vec3 mid = this->origin + glm::vec3(0.5 * this->size);

  return glm::bvec3(point.x > mid.x,
                    point.y > mid.y,
                    point.z > mid.z);
}


NodeDimensions NodeDimensions::getNextDimensions(glm::bvec3 index) const {
  glm::vec3 next_origin = this->origin;
  double next_size = 0.5 * this->size;

  if (index.x) next_origin.x += next_size;
  if (index.y) next_origin.y += next_size;
  if (index.z) next_origin.z += next_size;

  return NodeDimensions(next_origin, next_size);
}


}
}
}