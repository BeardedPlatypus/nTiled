#pragma once

// ----------------------------------------------------------------------------
//  nTiled Headers
// ----------------------------------------------------------------------------
#include "world\World.h"
#include "light-octree\LightOctree.h"

namespace nTiled {
namespace pipeline {
namespace hashed {

class HashedLightManager {
public:
  // --------------------------------------------------------------------------
  //  Constructor | Destructor
  // --------------------------------------------------------------------------
  /*! @brief Construct a new HashedLightManager with the given parameters
   *
   * @param world A constant reference to the world this HashedLightManager
   *              observes
   * @param minimal_node_size The minimal node size 
   */
  HashedLightManager(const world::World& world,
                     double minimal_node_size);

  /*! @brief Destruct this HashedLightManager.
   */
  ~HashedLightManager();

  // --------------------------------------------------------------------------
  //  Get | Set methods
  // --------------------------------------------------------------------------
  /*! @brief Get the minimal node size used within hashed shading.
   *
   * @returns the minimal node size used within hashed shading.
   */
  double getMinimalNodeSize() const { return this->minimal_node_size; }

  /*! @brief Get the reference to the world of this HashedLightManager. 
   *
   * @returns The world this HashedLightManager depicts
   */
  const world::World& getWorld() const { return this->world; }

  /*! @brief Get the LightOctree associated with this HashedLightManager
   * 
   * @returns The pointer to the LightOctree of this HashedLightmanager
   */
  LightOctree* getLightOctree() { return this->p_light_octree; }

  /*! @brief Get pointers to all the SingleLightTree currently associated with
   *         this HashedLightManager
   * 
   * @returns a vector containing pointers to all the SingleLightTrees
   */
  const std::vector<SingleLightTree*>& getSLTs() const { return this->ps_slt; }


  // --------------------------------------------------------------------------
  //  LightOctree Construction methods
  // --------------------------------------------------------------------------
  //TODO keep track whether this is called for memory management purposes
  /*! @brief Construct a new LightOctree based on the lights in the world 
   *         associated with this HashedLightManager. 
   * 
   *         The new LightOctree can be obtained with 
   *         (new this)->getLightOctree();
   */
  void constructLightOctree();

  /*! @brief Construct a new empty LightOctree with the size and origin
   *         such that it will fit all the lights in the world associated
   *         with this HashedLightManager
   */
  void constructEmptyLightOctree();

  /*! @brief Construct a SingleLightTree for each light in the world associated
   *         with this HashedLightManager. these can be obtained with
   *         (new this)->getSLTs();
   */
  void constructSLTs();
  
  // --------------------------------------------------------------------------
  //  LinklessOctree Construction methods
  // --------------------------------------------------------------------------

private:
  // --------------------------------------------------------------------------
  //  general variables
  // --------------------------------------------------------------------------
  /*! @brief The minimal node size used within hashed shading. */
  double minimal_node_size;

  /*! @brief The world this hashed light manager depicts. */
  const world::World& world;

  // --------------------------------------------------------------------------
  //  LightOctree variables
  // --------------------------------------------------------------------------
  /*! @brief Pointer to the LightOctree associated with this HashedShadingManager. */
  LightOctree* p_light_octree;

  /*! @brief Whether this HashedLightManager has constructed a LightOctree. */
  bool has_constructed_light_octree;

  /*! @brief Set of SingleLightTrees associated with this HashedShadingManager. */
  std::vector<SingleLightTree*> ps_slt;

  /*! @brief Whether this HashedLightManager has constructed slts. */
  bool has_constructed_slts;
};

}
}
}
