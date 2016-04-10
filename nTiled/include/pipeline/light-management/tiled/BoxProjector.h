#pragma once

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "pipeline\light-management\tiled\LightProjector.h"

namespace nTiled {
namespace pipeline {

class BoxProjector : public LightProjector {
public:
  // FIXME add better clipping
  // constructor
  BoxProjector();

  // compute method
  bool computeProjection(
    const world::PointLight& light,  // input
    const camera::Camera& camera,
    glm::uvec2 viewport,
    glm::uvec2 tilesize,
    glm::uvec4& projection) const;   // output

  bool computeNDCProjection(
    // Input
    const world::PointLight& light,
    const camera::Camera& camera,
    // Output
    glm::vec4& ndc_coordinates) const;

  bool compute2dProjection(
    // Input
    glm::vec2 pos_cameraspace,
    float cutoff_radius,
    // Output
    glm::vec2& B_star,
    glm::vec2& T_star) const;
};

} // pipeline 
} // nTiled