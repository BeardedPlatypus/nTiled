#include "pipeline\light-management\hashed\light-octree\slt\SingleLightTreeBuilder.h"

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <queue>


// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "math\points.h"
#include "math\util.h"
#include "pipeline\light-management\hashed\light-octree\slt\Exceptions.h"

#include "pipeline\light-management\hashed\light-octree\slt\nodes\SLTLeaf.h"
#include "pipeline\light-management\hashed\light-octree\slt\nodes\SLTBranch.h"


namespace nTiled {
namespace pipeline {
namespace hashed {

// ---------------------------------------------------------------------------
//  Constructor
// ---------------------------------------------------------------------------
SingleLightTreeBuilder::SingleLightTreeBuilder(const double minimum_node_size,
                                               const glm::vec3 origin_octree) :
    minimum_node_size(minimum_node_size),
    origin_octree(origin_octree) { 
}


// ---------------------------------------------------------------------------
//  SLT Construction methods
// ---------------------------------------------------------------------------
bool SingleLightTreeBuilder::nodeWithinLight(const world::PointLight& light,
                                             glm::vec3 node_origin,
                                             double node_edge) const {
  glm::vec3 closest = math::getClosestPoint(node_origin,
                                            glm::vec3(light.position),
                                            node_origin + glm::vec3(node_edge));
  double dist_x = light.position.x - closest.x;
  double dist_y = light.position.y - closest.y;
  double dist_z = light.position.z - closest.z;

  return ((dist_x * dist_x + dist_y * dist_y + dist_z * dist_z) <
          (light.radius * light.radius));
}


// ---------------------------------------------------------------------------
enum class FillDirection {
  LeftLowerClose,
  RightLowerClose,
  LeftUpperClose,
  RightUpperClose,
  LeftLowerFar,
  RightLowerFar,
  LeftUpperFar,
  RightUpperFar
};


struct FillElement {
  FillElement(glm::uvec3 index,
              FillDirection dir) : index(index), dir(dir) { }

  glm::uvec3 index;
  FillDirection dir;
};


Lattice* SingleLightTreeBuilder::constructLattice(const world::PointLight& light) const {
  if ((this->origin_octree.x > (light.position.x - light.radius)) ||
      (this->origin_octree.y > (light.position.y - light.radius)) ||
      (this->origin_octree.z > (light.position.z - light.radius))) {
    throw SLTLightNotInOctreeException();
  }

  double node_size = this->getMinimalNodeSize();
  glm::vec3 origin_in_octree = this->getOriginOctree();

  // calculate origin of the lattice
  //glm::vec3 origin = glm::vec3(floor((light.position.x - light.radius - origin_in_octree.x) / node_size) * node_size + origin_in_octree.x,
  //                             floor((light.position.y - light.radius - origin_in_octree.y) / node_size) * node_size + origin_in_octree.y,
  //                             floor((light.position.z - light.radius - origin_in_octree.z) / node_size) * node_size + origin_in_octree.z);

  //unsigned int n_nodes = int(floor((light.position.x + light.radius - origin.x) / node_size)) + 1;

  //unsigned int n_nodes_y = int(floor((light.position.y + light.radius - origin.y) / node_size)) + 1; 
  //if (n_nodes < n_nodes_y) n_nodes = n_nodes_y;
  //unsigned int n_nodes_z = int(floor((light.position.z + light.radius - origin.z) / node_size)) + 1; 
  //if (n_nodes < n_nodes_z) n_nodes = n_nodes_z;
  glm::uvec3 p_min = glm::uvec3(unsigned int(floor(light.position.x - light.radius - origin_in_octree.x) / node_size),
                                unsigned int(floor(light.position.y - light.radius - origin_in_octree.y) / node_size),
                                unsigned int(floor(light.position.z - light.radius - origin_in_octree.z) / node_size));

  glm::uvec3 p_max = glm::uvec3(unsigned int(floor(light.position.x + light.radius - origin_in_octree.x) / node_size),
                                unsigned int(floor(light.position.y + light.radius - origin_in_octree.y) / node_size),
                                unsigned int(floor(light.position.z + light.radius - origin_in_octree.z) / node_size));

  unsigned int n_nodes = p_max.x - p_min.x + 1;
  if (p_max.y - p_min.y + 1 > n_nodes) n_nodes = p_max.y - p_min.y + 1;
  if (p_max.z - p_min.z + 1 > n_nodes) n_nodes = p_max.z - p_min.z + 1;

  glm::vec3 origin =  glm::vec3(p_min.x * node_size + origin_in_octree.x,
                                p_min.y * node_size + origin_in_octree.y,
                                p_min.z * node_size + origin_in_octree.z);

  Lattice* p_lattice = new Lattice(origin, n_nodes, node_size, light);

  std::vector<bool> checked = std::vector<bool>(n_nodes * n_nodes * n_nodes, false);
  unsigned int last = n_nodes - 1;

  std::queue<FillElement> to_check = std::queue<FillElement>(
  { FillElement(glm::uvec3(   0,    0,    0), FillDirection::LeftLowerClose),
    FillElement(glm::uvec3(   0,    0, last), FillDirection::LeftLowerFar),
    FillElement(glm::uvec3(   0, last,    0), FillDirection::LeftUpperClose),
    FillElement(glm::uvec3(   0, last, last), FillDirection::LeftUpperFar),
    FillElement(glm::uvec3(last,    0,    0), FillDirection::RightLowerClose),
    FillElement(glm::uvec3(last,    0, last), FillDirection::RightLowerFar),
    FillElement(glm::uvec3(last, last,    0), FillDirection::RightUpperClose),
    FillElement(glm::uvec3(last, last, last), FillDirection::RightUpperFar)
  });

  while (!to_check.empty()) {
    FillElement next_element = to_check.front();
    to_check.pop();

    unsigned int node_i = math::toIndex(next_element.index, n_nodes);

    if (!checked.at(node_i)) {
      checked.at(node_i) = true;

      glm::vec3 node_origin =
        origin + glm::vec3(next_element.index.x * node_size,
                           next_element.index.y * node_size,
                           next_element.index.z * node_size);

      if (!this->nodeWithinLight(light, node_origin, node_size)) {
        p_lattice->setNode(next_element.index, NodeType::Empty);

        switch (next_element.dir) {
        case FillDirection::LeftLowerClose:
          to_check.push(FillElement(next_element.index + glm::uvec3(1, 0, 0),
                                    next_element.dir));
          to_check.push(FillElement(next_element.index + glm::uvec3(0, 1, 0),
                                    next_element.dir));
          to_check.push(FillElement(next_element.index + glm::uvec3(0, 0, 1),
                                    next_element.dir));
          break;
        case FillDirection::LeftLowerFar:
          to_check.push(FillElement(next_element.index + glm::uvec3(1, 0, 0),
                                    next_element.dir));
          to_check.push(FillElement(next_element.index + glm::uvec3(0, 1, 0),
                                    next_element.dir));
          to_check.push(FillElement(next_element.index + glm::uvec3(0, 0, -1),
                                    next_element.dir));
          break;
        case FillDirection::LeftUpperClose:
          to_check.push(FillElement(next_element.index + glm::uvec3(1, 0, 0),
                                    next_element.dir));
          to_check.push(FillElement(next_element.index + glm::uvec3(0, -1, 0),
                                    next_element.dir));
          to_check.push(FillElement(next_element.index + glm::uvec3(0, 0, 1),
                                    next_element.dir));
          break;
        case FillDirection::LeftUpperFar:
          to_check.push(FillElement(next_element.index + glm::uvec3(1, 0, 0),
                                    next_element.dir));
          to_check.push(FillElement(next_element.index + glm::uvec3(0, -1, 0),
                                    next_element.dir));
          to_check.push(FillElement(next_element.index + glm::uvec3(0, 0, -1),
                                    next_element.dir));
          break;
        case FillDirection::RightLowerClose:
          to_check.push(FillElement(next_element.index + glm::uvec3(-1, 0, 0),
                                    next_element.dir));
          to_check.push(FillElement(next_element.index + glm::uvec3(0, 1, 0),
                                    next_element.dir));
          to_check.push(FillElement(next_element.index + glm::uvec3(0, 0, 1),
                                    next_element.dir));
          break;
        case FillDirection::RightLowerFar:
          to_check.push(FillElement(next_element.index + glm::uvec3(-1, 0, 0),
                                    next_element.dir));
          to_check.push(FillElement(next_element.index + glm::uvec3(0, 1, 0),
                                    next_element.dir));
          to_check.push(FillElement(next_element.index + glm::uvec3(0, 0, -1),
                                    next_element.dir));
          break;
        case FillDirection::RightUpperClose:
          to_check.push(FillElement(next_element.index + glm::uvec3(-1, 0, 0),
                                    next_element.dir));
          to_check.push(FillElement(next_element.index + glm::uvec3(0, -1, 0),
                                    next_element.dir));
          to_check.push(FillElement(next_element.index + glm::uvec3(0, 0, 1),
                                    next_element.dir));
          break;
        case FillDirection::RightUpperFar:
          to_check.push(FillElement(next_element.index + glm::uvec3(-1, 0, 0),
                                    next_element.dir));
          to_check.push(FillElement(next_element.index + glm::uvec3(0, -1, 0),
                                    next_element.dir));
          to_check.push(FillElement(next_element.index + glm::uvec3(0, 0, -1),
                                    next_element.dir));
          break;
        default:
          throw SLTLatticeConstructionException();
          break;
        }
      }
    }
  }

  return p_lattice;
}


double SingleLightTreeBuilder::getMaxSizeSLT(const world::PointLight& light) const {
  return this->getMaxNNodesSLT(light) * this->getMinimalNodeSize();
}


unsigned int SingleLightTreeBuilder::getMaxNNodesSLT(const world::PointLight& light) const {
  glm::vec3 origin_octree = this->getOriginOctree();
  if (((light.position.x - light.radius) < origin_octree.x) ||
      ((light.position.y - light.radius) < origin_octree.y) ||
      ((light.position.z - light.radius) < origin_octree.z)) {
    throw SLTLightNotInOctreeException();
  }

  // calculate raw size
  double width = 2 * light.radius;
  unsigned int n_minimum_nodes = int(ceil(width / this->getMinimalNodeSize()));
  unsigned int n_nodes = (1 << (unsigned int(ceil(log2(n_minimum_nodes))))); // 2 ^ (ceil(lb(n_minimum_nodes)))

  double denominator = 1.0 / (n_nodes * this->getMinimalNodeSize());

  glm::vec3 p_min = glm::vec3(light.position.x - light.radius - origin_octree.x,
                              light.position.y - light.radius - origin_octree.y,
                              light.position.z - light.radius - origin_octree.z);

  glm::vec3 p_max = glm::vec3(light.position.x + light.radius - origin_octree.x,
                              light.position.y + light.radius - origin_octree.y,
                              light.position.z + light.radius - origin_octree.z);

  glm::uvec3 node_min_p = glm::uvec3(unsigned int(floor(p_min.x * denominator)),
                                     unsigned int(floor(p_min.y * denominator)),
                                     unsigned int(floor(p_min.z * denominator)));

  glm::uvec3 node_max_p = glm::uvec3(unsigned int(floor(p_max.x * denominator)),
                                     unsigned int(floor(p_max.y * denominator)),
                                     unsigned int(floor(p_max.z * denominator)));

  while (!(node_min_p.x == node_max_p.x &&
           node_min_p.y == node_max_p.y &&
           node_min_p.z == node_max_p.z)) {
    n_nodes *= 2;
    denominator *= 0.5;
    node_min_p = glm::uvec3(unsigned int(floor(p_min.x * denominator)),
                            unsigned int(floor(p_min.y * denominator)),
                            unsigned int(floor(p_min.z * denominator)));

    node_max_p = glm::uvec3(unsigned int(floor(p_max.x * denominator)),
                            unsigned int(floor(p_max.y * denominator)),
                            unsigned int(floor(p_max.z * denominator)));
  }

  return n_nodes;
}


NodeType SingleLightTreeBuilder::determineNodeType(glm::vec3 node_origin,
                                                   double node_size,
                                                   const Lattice& lattice) const {
  glm::vec3 lattice_origin = lattice.getOrigin();
  double lattice_size = lattice.getWidth();

  glm::vec3 node_min = node_origin + glm::vec3(0.1 * lattice.getNodeSize());
  glm::vec3 node_max = node_origin + glm::vec3(node_size - 0.1 * lattice.getNodeSize());

  double node_size_adj = node_size - (0.2 * lattice.getNodeSize());

  if ((node_max.x < lattice_origin.x) ||
      (node_max.y < lattice_origin.y) ||
      (node_max.z < lattice_origin.z) ||
      (node_min.x > lattice_origin.x + lattice_size) ||
      (node_min.y > lattice_origin.y + lattice_size) ||
      (node_min.z > lattice_origin.z + lattice_size)) {
    // node outside of lattice
    return NodeType::Empty;
  } else {
    if ((node_min.x > lattice_origin.x) &&
        (node_min.y > lattice_origin.y) &&
        (node_min.z > lattice_origin.z) &&
        (node_max.x < lattice_origin.x + lattice_size) &&
        (node_max.y < lattice_origin.y + lattice_size) &&
        (node_max.z < lattice_origin.z + lattice_size)) {
      // adjust node size to ensure not on halves
      if (lattice.getNode(lattice.getClosestNodeToLightSource(node_min, node_size_adj)) == NodeType::Empty) {
        return NodeType::Empty;
      } else {
        if (lattice.getNode(lattice.getFurthestNodeAwayFromLightSource(node_min, node_size_adj)) == NodeType::Filled) {
          return NodeType::Filled;
        } else {
          return NodeType::Partial;
        }
      }
    } else {
      // node partially inside lattice
      if (lattice.getNode(lattice.getClosestNodeToLightSource(node_min, node_size_adj)) == NodeType::Empty) {
        return NodeType::Empty;
      } else {
        return NodeType::Partial;
      }
    }
  }
}


SingleLightTree* SingleLightTreeBuilder::constructSLT(const world::PointLight& light) const {
  glm::vec3 octree_origin = this->getOriginOctree();

  unsigned int n_nodes = this->getMaxNNodesSLT(light);
  double width = this->getMaxSizeSLT(light);

  // calculate origin 
  glm::vec3 origin = glm::vec3(floor((light.position.x - light.radius - octree_origin.x) / width) * width + octree_origin.x,
                               floor((light.position.y - light.radius - octree_origin.y) / width) * width + octree_origin.y,
                               floor((light.position.z - light.radius - octree_origin.z) / width) * width + octree_origin.z);

  Lattice* p_lattice = this->constructLattice(light);

  SLTNode* root;
  NodeType node_type = this->determineNodeType(origin, width, *p_lattice);
  if (node_type == NodeType::Partial) {
    // ------------------------------------------------------------------------
    //  calculate first partial
    NodeDimensions cur_dim = NodeDimensions(origin, width);
    SLTBranch* cur_partial = new SLTBranch();
    root = cur_partial;

    // ------------------------------------------------------------------------
    //  set up queue
    std::queue<std::pair<SLTBranch*, NodeDimensions>> queue = {};
    queue.push(std::pair<SLTBranch*, NodeDimensions>(cur_partial, cur_dim));

    // ------------------------------------------------------------------------
    //  variables while
    NodeDimensions next_dim = cur_dim;
    glm::bvec3 index;
    NodeType next_node_type;
    SLTBranch* next_partial;

    while (!queue.empty()) {
      // take elements from queue
      cur_partial = queue.front().first;
      cur_dim = queue.front().second;
      queue.pop();

      // subdivide current element
      for (unsigned int x_i = 0; x_i < 2; ++x_i) {
        for (unsigned int y_i = 0; y_i < 2; ++y_i) {
          for (unsigned int z_i = 0; z_i < 2; ++z_i) {

            index = glm::bvec3(x_i == 1,
                               y_i == 1,
                               z_i == 1);
            next_dim = cur_dim.getNextDimensions(index);
            bool within_light = this->nodeWithinLight(light, next_dim.origin, next_dim.size);

            next_node_type = this->determineNodeType(next_dim.origin,
                                                     next_dim.size, 
                                                     *p_lattice);

            if (next_node_type == NodeType::Partial) {
              next_partial = new SLTBranch();
              queue.push(std::pair<SLTBranch*, NodeDimensions>(next_partial, next_dim));
              cur_partial->setNode(index, next_partial);
            } else {
              cur_partial->setNode(index, new SLTLeaf(next_node_type));
            }
          }
        }
      }
    }
  } else {
    root = new SLTLeaf(node_type);
  }

  // clean up
  delete p_lattice;

  return new SingleLightTree(origin, n_nodes, this->getMinimalNodeSize(), root);
}


}
}
}