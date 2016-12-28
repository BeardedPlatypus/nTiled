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
                 std::vector<SpatialHashFunction<glm::u8vec2>*>* node_hash_maps);
  ~LinklessOctree();

  void loadToShader(GLuint shader);

  unsigned int getDepth() const { return this->node_hash_maps->size(); }

private:
  std::vector<SpatialHashFunction<glm::uvec2>*>* leaf_hash_maps;
  std::vector<bool>* has_leaf_hash_map;

  /*! @brief node_hash_maps contains per depth level the nodes of that specific
   *         depth level packed into GLushorts
   *         index is defined as i = x * 1 + y * 2 + z * 4
   */
  std::vector<SpatialHashFunction<glm::u8vec2>*>* node_hash_maps;

  // openGL raw data
  GLushort** octree_hash_tables;
  GLushort** octree_offset_tables;

  GLuint** data_hash_tables;
  GLushort** data_offset_tables;
};


}
}
}
