#include "pipeline\light-management\hierarchical\single-light-tree\construction-util\Lattice.h"

namespace nTiled {
namespace pipeline {
namespace hierarchical {
namespace slt {


// ----------------------------------------------------------------------------
//  Constructors
// ----------------------------------------------------------------------------
Lattice::Lattice(const glm::vec3 octree_origin,
                 const glm::uvec3 origin_lattice_coordinates,
                 const unsigned int n_nodes,
                 const int depth,
                 const float minimum_node_size) :
  octree_origin(octree_origin),
  origin_lattice_coordinates(origin_lattice_coordinates),
  n_nodes(n_nodes),
  depth(depth),
  minimum_node_size(minimum_node_size),
  unique_nodes(std::vector<LatticeNode*>()),
  lattice_nodes(std::vector<LatticeNode*>(n_nodes * n_nodes * n_nodes)) { }


Lattice::Lattice(const glm::vec3 octree_origin,
                 const glm::uvec3 origin_lattice_coordinates,
                 const unsigned int n_nodes,
                 const int depth,
                 const float minimum_node_size,
                 LatticeNode* p_node) :
  octree_origin(octree_origin),
  origin_lattice_coordinates(origin_lattice_coordinates),
  n_nodes(n_nodes),
  depth(depth),
  minimum_node_size(minimum_node_size),
  unique_nodes(std::vector<LatticeNode*>({ p_node })),
  lattice_nodes(std::vector<LatticeNode*>((n_nodes * n_nodes * n_nodes), p_node)) { }



Lattice::Lattice(const glm::vec3 octree_origin,
                 const glm::uvec3 origin_in_grid,
                 const unsigned int n_nodes,
                 const int depth,
                 const float minimum_node_size,
                 const NoLightNode& no_light) :
  Lattice(octree_origin, 
          origin_in_grid, 
          n_nodes, 
          depth, 
          minimum_node_size, 
          new LatticeNode(&no_light)) { }


Lattice::Lattice(const glm::vec3 octree_origin,
                 const glm::uvec3 origin_in_grid,
                 const unsigned int n_nodes,
                 const int depth,
                 const float minimum_node_size,
                 const FullLightNode& full_light) :
  Lattice(octree_origin, 
          origin_in_grid, 
          n_nodes, 
          depth, 
          minimum_node_size, 
          new LatticeNode(&full_light)) { }


// ----------------------------------------------------------------------------
//  Destructor
// ----------------------------------------------------------------------------
Lattice::~Lattice() {
  for (LatticeNode* p_node : this->unique_nodes) {
    delete p_node;
  }
}


// ----------------------------------------------------------------------------
//  Get || Set methods
// ----------------------------------------------------------------------------
const LatticeNode& Lattice::getLatticeNode(glm::uvec3 position) const {
  return *(this->lattice_nodes[this->getIndexNode(position)]);
}


void Lattice::setLatticeNode(glm::uvec3 position,
                             LatticeNode* p_node) {
  int index = this->getIndexNode(position);

  this->unique_nodes.push_back(p_node);
  this->lattice_nodes[index] = p_node;
}


void Lattice::setLatticeNode(glm::uvec3 position,
                             const LatticeNode& node) {
  this->setLatticeNode(position, new LatticeNode(node));
}


void Lattice::setLatticeNode(glm::uvec3 position, const NoLightNode& no_light) {
  this->setLatticeNode(position, new LatticeNode(&no_light));
}


void Lattice::setLatticeNode(glm::uvec3 position, const FullLightNode& full_light) {
  this->setLatticeNode(position, new LatticeNode(&full_light));
}


void Lattice::setLatticeNode(glm::uvec3 position, const PartialLightNode& partial_light) {
  this->setLatticeNode(position, new LatticeNode(&partial_light));
}


int Lattice::getIndexNode(glm::uvec3 position) const {
  return (position.x
          + position.z * this->getNNodes()
          + position.y * this->getNNodes() * this->getNNodes()
          );
}


} // slt
} // hierarchical
} // pipeline
} // nTiled