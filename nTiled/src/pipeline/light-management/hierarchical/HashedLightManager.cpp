#include "pipeline\light-management\hierarchical\HashedLightManager.h"

#include "pipeline\light-management\hierarchical\LightOctreeBuilder.h"

#include <glm/gtc/type_ptr.hpp>


namespace nTiled {
namespace pipeline {
namespace hierarchical {


HashedConfig::HashedConfig(float minimum_node_size,
                           unsigned int starting_depth,
                           float r_increase_ratio,
                           unsigned int max_attempts) :
  minimum_node_size(minimum_node_size),
  starting_depth(starting_depth),
  r_increase_ratio(r_increase_ratio),
  max_attempts(max_attempts) { }


HashedLightManager::HashedLightManager(const world::World& world,
                                       float minimum_node_size,
                                       unsigned int starting_depth,
                                       float r_increase_ratio,
                                       unsigned int max_attempts) :
    world(world),
    minimum_node_size(minimum_node_size),
    starting_depth(starting_depth),
    r_increase_ratio(r_increase_ratio),
    max_attempts(max_attempts) { }


HashedLightManager::~HashedLightManager() {
  delete this->p_light_octree;
  delete this->p_linkless_octree;
}


void HashedLightManager::loadToShader(GLuint shader) {
  // load octree origin
  glm::vec4 octree_origin_4 = this->p_light_octree->getOrigin();
  glm::vec3 octree_origin = glm::vec3(octree_origin_4.x, 
                                      octree_origin_4.y, 
                                      octree_origin_4.z) / octree_origin_4.w;

  GLint p_octree_origin = glGetUniformLocation(shader, "octree_origin");
  glUniform3fv(p_octree_origin, 1, glm::value_ptr(octree_origin));

  // lode node size base den
  GLfloat node_size_base_den = 1.0f / this->minimum_node_size;

  GLint p_node_size_base_den = glGetUniformLocation(shader,
                                                    "node_size_base_den");
  glUniform1f(p_node_size_base_den, node_size_base_den);

  this->p_linkless_octree->loadToShader(shader);

}



void HashedLightManager::constructLightOctree() {
  LightOctreeBuilder builder = LightOctreeBuilder();
  this->p_light_octree = builder.buildLightOctree(this->world.p_lights,
                                                  this->minimum_node_size);
  // Current data node
  unsigned short current = 0;

  // Construction data array
  // nodes[current] contains data this layer
  // nodes[(current + 1) & 1] contains data next layer
  std::vector<std::pair<glm::uvec3, lo::Node*>> nodes[2] = {
    this->p_light_octree->getNodesAtDepth(this->starting_depth),
    std::vector<std::pair<glm::uvec3, lo::Node*>>() };

  // Hash Node data single layer
  std::vector<std::pair<glm::uvec3, glm::u8vec2>> hash_nodes =
    std::vector<std::pair<glm::uvec3, glm::u8vec2>>();

  // Leaf Node data single layer
  std::vector<std::pair<glm::uvec3, glm::uvec2>> leaf_nodes =
    std::vector<std::pair<glm::uvec3, glm::uvec2>>();

  // Light index list, containing indices to lights
  this->light_index_list.clear();

  // Vector pointer to if leaf data hash function exists
  std::vector<SpatialHashFunction<glm::u8vec2>*>* p_node_hash_functions =
    new std::vector<SpatialHashFunction<glm::u8vec2>*>();

  // Vector pointer to leaf data hash maps
  std::vector<SpatialHashFunction<glm::uvec2>*>* p_leaf_hash_functions = 
    new std::vector<SpatialHashFunction<glm::uvec2>*>();

  std::vector<bool>* p_has_leaf_hash_functions =
    new std::vector<bool>();

  while (!nodes[current].empty()) {
    // get data of this level
    for (std::pair<glm::uvec3, lo::Node*> node : nodes[current]) {
      node.second->extractData(node.first,
                               hash_nodes,
                               leaf_nodes,
                               this->light_index_list,
                               nodes[(current + 1) & 1]);
    }
    // construct spatial functions of this map
    p_node_hash_functions->push_back(new SpatialHashFunction<glm::u8vec2>(hash_nodes,
                                                                          this->max_attempts,
                                                                          this->r_increase_ratio));

    if (leaf_nodes.empty()) {
      p_has_leaf_hash_functions->push_back(false);
      p_leaf_hash_functions->push_back(nullptr);
    } else {
      p_has_leaf_hash_functions->push_back(true);
      p_leaf_hash_functions->push_back(new SpatialHashFunction<glm::uvec2>(leaf_nodes,
                                                                           this->max_attempts,
                                                                           this->r_increase_ratio));
    }
    nodes[current].clear();
    hash_nodes.clear();
    leaf_nodes.clear();

    current = (current + 1) & 1;
  }
  
  this->p_linkless_octree = new LinklessOctree(p_leaf_hash_functions,
                                               p_has_leaf_hash_functions,
                                               p_node_hash_functions);
}




HashedLightManagerBuilder::HashedLightManagerBuilder() { }

HashedLightManager* HashedLightManagerBuilder::constructNewHashedLightManager(
  const world::World& world,
  float minimum_node_size,
  unsigned int starting_depth,
  float r_increase_ratio,
  unsigned int max_attempts) const {
  return new HashedLightManager(world, 
                                minimum_node_size, 
                                starting_depth, 
                                r_increase_ratio, 
                                max_attempts);
}

}
}
}
