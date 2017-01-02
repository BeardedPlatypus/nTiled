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

  unsigned int getLinklessOctreeLevels() const { return this->p_linkless_octree->getNLevels(); }

  void exportToJson(const std::string& path_lights,
                    const std::string& path_light_octree,
                    const std::string& path_linkless_octree,
                    const std::string& path_light_indices) const;

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
