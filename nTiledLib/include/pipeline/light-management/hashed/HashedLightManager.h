#pragma once

// ----------------------------------------------------------------------------
//  nTiled Headers
// ----------------------------------------------------------------------------
#include "world\World.h"
#include "light-octree\LightOctree.h"
#include "linkless-octree\LinklessOctree.h"
#include "HashedConfig.h"


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
   * @param world The world this HashedLightManager is observing
   * @param hashed_config The configuration values for this 
   *                      HashedLightManager
   */
  HashedLightManager(const world::World& world,
                     HashedConfig hashed_config);
                     

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

  /*! @brief Get the starting depth of the LinklessOctree of this 
   *         HashedLightManager
   *
   * @returns The starting depth of the LinklessOctree of this 
   *          HashedLightmanager
   */
  unsigned int getStartingDepth() const { return this->starting_depth; }

  /*! @brief Get the increase ratio of r for the construction of the 
   *         LinklessOctree.
   * 
   * @return The increase ratio of r for the construction of the LinklessOctree
   */
  double getRIncreaseRatio() const { return this->r_increase_ratio; }

  /*! @brief Get the maximum number of attempts to construct a 
   *         SpatialHashFunction of the LinklessOctree of this 
   *         HashedLightManager.
   *
   * @return The maximum number of attempts to construct a SpatialHashFunction
   */
  unsigned int getMaxNAttempts() const { return this->max_attempts; }

  /*! @brief Get the reference to the world of this HashedLightManager. 
   *
   * @returns The world this HashedLightManager depicts
   */
  const world::World& getWorld() const { return this->world; }

  /*! @brief Geth the LinklessOctree associated with this HashedLightManager. 
   * 
   * @returns The pointer to the LinklessOctree of this HashedLightManager
   */
  LinklessOctree* getLinklessOctree() { return this->p_linkless_octree; }

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
  /*! @brief Initialise this HashedLightmanager by building all relevant 
   *         datastructures.
   */
  void init();

  //TODO keep track whether this is called for memory management purposes
  /*! @brief Construct a new LightOctree based on the lights in the world 
   *         associated with this HashedLightManager. 
   * 
   *         The new LightOctree can be obtained with 
   *         (new this)->getLightOctree();
   */
  virtual void constructLightOctree();

  /*! @brief Construct a new empty LightOctree with the size and origin
   *         such that it will fit all the lights in the world associated
   *         with this HashedLightManager
   */
  virtual void constructEmptyLightOctree();

  /*! @brief Construct a SingleLightTree for each light in the world associated
   *         with this HashedLightManager. these can be obtained with
   *         (new this)->getSLTs();
   */
  virtual void constructSLTs();

  /*! @Brief Add the constructed SingleLightTrees to the current LightOctree
   */
  virtual void addConstructedSLTs();
  
  // --------------------------------------------------------------------------
  //  LinklessOctree Construction methods
  // --------------------------------------------------------------------------
  /*! @brief Construct a new LinklessOctree based on the lights in the 
   *         in the world associated with this HashedLightManager.
   */
  virtual void constructLinklessOctree();

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

  // --------------------------------------------------------------------------
  //  LinklessOctree variables
  // --------------------------------------------------------------------------
  /*! @brief Pointer to the LinklessOctree associated with this HashedShadingManager*/
  LinklessOctree* p_linkless_octree;

  /*! @brief Whether this HashedLightManager has constructed a LinklessOctree. */
  bool has_constructed_linkless_octree;

  /*! @brief The starting depth of the LinklessOctree. */
  unsigned int starting_depth;

  /*! @brief The increase ratio used in the construction of the LinklessOctree. */
  double r_increase_ratio;

  /*! @brief The max_attempts used to construct a LinklessOctree. */
  unsigned int max_attempts;

  /*! @brief The seed used in the random number generator constructed hashfunctions. */
  unsigned int hash_builder_seed;
};


}
}
}
