/*! @file CameraConstructionData.h
 *  @brief CameraConstructionData.h contains the definition of the 
 *         CameraConstructionData struct.
 */
#pragma once

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <glm\glm.hpp>


namespace nTiled {
namespace camera {

/*! @brief The CameraConstructionData contains all data needed for a Camera
 *         to construct its internal data.
 */
struct CameraConstructionData {
  /*! @brief Construct a new CameraConstructionData struct with the given 
   *         parameters.
   * 
   * @param camera_eye  The place from which the new Camera will look.
   * @param camera_center The center to which the new Camera will look.
   * @param camera_up The up-vector of the new Camera.
   * @param fovy The field of view (in degrees) in the y direction.
   * @param aspect The aspect ratio of the new Camera.
   * @param z_near The distance of the near clipping field.
   * @param z_far The distance of the far clipping field.
   */
  CameraConstructionData(glm::vec3 camera_eye,
                         glm::vec3 camera_center,
                         glm::vec3 camera_up,
                         float fovy,
                         float aspect,
                         float z_near,
                         float z_far);

  // look at
  /*! @brief The place from which the new Camera will look. */
  glm::vec3 camera_eye;
  /*! @brief The center to which the new Camera will look. */
  glm::vec3 camera_center;
  /*! @brief The up vectior of the new Camera. */
  glm::vec3 camera_up;

  // perspective
  /*! @brief The field of view in the y direction in degrees. */
  float fovy;
  /*! @brief the ascept ration of the new Camera. */
  float aspect;

  // distance
  /*! @brief The distance of the near clipping field. */
  float z_near;
  /*! @brief The distance of the far clipping field. */
  float z_far;
};

} // camera
} // nTiled
