#pragma once

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <glad\glad.h>
#include <random>

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "world\World.h"
#include "light-octree\LightOctree.h"
#include "linkless-octree\LinklessOctree.h"

namespace nTiled {
namespace pipeline {
namespace hashed {

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
  void updateOctreeOrigin(GLuint shader, const glm::mat4& lookAt_matrix);

  unsigned int getLinklessOctreeLevels() const { return this->p_linkless_octree->getNLevels(); }

  void exportToJson(const std::string& path_lights,
                    const std::string& path_light_octree,
                    const std::string& path_linkless_octree,
                    const std::string& path_light_indices) const;

private:
  LightOctree* p_light_octree;
  LinklessOctree* p_linkless_octree;

  std::vector<GLuint> light_index_list;

  GLuint light_index_buffer;

  const world::World& world;
  float minimum_node_size;
  unsigned int starting_depth;
  float r_increase_ratio;
  unsigned int max_attempts;

  // util
  std::mt19937 gen;
  std::uniform_int_distribution<unsigned short> distribution;
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
