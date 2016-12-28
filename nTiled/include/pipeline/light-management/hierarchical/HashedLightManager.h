#pragma once

#include "world\World.h"
#include "LightOctree.h"
#include "LinklessOctree.h"

namespace nTiled {
namespace pipeline {
namespace hierarchical {

struct HashedConfig {
  HashedConfig(float minimum_node_size,
               unsigned int starting_depth,
               float r_increase_ratio,
               unsigned int max_attempts);

  float minimum_node_size;
  unsigned int starting_depth;
  float r_increase_ratio;
  unsigned int max_attempts;
};


class HashedLightManager {
public:
  HashedLightManager(const world::World& world,
                     float minimum_node_size, 
                     unsigned int starting_depth,
                     float r_increase_ratio,
                     unsigned int max_attempts);
  ~HashedLightManager();

  LightOctree* getLightOctree() { return this->p_light_octree; }
  LinklessOctree* getLinkLessOctree() { return this->p_linkless_octree; }

  void constructLightOctree();
  void loadToShader(GLuint shader);

  unsigned int getOctreeDepth() const { return this->p_linkless_octree->getDepth(); }

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


class HashedLightManagerBuilder {
public:
  HashedLightManagerBuilder();

  virtual HashedLightManager* constructNewHashedLightManager(
    const world::World& world,
    float minimum_node_size,
    unsigned int starting_depth,
    float r_increase_ratio,
    unsigned int max_attempts) const;
};

}
}
}
