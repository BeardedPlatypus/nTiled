#include "pipeline\light-management\hashed\linkless-octree\LinklessOctree.h"

namespace nTiled {
namespace pipeline {
namespace hashed {

// ----------------------------------------------------------------------------
//  Constructor | Destructor
// ----------------------------------------------------------------------------
LinklessOctree::LinklessOctree(
    unsigned int depth,
    unsigned int n_levels,
    double minimum_node_size,
    glm::vec3 origin,
    std::vector<SpatialHashFunction<glm::u8vec2>*>* p_octree_hash_maps,
    std::vector<bool>* p_data_hash_map_exists,
    std::vector<SpatialHashFunction<glm::uvec2>*>* p_data_hash_maps,
    std::vector<GLuint>* p_light_indices) : 
  depth(depth),
  n_levels(n_levels),
  minimum_node_size(minimum_node_size),
  origin(origin),
  p_octree_hash_maps(p_octree_hash_maps),
  p_data_hash_map_exists(p_data_hash_map_exists),
  p_data_hash_maps(p_data_hash_maps),
  p_light_indices(p_light_indices) {
  // TODO build openGL stuff here
}


LinklessOctree::~LinklessOctree() {
  for (SpatialHashFunction<glm::u8vec2>* p_hfunc : *p_octree_hash_maps)
    delete p_hfunc;
  delete p_octree_hash_maps;

  for (unsigned int i = 0; i < this->p_data_hash_map_exists->size(); ++i)
    if (p_data_hash_map_exists->at(i))
      delete this->p_data_hash_maps->at(i);

  delete this->p_data_hash_map_exists;
  delete this->p_data_hash_maps;

  delete this->p_light_indices;
}

// ----------------------------------------------------------------------------
//  Get methods
// ----------------------------------------------------------------------------
std::vector<GLuint> LinklessOctree::retrieveLights(glm::vec3 point) const {
  glm::vec3 orig = this->getOrigin();
  double width = this->getWidth();
  if (point.x < orig.x ||
      point.y < orig.y ||
      point.z < orig.z ||
      point.x > orig.x + width ||
      point.y > orig.y + width ||
      point.z > orig.z + width) {
    return std::vector<GLuint>();
  }

  glm::u8vec2 octree_data;
  glm::uvec2 light_data = glm::uvec2(0);

  double next_node_size = ((this->getTotalNNodes() / this->getInitialNNodes())
                           * this->getMinimumNodeSize());

  glm::vec3 p_octree_coord = point - orig;
  unsigned int n_nodes = this->getInitialNNodes();

  glm::uvec3 coord_cur;
  glm::uvec3 coord_next = glm::uvec3(floor(p_octree_coord.x / next_node_size),
                                     floor(p_octree_coord.y / next_node_size),
                                     floor(p_octree_coord.z / next_node_size));

  glm::uvec3 index_dif;
  glm::bvec3 index_vec;
  unsigned int index_int;

  for (unsigned int layer_i = 0; layer_i < this->getNLevels(); ++layer_i) {
    coord_cur = coord_next;
    next_node_size *= 0.5;
    coord_next = glm::uvec3(floor(p_octree_coord.x / next_node_size),
                            floor(p_octree_coord.y / next_node_size),
                            floor(p_octree_coord.z / next_node_size));
    index_dif = coord_next - (coord_cur + coord_cur);
    index_int = index_dif.x + index_dif.y * 2 + index_dif.z * 4;
    index_vec = glm::bvec3(index_dif.x == 1,
                           index_dif.y == 1,
                           index_dif.z == 1);

    octree_data = this->p_octree_hash_maps->at(layer_i)->getData(coord_cur);
    if (math::extractBit(octree_data.x, index_int)) {
      if (math::extractBit(octree_data.y, index_int)) {
        light_data = this->p_data_hash_maps->at(layer_i)->getData(coord_next);
      }
      break;
    }
  }

  std::vector<GLuint> node_indices = {};
  for (unsigned int i = light_data.x; i < light_data.x + light_data.y; ++i) {
    node_indices.push_back(this->p_light_indices->at(i));
  }

  return node_indices;
}

}
}
}