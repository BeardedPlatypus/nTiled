/*! @file BoxProjector.h 
 *  @brief BoxProjector contains the BoxProjector class which is an
 *         implementation of the LightProjector class.
 */
#pragma once

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "pipeline\light-management\tiled\LightProjector.h"

namespace nTiled {
namespace pipeline {

/*! @brief BoxProjector is responsible for creating box projections of lights
 *         onto screen space coordinates.
 */
class BoxProjector : public LightProjector {
public:
  /*! @brief Construct a new BoxProjector. */
  BoxProjector();

  // compute method
  bool computeProjection(
    const world::PointLight& light,  // input
    const camera::Camera& camera,
    glm::uvec2 viewport,
    glm::uvec2 tilesize,
    glm::uvec4& projection) const;   // output

  /*! @brief Compute the Projection in Normalised Device Coordinates (NDC). 
   * 
   * @param light The PointLight that is projected onto the tiles.
   * @param camera The Camera used to project the light onto the grid.
   * @param ndc_coordinates the result of the projection in NDC
   *
   * @Rreturn True if a projection exists given the parameters. The result is 
   *          then returned through the ndc_coordinates.
   *          False if no projection exists. The ndc_projection parameter is 
   *          untouched.
   */
  bool computeNDCProjection(
    // Input
    const world::PointLight& light,
    const camera::Camera& camera,
    // Output
    glm::vec4& ndc_coordinates) const;

  /*! @brief Compute a 2d projection of a circle onto screen space in a single dimension.
   * 
   * @param pos_cameraspace The position in camera space of the sphere
   * @param cutoff_radius The radius of the sphere.
   * @param B_star The left position of the 2d projection onto a single dimension
   * @param T_star The right position of the 2d projection onta a single dimension
   */
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