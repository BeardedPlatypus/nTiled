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

private:
  /*! LightProjector to be used to calculate the affected lights */
  const LightProjector& projector;
  /*! World Reference of this ClusteredLightManager */
  const world::World& world;
  /*! View Reference of this ClusteredLightManager */
  const state::View& view;
  /*! Dimensions of each individual tile*/
  const glm::uvec2 tile_size;

  clustered::LightClustering light_clustering;

  // --------------------------------------------------------------------------
  //  Compute shaders
  // --------------------------------------------------------------------------
  clustered::KeyComputeShader key_compute_shader;
  clustered::KeySortAndCompactShader key_sort_compact_shader;

  float k_inv_denominator;

};

} // pipeline
} // nTiled
