/*! @file LightProjector.h
 *  @brief LightProjector.h contains the definition of the LightProjector 
 *         class
 */
#pragma once

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <vector>

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "world\PointLight.h"
#include "camera\Camera.h"


namespace nTiled {
namespace pipeline {

/*! @brief LightProjector is responsible for calculating the projection
 *         of lights onto screenspace. It serves as a base class
 *         for all different LightProjectors.
 */
class LightProjector {
public:
  virtual ~LightProjector() {}

  /*! @brief Compute the projection of light in to the tiles 
   *
   * @param light The PointLight that is projected onto the tiles.
   * @param camera The Camera used to project the light onto the grid.
   * @param viewport Size of the viewport in pixels
   * @param tilesize Size of a single tile in pixels
   * @param projection The result of the projection in tile indices
   *
   * @Rreturn True if a projection exists given the parameters. The result is 
   *          then returned through the projection parameter.
   *          False if no projection exists. The projection parameter is 
   *          untouched.
   */
  virtual bool computeProjection(
    const world::PointLight& light,
    const camera::Camera& camera,
    glm::uvec2 viewport,
    glm::uvec2 tilesize,
    glm::uvec4& projection) const = 0;
};


} // pipeline
} // nTiled

