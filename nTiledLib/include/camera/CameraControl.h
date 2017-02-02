/*! @file CameraControl.h
 *  @brief CameraControl.h contains all the implemented CameraControl classes.
 * 
 * Currently the following Camera control methods are available: 
 * TurnTableCameraControl
 * PathCameraControl
 */
#pragma once

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <imgui.h>
#include <vector>
#include <glm\glm.hpp>


// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "camera\CameraData.h"


namespace nTiled {
namespace camera {

/*! @brief CameraControl specifies the interface to which each CameraControl 
 *         needs to adhere. 
 */
class CameraControl {
public:
  // --------------------------------------------------------------------------
  //  Virtual deconstructor
  // --------------------------------------------------------------------------
  virtual ~CameraControl() {}

  // --------------------------------------------------------------------------
  //  Methods
  // --------------------------------------------------------------------------
  /*! @brief Update the given CameraData based on the collected input.
   */
  virtual void update(const ImGuiIO& io,
                      CameraData& data) = 0;
  /*! @brief Activate this CameraControl given the current input. */
  virtual void activate(const ImGuiIO& io) = 0;

  /*! @brief Whether this CameraControl is user controlled or not. */
  virtual inline bool isUserControlled() = 0;
};


/*! @brief TurnTableCameraControl implements a TurnTable camera similar 
 *         to which is available in Blender.
 */
class TurnTableCameraControl : public CameraControl {
 public:
  // ---------------------------------------------------------------------------
  //  Constructors
  // ---------------------------------------------------------------------------
   /*! @brief Construct a new TurnTableCameraControl with the given 
    *         sensitivity.
    *  
    *  @param sensitivity The sensitivity of this new TurnTableCameraControl.
    * 
    * @post (new this).sensitivity == sensitivity
    */
  TurnTableCameraControl(double sensitivity);

  /*! @brief Construct a new TurnTableCameraControl with default sensitivity.
   */
  TurnTableCameraControl();

  // ---------------------------------------------------------------------------
  //  Member functions
  // ---------------------------------------------------------------------------
  void update(const ImGuiIO& io,
              CameraData& data);
  void activate(const ImGuiIO& io);

  inline bool isUserControlled() { return true; }

 private:
   /*! @brief The sensitivity of this TurnTableCameraControl. */
  double sensitivity;
  /*! @brief The previous position of the mouse in x and y pixel coordinates. */
  ImVec2 pos_prev;
};

class AutomaticRotateCameraControl : public CameraControl {
 public:
  // ---------------------------------------------------------------------------
  //  Constructors
  // ---------------------------------------------------------------------------
  AutomaticRotateCameraControl();
  AutomaticRotateCameraControl(double rotation_speed);

  // ---------------------------------------------------------------------------
  //  Member functions
  // ---------------------------------------------------------------------------
  void update(const ImGuiIO& io,
              CameraData& data);
  void activate(const ImGuiIO& io);

 private:
  double rotation_speed;
};

/*! @brief PathCameraControl implements a CameraControl which follows a 
 *         a specified path (series of look at matrices). 
 */
class PathCameraControl : public CameraControl {
public: 
  // --------------------------------------------------------------------------
  //  Constructors
  // --------------------------------------------------------------------------
  /*! @brief Construct a new PathCameraControl with the specified series of 
   *         look at matrices defined in frames.
   *
   *  @param frames A series of look at matrices specifying the path of this
   *                new PathCameraControl.
   */
  PathCameraControl(std::vector<glm::mat4> frames);

  // --------------------------------------------------------------------------
  //  Member functions
  // --------------------------------------------------------------------------
  void update(const ImGuiIO& io,
              CameraData& data);
  void activate(const ImGuiIO& io);

  inline bool isUserControlled() { return false; }
 private:
   /*! @brief The frames of this PathCameraControl. */
   const std::vector<glm::mat4> frames;
   /*! @brief Current frame of this PathCameraControl. */
   int current_frame = 0;
};

} // camera
} // nTiled