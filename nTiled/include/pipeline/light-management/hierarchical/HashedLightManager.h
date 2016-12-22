#pragma once

#include "world\World.h"
#include "LightOctree.h"
#include "LinklessOctree.h"

namespace nTiled {
namespace pipeline {
namespace hierarchical {

class HashedLightManager {
  HashedLightManager(const world::World& world,
                     float minimum_node_size, 
                     unsigned int starting_depth,
                     float r_increase_ratio,
                     unsigned int max_attempts);

  void constructLightOctree();
private:
  LightOctree* p_light_octree;
  LinklessOctree* p_linkless_octree;

  std::vector<GLuint> light_index_list;

  const world::World& world;
  const float minimum_node_size;
  const unsigned int starting_depth;
  const float r_increase_ratio;
  const unsigned int max_attempts;
};

}
}
}
