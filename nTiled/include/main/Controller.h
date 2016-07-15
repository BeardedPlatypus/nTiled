#pragma once


// ----------------------------------------------------------------------------
//  System Libraries 
// ----------------------------------------------------------------------------
//  openGL
#include <glad\glad.h>
#include <GLFW\glfw3.h>
// std
#include <queue>
#include <iostream>
// gui
#include <imgui.h>
#include "gui/imgui_impl_glfw_gl3.h"

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "gui\GuiManager.h"
#include "world\World.h"
#include "state\State.h"
#include "pipeline\Pipeline.h"

#include "log\Logger.h"

#include "Clock.h"
#include "FrameEvent.h"
#include "DrawMethod.h"


namespace nTiled {

// ----------------------------------------------------------------------------
//  Controller
// ----------------------------------------------------------------------------
/*! 
 The Controller class takes care of the main execution of the program.
 */
class Controller {
public:
  Controller(const std::string& scene_path);
  ~Controller();

  // --------------------------------------------------------------------------
  //  initialise methods
  // --------------------------------------------------------------------------
  void initialise();
  void render();


  // --------------------------------------------------------------------------
  //  Execute methods
  // --------------------------------------------------------------------------
  /*!
   Set the DrawMethod of this Controller to the specified draw_method

   Param:
     draw_method (const DrawMethod&): the new draw method of this controller
   */
  void setDrawMethod(DrawMethod* draw_method);
  void toggleLoggingData(bool activate);
  void exportLogData();
  void exit();

private:
  // --------------------------------------------------------------------------
  //  Initialise methods
  // --------------------------------------------------------------------------
  void initialiseState();
  void initialiseOpenGL();
  void initialiseNTiledComponents();
  void initialiseFrameEvents();

  // --------------------------------------------------------------------------
  //  RenderLoop functions
  // --------------------------------------------------------------------------
  void renderLoopDefault();
  void renderLoopEventQueue();

  // --------------------------------------------------------------------------
  //  Attributes
  // --------------------------------------------------------------------------
  GLFWwindow* p_window;

  state::State* p_state;
  pipeline::Pipeline* p_pipeline;
  gui::GuiManager* p_gui_manager;

  Clock clock;
  logged::ExecutionTimeLogger* p_logger;
  
  // --------------------------------------------------------------------------
  // FrameEvents
  std::priority_queue<FrameEvent*,
                      std::vector<FrameEvent*>,
                      FrameEventCompare> event_queue;

  // --------------------------------------------------------------------------
  /*! Scene path of this controller*/
  const std::string scene_path;

  /*! DrawMethod of this controller. */
  DrawMethod* current_draw_method;
  
  std::vector<DrawMethod*> draw_methods;

  bool should_close;
};

}
