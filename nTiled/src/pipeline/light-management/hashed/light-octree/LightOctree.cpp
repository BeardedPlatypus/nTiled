#include "pipeline\light-management\hashed\light-octree\LightOctree.h"

namespace nTiled {
namespace pipeline {
namespace hashed {

LightOctree::LightOctree(glm::vec4 octree_origin,
                         float minimum_leaf_node_size,
                         unsigned int octree_depth) :
  octree_origin(octree_origin),
  minimum_leaf_node_size(minimum_leaf_node_size),
  octree_depth(octree_depth),
  p_root(new lo::Root()) {
}


LightOctree::~LightOctree() {
  delete this->p_root;
}


void LightOctree::addSLT(const SingleLightTree& slt) {
  this->p_root->addSLT(0, glm::uvec3(0), slt);
}


std::vector<std::pair<glm::uvec3, lo::Node*>> LightOctree::getNodesAtDepth(unsigned int depth) {
  if (depth < 0) {
    return std::vector<std::pair<glm::uvec3, lo::Node*>>();
  }

  unsigned short current = 0;
  std::vector<std::pair<glm::uvec3, lo::Node*>> nodes[2] = {
    std::vector<std::pair<glm::uvec3, lo::Node*>>(),
    std::vector<std::pair<glm::uvec3, lo::Node*>>() };

  nodes[current].push_back(
    std::pair<glm::uvec3, lo::Node*>(glm::uvec3(0, 0, 0), this->p_root));

  for (unsigned int i = 0; i < depth; i++) {
    for (const std::pair<glm::uvec3, lo::Node*> n : nodes[current]) {
      n.second->getSubNodes(n.first, nodes[(current + 1) & 1]);
    }
    nodes[current].clear();
    current = (current + 1) & 1;
  }

  return nodes[current];
}


}
}
}