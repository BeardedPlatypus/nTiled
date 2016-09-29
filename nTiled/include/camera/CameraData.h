/*! @file CameraData.h
 *  @brief CameraData.h contains the definition of the CameraData struct.
 */
#pragma once

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <glm\glm.hpp>

namespace nTiled {
namespace camera {

/*! @brief The CameraData contains all data needed to store a single view of
 *         a single camera.
 */
struct CameraData {
  // constructor
  /*! @brief Construct a new CameraData with the given parameters.
   *
   * @param lookAt The look-at matrix of this new CameraData.
   * @param perspective_matrix The perspective matrix of this new CameraData.
   * @param depthrange The depth range of this new CameraData.
   */
  CameraData(glm::mat4 lookAt,
             glm::mat4 perspective_matrix,
             glm::vec2 depthrange);

  // member variables
  /*! @brief The look at matrix of this CameraData. */
  glm::mat4 lookAt;
  /*! @brief The perpsective matrix of this CameraData. */
  glm::mat4 perspective_matrix;
  /*! @brief The depth range of this CameraData. */
  glm::vec2 depthrange;
};

} // camera
} // nTiled