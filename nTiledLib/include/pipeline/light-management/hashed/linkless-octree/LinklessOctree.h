#pragma once

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <vector>
#include <glm\glm.hpp>
#include <glad\glad.h>

// ----------------------------------------------------------------------------
//  nTiled Headers
// ----------------------------------------------------------------------------
#include "SpatialHashFunction.h"


namespace nTiled {
namespace pipeline {
namespace hashed {

class LinklessOctree {
public:
  LinklessOctree(float minimum_node_size,
                 unsigned int starting_depth,
                 unsigned int initial_n_nodes_dim,
                 glm::vec4 octree_origin,
                 std::vector<SpatialHashFunction<glm::uvec2>*>* leaf_hash_maps,
                 std::vector<bool>* has_leaf_hash_map,
                 std::vector<SpatialHashFunction<glm::u8vec2>*>* node_hash_maps);
  ~LinklessOctree();

  void loadToShader(GLuint shader);

  unsigned int getNLevels() const { return this->node_hash_maps->size(); }

  unsigned int getInitialNNodesDim() const { return this->initial_n_nodes_dim; }

  float getInitialNodeSize() const { return this->minimum_node_size * (1 << (this->getNLevels())); }

  glm::vec4 getOrigin() const { return this->octree_origin; }

  virtual void exportToJson(const std::string& path) const;

private:
  float minimum_node_size;
  unsigned int starting_depth;
  unsigned int initial_n_nodes_dim;
  glm::vec4 octree_origin;

  std::vector<SpatialHashFunction<glm::uvec2>*>* leaf_hash_maps;
  std::vector<bool>* has_leaf_hash_map;

  /*! @brief node_hash_maps contains per depth level the nodes of that specific
  *         depth level packed into GLushorts
  *         index is defined as i = x * 1 + y * 2 + z * 4
  */
  std::vector<SpatialHashFunction<glm::u8vec2>*>* node_hash_maps;

  // openGL raw data
  GLubyte** octree_hash_tables;
  GLubyte** octree_offset_tables;

  GLuint** data_hash_tables;
  GLubyte** data_offset_tables;

};


}
}
}
