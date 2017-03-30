#include "pipeline\light-management\hashed\HashedLightManager.h"

// ----------------------------------------------------------------------------
//  nTiled Headers
// ----------------------------------------------------------------------------
#include "pipeline\light-management\hashed\light-octree\slt\SingleLightTreeBuilder.h"
#include "pipeline\light-management\hashed\Exceptions.h"

#include "math\util.h"
#include "math\points.h"
#include "math\octree.h"


namespace nTiled {
namespace pipeline {
namespace hashed {


// ----------------------------------------------------------------------------
//  Constructor | Destructor
// ----------------------------------------------------------------------------
HashedLightManager::HashedLightManager(const world::World& world,
                                       double minimal_node_size) :
  world(world),
  minimal_node_size(minimal_node_size),
  ps_slt({}),
  has_constructed_light_octree(false),
  has_constructed_slts(false) {
}


HashedLightManager::~HashedLightManager() {
  if (this->has_constructed_slts) {
    for (SingleLightTree* p_slt : this->ps_slt) {
      delete p_slt;
    }
  }

  if (this->has_constructed_light_octree) {
    delete this->p_light_octree;
  }
}

// ----------------------------------------------------------------------------
//  LightOctree construction
// ----------------------------------------------------------------------------
void HashedLightManager::constructLightOctree() {
  this->constructEmptyLightOctree();
  this->constructSLTs();

  std::vector<SingleLightTree*> slts = this->getSLTs();
  for (unsigned int i = 0; i < slts.size(); ++i) {
    this->getLightOctree()->addSLT(*(slts.at(i)), i);
  }
}


void HashedLightManager::constructEmptyLightOctree() {
  // Check if lights are not empty
  if (this->world.p_lights.empty()) throw HashedShadingNoLightException();

  // Find the extreme values
  world::PointLight* p_light = this->world.p_lights.at(0);
  glm::vec3 pmin;
  glm::vec3 pmax;

  glm::vec3 vmin = glm::vec3(p_light->position) - glm::vec3(p_light->radius);
  glm::vec3 vmax = glm::vec3(p_light->position) + glm::vec3(p_light->radius);

  for (unsigned int i = 1; i < this->world.p_lights.size(); ++i) {
    p_light = this->world.p_lights.at(i);

    pmin = glm::vec3(p_light->position) - glm::vec3(p_light->radius);
    pmax = glm::vec3(p_light->position) + glm::vec3(p_light->radius);

    if (pmin.x < vmin.x) vmin.x = pmin.x;
    if (pmin.y < vmin.y) vmin.y = pmin.y;
    if (pmin.z < vmin.z) vmin.z = pmin.z;

    if (pmax.x > vmax.x) vmax.x = pmax.x;
    if (pmax.y > vmax.y) vmax.y = pmax.y;
    if (pmax.z > vmax.z) vmax.z = pmax.z;
  }

  vmin -= glm::vec3(0.1 * this->getMinimalNodeSize());
  vmax += glm::vec3(0.1 * this->getMinimalNodeSize());

  // calculate max size
  double max_size = math::f_max(vmax.x - vmin.x,
                                math::f_max(vmax.y - vmin.y,
                                            vmax.z - vmin.z));

  unsigned int n_nodes_min = unsigned int(floor(max_size / this->getMinimalNodeSize())) + 1;
  unsigned int depth = unsigned int(ceil(log2(n_nodes_min))) + 1; // depth is defined as 2^(depth - 1) <= n_nodes

  this->p_light_octree = new LightOctree(vmin, depth, this->getMinimalNodeSize());
  this->has_constructed_light_octree = true;
}


void HashedLightManager::constructSLTs() {
  // remove previous SLTS
  if (this->has_constructed_slts)
    for (SingleLightTree* p_slt : this->getSLTs()) delete p_slt;

  // clear vector
  this->ps_slt.clear();

  // construct new slts
  SingleLightTreeBuilder builder = SingleLightTreeBuilder(this->getMinimalNodeSize(),
                                                          this->getLightOctree()->getOrigin());

  for (world::PointLight* p_light : this->getWorld().p_lights) {
    this->ps_slt.push_back(builder.constructSLT(*p_light));
  }
}

}
}
}