#pragma once

#include "camera\CameraData.h"
#include <imgui.h>

#include <vector>
#include <glm\glm.hpp>

namespace nTiled {
namespace camera {
/*!
  Camera Control Interface
 */
class CameraControl {
 public:
  virtual ~CameraControl() {}
  virtual void update(const ImGuiIO& io,
                      CameraData& data) = 0;
  virtual void activate(const ImGuiIO& io) = 0;

  virtual inline bool isUserControlled() = 0;
};


/*!
 TurnTable Camera Control
 */
class TurnTableCameraControl : public CameraControl {
 public:
  //TODO: figure out a smarter way to deal with window instead of passing 
  // the whole reference to the window.
  // ---------------------------------------------------------------------------
  //  Constructors
  // ---------------------------------------------------------------------------
  TurnTableCameraControl();
  TurnTableCameraControl(double sensitivity);

  // ---------------------------------------------------------------------------
  //  Member functions
  // ---------------------------------------------------------------------------
  void update(const ImGuiIO& io,
              CameraData& data);
  void activate(const ImGuiIO& io);

  inline bool isUserControlled() { return true; }

 private:
  double sensitivity;
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

class PathCameraControl : public CameraControl {
public: 
  // --------------------------------------------------------------------------
  //  Constructors
  // --------------------------------------------------------------------------
  PathCameraControl(std::vector<glm::mat4> frames);

  // --------------------------------------------------------------------------
  //  Member functions
  // --------------------------------------------------------------------------
  void update(const ImGuiIO& io,
              CameraData& data);
  void activate(const ImGuiIO& io);

  inline bool isUserControlled() { return false; }
 private:
   const std::vector<glm::mat4> frames;
   /** Current frame of this PathCameraControl */
   int current_frame = 0;
};

} // camera
} // nTiled