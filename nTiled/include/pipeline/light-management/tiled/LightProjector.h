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

class LightProjector {
public:
  virtual ~LightProjector() {}

  /*!
   Compute the projection of light in to the tiles 

   Param:
       light (const PointLight&): the light that is projected onto the tiles.
       camera (const Camera&): the camera used to project the light onto 
                               the grid.
       viewport (uvec2): size of the viewport in pixels
       tilesize (uvec2): size of a single tile in pixels
       projection (uvec4&): the result of the projection in tile indices
   Return:
       True if a projection exists given the parameters. The result is then 
       returned through the projection parameter.
       False if no projection exists. The projection parameter is untouched.
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

