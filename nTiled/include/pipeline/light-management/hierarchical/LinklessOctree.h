#pragma once

#include <vector>
#include <glm\glm.hpp>
#include <glad\glad.h>

#include "LightOctree.h"
#include "SpatialHashFunction.h"

namespace nTiled {
namespace pipeline {
namespace hierarchical {

class LinklessOctree {
public:
  LinklessOctree(std::vector<SpatialHashFunction<glm::uvec2>*>* leaf_hash_maps,
                 std::vector<bool>* has_leaf_hash_map,
                 std::vector<SpatialHashFunction<glm::bvec2>*>* node_hash_maps);
  ~LinklessOctree();

  void loadToShader();

private:
  std::vector<SpatialHashFunction<glm::uvec2>*>* leaf_hash_maps;
  std::vector<bool>* has_leaf_hash_map;

  std::vector<SpatialHashFunction<glm::bvec2>*>* node_hash_maps;
};


}
}
}
