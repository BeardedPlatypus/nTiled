/*! @file ClusteredLightManager.h
 *  @brief ClusteredLightManager.h contains the definition of the 
 *         ClusteredLightManager which serves as the interface for the 
 *         Clustered Shading algorithm.
 */
#pragma once

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <glad\glad.h>

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "state\StateView.h"
#include "world\World.h"

#include "pipeline\light-management\tiled\BoxProjector.h"

// ----------------------------------------------------------------------------
// compute shaders
// ----------------------------------------------------------------------------
#include "compute-client\KeyComputeShader.h"
#include "compute-client\KeySortAndCompactShader.h"
#include "LightClustering.h"

namespace nTiled {
namespace pipeline {

/*! @brief Clustered Light Manager is responsible for managing all parts of the
 *         Clustered Shading algorithm. It manages the compute shaders as well
 *         as the LightClustering datastructure.
 */
class ClusteredLightManager {
public:
  /*! @brief Construct a new ClusteredLightManager with the given parameters.
   * 
   * @param view Reference to the View this new ClusteredLightManager will 
   *             observe.
   * @param world Reference to the World this new ClusteredLightManager will
   *              observe.
   * @param tile_size The tile size in pixels used in this ClusteredShading
   *                  algorithm.
   * @param depth_texture openGL pointer to the depth texture used in the 
   *                      clustered shading algoritm.
   */
  ClusteredLightManager(const state::View& view,
                        const world::World& world,
                        glm::uvec2 tile_size,
                        GLuint depth_texture);

  /*! @brief Default ClusteredLightManager destructor. */
  ~ClusteredLightManager();

  /*! Construct a new Clustering frame to be used in the calculation of lights. */
  void constructClusteringFrame();

  // Accessors
  // ---------
  /*! @brief Get the data of the summed indices of this clustering
   */
  const std::vector<GLuint>& getSummedIndicesData() const;
  /*! @brief Get the openGL pointer to the k index textureimage object.
   */
  GLuint getKIndexMapPointer() const;

  /*! @brief Get the data of the light clusters buffer calculated in the light clustering
   */
  const std::vector<glm::uvec2>& getLightClusterData() const;
  /*! @brief Get the data of the light index buffer calculated in light_clustering
   */
  const std::vector<GLuint>& getLightIndexData() const;

protected:
  // -------------------------------------------------------------------------
  //  constructClusteringFrame sub-functions
  // -------------------------------------------------------------------------
  /*! @brief Compute the keys with the KeyComputeShader of this 
   *         ClusteredLightmanager. 
   */
  virtual void computeKeys();

  /*! @brief Sort and compact the keys with the SortAndCompactShader of this
   *         ClusteredLightManager which were computed with KeyComputeShader
   * 
   * This should be called after computeKeys()
   */
  virtual void sortAndCompactKeys();

  /*! @brief Clear the previous clustering stored in the LightClustering of 
   *         this ClusteredLightManager.
   */
  virtual void clearClustering();

  /*! @brief Build the clustering of this frame, based on the sorted and 
   *         compacted keys computed in computeKeys() and sortAndCompactKeys()
   */
  virtual void buildClustering();
  
  /*! @brief Finalise clustering such that it can be loaded into video memory
   *         and used for rendering.
   */
  virtual void finaliseClustering();

  // -------------------------------------------------------------------------
  //  Member variables
  // -------------------------------------------------------------------------
  /*! @brief LightProjector to be used to calculate the affected lights */
  const LightProjector& projector;
  /*! @brief World Reference of this ClusteredLightManager */
  const world::World& world;
  /*! @brief View Reference of this ClusteredLightManager */
  const state::View& view;
  /*! @brief Dimensions of each individual tile*/
  const glm::uvec2 tile_size;

  /*! @brief The vector used to store the summed indices produced by the 
   *         compute shader.
   */
  std::vector<GLuint> summed_indices;
  /*! @brief The LightClustering used in this ClusteredLightmanager. */
  clustered::LightClustering light_clustering;

  // --------------------------------------------------------------------------
  //  Compute shaders
  // --------------------------------------------------------------------------
  /*! @brief The KeyComputeShader of this ClusteredLightManager. */
  clustered::KeyComputeShader key_compute_shader;
  /*! @brief The KeySortAndCompactShader of this ClusteredLightManager. */
  clustered::KeySortAndCompactShader key_sort_compact_shader;

  /*! @brief The inversed denominator used in the compute shaders. */
  float k_inv_denominator;
};


/*! @brief The ClusteredLightManagerBuilder is responsible for constructing 
 *         ClusteredLightManagers.
 */
class ClusteredLightManagerBuilder {
public:
  /*! @brief Construct a new ClusteredLightManagerBuilder
  */
  ClusteredLightManagerBuilder();

  /*! @brief Construct a new ClusteredLightManager with the given parameters 
   *         and return a pointer to it.
   * 
   * @param view The View the new ClusteredLightManager will observe.
   * @param world The World the new ClusteredLightManager will observe.
   * @param tile_size The tile size in pixels used in this ClusteredShading
   *                  algorithm.
   * @param depth_texture openGL pointer to the depth texture used in the 
   *                      clustered shading algoritm.
   * 
   * @return Return a pointer to the newly constructed ClusteredLightManager.
   */
  virtual ClusteredLightManager* constructNewClusteredLightManager(
    const state::View& view, 
    const world::World& world,
    glm::uvec2 tile_size, 
    GLuint depth_texture) const;
};

} // pipeline
} // nTiled
