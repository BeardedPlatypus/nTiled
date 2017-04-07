#include "pipeline\light-management\hashed\HashedLightManager.h"

// ----------------------------------------------------------------------------
//  nTiled Headers
// ----------------------------------------------------------------------------
#include "pipeline\light-management\hashed\light-octree\slt\SingleLightTreeBuilder.h"
#include "pipeline\light-management\hashed\Exceptions.h"
#include "pipeline\light-management\hashed\light-octree\nodes\LOBranch.h"
#include "pipeline\light-management\hashed\light-octree\nodes\LOLeaf.h"

#include "pipeline\light-management\hashed\linkless-octree\SpatialHashFunctionBuilder.h"

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
                                       HashedConfig hashed_config) :
  world(world),
  minimal_node_size(hashed_config.minimum_node_size),
  starting_depth(hashed_config.starting_depth),
  r_increase_ratio(hashed_config.r_increase_ratio),
  max_attempts(hashed_config.max_attempts),
  ps_slt({}),
  has_constructed_light_octree(false),
  has_constructed_slts(false),
  has_constructed_linkless_octree(false) {
}


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
void HashedLightManager::init() {
  this->constructLightOctree();
  this->constructLinklessOctree();
}


void HashedLightManager::constructLinklessOctree() {
  // Check if the depth is compatible
  if (this->getLightOctree()->getDepth() <= this->getStartingDepth()) {
    throw HashedShadingInvalidStartingDepthException();
  }

  SpatialHashFunctionBuilder<glm::u8vec2> octree_map_builder =
    SpatialHashFunctionBuilder<glm::u8vec2>();

  SpatialHashFunctionBuilder<glm::uvec2> data_map_builder =
    SpatialHashFunctionBuilder<glm::uvec2>();

  std::vector<std::pair<glm::uvec3, const LOBranch*>> nodes_at_depth =
    this->getLightOctree()->retrieveNodesAtDepth(this->getStartingDepth());

  std::vector<std::pair<glm::uvec3, const LOBranch*>> branches =
    std::vector<std::pair<glm::uvec3, const LOBranch*>>(nodes_at_depth);
  std::vector<std::pair<glm::uvec3, const LOBranch*>> branches_next = {};
  std::vector<std::pair<glm::uvec3, const LOLeaf*>> leaves = {};

  std::vector<std::pair<glm::uvec3, glm::u8vec2>> octree_data = {};
  std::vector<std::pair<glm::uvec3, glm::uvec2>> light_data = {};

  std::vector<GLuint>* p_light_indices = new std::vector<GLuint>();
  std::vector<SpatialHashFunction<glm::u8vec2>*>* p_octree_maps = 
    new std::vector<SpatialHashFunction<glm::u8vec2>*>();
  std::vector<SpatialHashFunction<glm::uvec2>*>* p_data_maps = 
    new std::vector<SpatialHashFunction<glm::uvec2>*>();
  std::vector<bool>* p_data_map_exists = new std::vector<bool>();

  glm::uvec3 child_pos;
  glm::bvec3 index_vec;
  unsigned int index_int;
  glm::u8vec2 child_representation;
  std::vector<GLuint> node_light_indices;
  glm::uvec2 light_dat;

  while (!branches.empty()) {
    branches_next.clear();
    leaves.clear();
    octree_data.clear();
    light_data.clear();

    // calculate octree structure, leaf nodes, and branch nodes for the next level
    for (std::pair<glm::uvec3, const LOBranch*> p_b : branches) {
      glm::u8vec2 octree_dat = glm::u8vec2(0);

      for (unsigned int x = 0; x < 2; ++x) {
        for (unsigned int y = 0; y < 2; ++y) {
          for (unsigned int z = 0; z < 2; ++z) {
            index_vec = glm::bvec3(x == 1,
                                   y == 1,
                                   z == 1);
            index_int = x + y * 2 + z * 4;

            const LONode& child_node = p_b.second->getChildNodeConst(index_vec);

            child_representation = child_node.getLinklessOctreeNodeRepresentation();
            octree_dat += glm::u8vec2(child_representation.x << index_int,
                                      child_representation.y << index_int);

            child_node.addToConstructionVectors(p_b.first + p_b.first + glm::uvec3(x, y, z),
                                                branches_next,
                                                leaves);
          }
        }
      }
      octree_data.push_back(std::pair<glm::uvec3, glm::u8vec2>(p_b.first, octree_dat));
    }

    // compile light data
    for (std::pair<glm::uvec3, const LOLeaf*> p_l : leaves) {
      if (!p_l.second->isEmpty()) {
        node_light_indices = p_l.second->getIndices();
        light_dat = glm::uvec2(p_light_indices->size(), node_light_indices.size());
        light_data.push_back(std::pair<glm::uvec3, glm::uvec2>(p_l.first, light_dat));

        p_light_indices->insert(p_light_indices->end(),
                                node_light_indices.begin(),
                                node_light_indices.end());
      }
    }

    // create relevant maps
    p_octree_maps->push_back(octree_map_builder.constructHashFunction(octree_data,
                                                                      this->getMaxNAttempts(),
                                                                      this->getRIncreaseRatio()));

    if (!light_data.empty()) {
      p_data_map_exists->push_back(true);
      p_data_maps->push_back(data_map_builder.constructHashFunction(light_data,
                                                                    this->getMaxNAttempts(),
                                                                    this->getRIncreaseRatio()));
    } else {
      p_data_map_exists->push_back(false);
      p_data_maps->push_back(nullptr);
    }

    branches = branches_next;
  }

  this->p_linkless_octree = new LinklessOctree(this->getLightOctree()->getDepth(),
                                               p_octree_maps->size(),
                                               this->getLightOctree()->getMinimalNodeSize(),
                                               this->getLightOctree()->getOrigin(),
                                               p_octree_maps,
                                               p_data_map_exists,
                                               p_data_maps,
                                               p_light_indices);

  this->has_constructed_linkless_octree = true;

  // clean up the constructed nodes
  for (std::pair<glm::uvec3, const LOBranch*> p_b : nodes_at_depth)
    delete p_b.second;
}


void HashedLightManager::constructLightOctree() {
  this->constructEmptyLightOctree();
  this->constructSLTs();
  this->addConstructedSLTs();
}


void HashedLightManager::addConstructedSLTs() {
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