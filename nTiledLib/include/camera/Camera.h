/*! @file Camera.h
 *  @brief Camera.h containst the specification of the Camera class.
 */
#pragma once

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <glm\glm.hpp>

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "camera\CameraControl.h"
#include "camera\CameraData.h"
#include "camera\CameraConstructionData.h"

namespace nTiled {
namespace camera {

/*! @brief The Camera class contains both the control and the current data of 
 *         a single camera in the scene.
 * 
 * The Camera class is the access point for other parts of the program to 
 * interact with the camera. It provides access methods for the data in
 * the camera as well as the methods to update the Camera to the current 
 * frame.
 */
class Camera {
public:
  // ---------------------------------------------------------------------------
  // Constructor
  // ---------------------------------------------------------------------------
  /*! @brief Construct a new Camera with the given CameraControl and 
   *         CameraConstructionData
   * 
   * @param control CameraControl used to control this Camera.
   * @param construction_data CameraConstructionData with which this camera
   *                          is innitialised
   */
  Camera(CameraControl* control,
         CameraConstructionData construction_data);
  /*! @brief Construct a new Camera with TurnTableControl and a default view.
   */
  Camera();

  // ---------------------------------------------------------------------------
  // Access Camera Data
  // ---------------------------------------------------------------------------
  /*! @brief Get the look-at matrix of this Camera. 
   *  @return The look at matrix of this Camera
   */
  glm::mat4 getLookAt() const;

  /*! @brief Get the perspective matrix of this Camera. 
   *  @return The perspective matrix of this Camera
   */
  glm::mat4 getPerspectiveMatrix() const;

  /*! @brief Get the depth range of this Camera, where x is the minimum depth
             and y the maximum depth. 
   *  @return The depth range of this Camera
   */
  glm::vec2 getDepthrange() const;

  // Access Camera construction Data
  /*! @brief Get the field of view of this Camera in degrees
   *  @return The field of view of this Camera in degrees
   */
  float getFoV() const;

  /*! @brief Update this Camera's data based on the input and this Camera's 
   *         CameraControl.
   *
   *  @param io A reference to the ImGuiIO used to calculate the new data 
   *            of this Camera.
   */
  void update(const ImGuiIO& io);

  /*! @brief Update this Camera to be the active Camera.
   *
   *  @param io A reference to the ImGuiIO used to calculate the new data of 
   *            this Camera
   */
  void toFocus(const ImGuiIO& io);

  /*! @brief Whether this Camera is controlled by the user.
   *  @return Whether this Camera is controlled by the user.
   */
  inline bool isUserControlled() { return this->control->isUserControlled(); }

private:
  /*! @brief The CameraControl of this Camera. */
  CameraControl* control;
  /*! @brief The data with which this Camera was constructed. */
  CameraConstructionData construction_data;
  /*! @brief The current data of this Camera with which the different matrices 
   *         are calculated. 
   */
  CameraData data;
  /*! @brief Whether this Camera is active. */
  bool is_active;
};

} // camera
} // nTiled