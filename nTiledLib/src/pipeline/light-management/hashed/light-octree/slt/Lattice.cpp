#include "pipeline\light-management\hashed\light-octree\slt\Lattice.h"

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <algorithm>


// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "math\util.h"
#include "math\points.h"
#include "pipeline\light-management\hashed\light-octree\slt\Exceptions.h"


namespace nTiled {
namespace pipeline {
namespace hashed {

// ----------------------------------------------------------------------------
//  Constructor
// ----------------------------------------------------------------------------
Lattice::Lattice(glm::vec3 origin,
                 unsigned int n_nodes_dim,
                 double node_size,
                 world::PointLight light) :
  origin(origin),
  n_nodes_dim(n_nodes_dim),
  node_size(node_size),
  light(light),
  nodes(std::vector<NodeType>(n_nodes_dim * n_nodes_dim * n_nodes_dim,
                              NodeType::Filled)) {
}


// ----------------------------------------------------------------------------
//  basic GET|SET
// ----------------------------------------------------------------------------
NodeType Lattice::getNode(glm::uvec3 index) const {
  return this->nodes.at(math::toIndex(index, this->n_nodes_dim));
}

void Lattice::setNode(glm::uvec3 index, NodeType node_type) {
  this->nodes.at(math::toIndex(index, this->n_nodes_dim)) = node_type;
}


// ----------------------------------------------------------------------------
//  node query methods
// ----------------------------------------------------------------------------
glm::uvec3 Lattice::getClosestNodeToLightSource(glm::vec3 cube_origin,
                                              double cube_size) const {
  glm::vec3 lattice_origin = this->getOrigin();
  double lattice_size = this->getWidth();
  double node_size = this->getNodeSize();

  if ((cube_origin.x + cube_size < lattice_origin.x) ||
      (cube_origin.y + cube_size < lattice_origin.y) ||
      (cube_origin.z + cube_size < lattice_origin.z) ||
      (cube_origin.x > lattice_origin.x + lattice_size) ||
      (cube_origin.y > lattice_origin.y + lattice_size) ||
      (cube_origin.z > lattice_origin.z + lattice_size))
    throw SLTOutOfLatticeBoundsException();


  glm::vec3 lowerbound = glm::vec3(std::max(double(cube_origin.x), lattice_origin.x + 0.5 * node_size),
                                   std::max(double(cube_origin.y), lattice_origin.y + 0.5 * node_size),
                                   std::max(double(cube_origin.z), lattice_origin.z + 0.5 * node_size));
  glm::vec3 upperbound = glm::vec3(std::min(cube_origin.x + cube_size, lattice_origin.x + lattice_size - 0.5 * node_size),
                                   std::min(cube_origin.y + cube_size, lattice_origin.y + lattice_size - 0.5 * node_size),
                                   std::min(cube_origin.z + cube_size, lattice_origin.z + lattice_size - 0.5 * node_size));

  glm::vec3 closest_point = math::getClosestPoint(lowerbound, 
                                                  glm::vec3(this->getLight().position), 
                                                  upperbound);

  return this->mapToNode(closest_point);
}


glm::uvec3 Lattice::getFurthestNodeAwayFromLightSource(glm::vec3 cube_origin,
                                                     double cube_size) const {
  glm::vec3 lattice_origin = this->getOrigin();
  double lattice_size = this->getWidth();

  if ((cube_origin.x + cube_size < lattice_origin.x) ||
      (cube_origin.y + cube_size < lattice_origin.y) ||
      (cube_origin.z + cube_size < lattice_origin.z) ||
      (cube_origin.x > lattice_origin.x + lattice_size) ||
      (cube_origin.y > lattice_origin.y + lattice_size) ||
      (cube_origin.z > lattice_origin.z + lattice_size))
    throw SLTOutOfLatticeBoundsException();

  glm::vec3 lowerbound = glm::vec3(std::max(double(cube_origin.x), lattice_origin.x + 0.5 * node_size),
                                   std::max(double(cube_origin.y), lattice_origin.y + 0.5 * node_size),
                                   std::max(double(cube_origin.z), lattice_origin.z + 0.5 * node_size));
  glm::vec3 upperbound = glm::vec3(std::min(cube_origin.x + cube_size, lattice_origin.x + lattice_size - 0.5 * node_size),
                                   std::min(cube_origin.y + cube_size, lattice_origin.y + lattice_size - 0.5 * node_size),
                                   std::min(cube_origin.z + cube_size, lattice_origin.z + lattice_size - 0.5 * node_size));

  glm::vec3 furthest_point = math::getFurthestPoint(lowerbound, 
                                                   glm::vec3(this->getLight().position), 
                                                   upperbound);

  return this->mapToNode(furthest_point);
}


// ----------------------------------------------------------------------------
glm::uvec3 Lattice::mapToNode(glm::vec3 point) const {

  glm::vec3 origin_lattice = this->getOrigin();
  double node_size = this->getNodeSize();

  if ((point.x < origin_lattice.x) ||
      (point.y < origin_lattice.y) ||
      (point.z < origin_lattice.z) ||
      (point.x > origin_lattice.x + this->getWidth()) ||
      (point.y > origin_lattice.y + this->getWidth()) ||
      (point.z > origin_lattice.z + this->getWidth()))
    throw SLTOutOfLatticeBoundsException();

  return glm::uvec3(floor((point.x - origin_lattice.x) / node_size),
                    floor((point.y - origin_lattice.y) / node_size),
                    floor((point.z - origin_lattice.z) / node_size));
}


}
}
}
