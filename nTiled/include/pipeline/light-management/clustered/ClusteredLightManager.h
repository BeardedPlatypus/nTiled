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

// compute shaders
#include "compute-client\KeyComputeShader.h"
#include "compute-client\KeySortAndCompactShader.h"
#include "LightClustering.h"

namespace nTiled {
namespace pipeline {

class ClusteredLightManager {
public:
  ClusteredLightManager(const state::View& view,
                        const world::World& world,
                        glm::uvec2 tile_size,
                        GLuint depth_texture);
  ~ClusteredLightManager();

  void constructClusteringFrame();

  // Accessors
  // ---------
  /*!
   Get the data of the summed indices of this clustering
   */
  const std::vector<GLuint>& getSummedIndicesData() const;
  /*!
   Get the openGL pointer to the k index textureimage object.
   */
  GLuint getKIndexMapPointer() const;

  /*!
   Get the data of the light clusters buffer calculated in the light clustering
   */
  const std::vector<glm::uvec2>& getLightClusterData() const;
  /*!
   Get the data of the light index buffer calculated in light_clustering
   */
  const std::vector<GLuint>& getLightIndexData() const;

protected:
  // -------------------------------------------------------------------------
  //  constructClusteringFrame sub-functions
  // -------------------------------------------------------------------------
  // TODO: documentation
  virtual void computeKeys();
  virtual void sortAndCompactKeys();
  virtual void clearClustering();
  virtual void buildClustering();
  virtual void finaliseClustering();

  // -------------------------------------------------------------------------
  //  Member variables
  // -------------------------------------------------------------------------
  /*! LightProjector to be used to calculate the affected lights */
  const LightProjector& projector;
  /*! World Reference of this ClusteredLightManager */
  const world::World& world;
  /*! View Reference of this ClusteredLightManager */
  const state::View& view;
  /*! Dimensions of each individual tile*/
  const glm::uvec2 tile_size;

  std::vector<GLuint> summed_indices;
  clustered::LightClustering light_clustering;

  // --------------------------------------------------------------------------
  //  Compute shaders
  // --------------------------------------------------------------------------
  clustered::KeyComputeShader key_compute_shader;
  clustered::KeySortAndCompactShader key_sort_compact_shader;

  float k_inv_denominator;
};


class ClusteredLightManagerBuilder {
public:
  /*! 
   Construct a new ClusteredLightManagerBuilder
  */
  ClusteredLightManagerBuilder();

  /*!
   Construct a new ClusteredLightManager with the given parameters and return a 
   pointer to it.
   */
  virtual ClusteredLightManager* constructNewClusteredLightManager(
    const state::View& view, const world::World& world,
    glm::uvec2 tile_size, GLuint depth_texture) const;
};

} // pipeline
} // nTiled
