#include "pipeline\light-management\hierarchical\HashedLightManager.h"

#include "pipeline\light-management\hierarchical\LightOctreeBuilder.h"

namespace nTiled {
namespace pipeline {
namespace hierarchical {

void HashedLightManager::constructLightOctree() {
  LightOctreeBuilder builder = LightOctreeBuilder();
  this->p_light_octree = builder.buildLightOctree(this->world.p_lights,
                                                  this->minimum_node_size);

  unsigned short current = 0;
  std::vector<std::pair<glm::uvec3, lo::Node*>> nodes[2] = {
    this->p_light_octree->getNodesAtDepth(this->starting_depth),
    std::vector<std::pair<glm::uvec3, lo::Node*>>() };

  std::vector<std::pair<glm::uvec3, glm::bvec2>> hash_nodes =
    std::vector<std::pair<glm::uvec3, glm::bvec2>>();
  std::vector<std::pair<glm::uvec3, glm::uvec2>> leaf_nodes =
    std::vector<std::pair<glm::uvec3, glm::uvec2>>();

  this->light_index_list.clear();

  std::vector<SpatialHashFunction<glm::uvec2>*>* leaf_hash_maps = 
    new std::vector<SpatialHashFunction<glm::uvec2>*>();
  std::vector<bool>* has_leaf_hash_map =
    new std::vector<bool>();
  std::vector<SpatialHashFunction<glm::bvec2>*>* node_hash_maps =
    new std::vector<SpatialHashFunction<glm::bvec2>*>();

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
    node_hash_maps->push_back(new SpatialHashFunction<glm::bvec2>(hash_nodes,
                                                                  this->max_attempts,
                                                                  this->r_increase_ratio));

    if (leaf_nodes.empty()) {
      has_leaf_hash_map->push_back(false);
      leaf_hash_maps->push_back(nullptr);
    } else {
      has_leaf_hash_map->push_back(true);
      leaf_hash_maps->push_back(new SpatialHashFunction<glm::uvec2>(leaf_nodes,
                                                                    this->max_attempts,
                                                                    this->r_increase_ratio));
    }
    nodes[current].clear();
    hash_nodes.clear();
    leaf_nodes.clear();

    current = (current + 1) & 1;
  }

  this->p_linkless_octree = new LinklessOctree(leaf_hash_maps,
                                               has_leaf_hash_map,
                                               node_hash_maps);
}

}
}
}
