#pragma once

#include "camera\CameraData.h"
#include <imgui.h>

namespace nTiled {
namespace camera {
/*
 * Camera Control Interface
 */
class CameraControl {
 public:
  virtual ~CameraControl() {}
  virtual void update(const ImGuiIO& io,
                      CameraData& data) = 0;
  virtual void activate(const ImGuiIO& io) = 0;
};


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

} // camera
} // nTiled