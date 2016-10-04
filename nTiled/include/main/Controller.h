/*! @file Controller.h
 *  @brief Controller.h contains the definition of the Controller
 *         which is the outermost layer responsible for executing nTiled.
 */
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
/*! @brief The Controller class takes care of the main execution of the 
 *         program.
 */
class Controller {
public:
  /*! @brief Construct a new Controller with the given scene path
   *
   * @param scene_path Path to the scene configuration json this nTiled execution 
   *                   should run.
   */
  Controller(const std::string& scene_path);

  /*! @brief Destruct this Controller. */
  ~Controller();

  // --------------------------------------------------------------------------
  //  initialise methods
  // --------------------------------------------------------------------------
  /*! @brief Initialse this Controller.
   */
  void initialise();

  /*! @brief Render a frame with this Controller. */
  void render();

  // --------------------------------------------------------------------------
  //  Execute methods
  // --------------------------------------------------------------------------
  /*! @brief Set the DrawMethod of this Controller to the specified draw_method
   *
   * @param draw_method The new draw method of this controller.
   */
  void setDrawMethod(DrawMethod* draw_method);

  /*! @brief Set logging data of this Controller to activate
   *
   * @param activate Bool to which logging data should be set.
   */
  void toggleLoggingData(bool activate);

  /*! @brief Export the logged data.
   */
  void exportLogData();

  /*! @brief Stop executing nTiled 
   */
  void exit();

private:
  // --------------------------------------------------------------------------
  //  Initialise methods
  // --------------------------------------------------------------------------
  /*! @brief Initialise the state 
   */
  void initialiseState();

  /*! @brief Initialise the openGL context. 
   */
  void initialiseOpenGL();

  /*! @brief Initialise all components of nTiled. 
   */
  void initialiseNTiledComponents();

  /*! @brief Initialise all frame events specified in the scene configuration.
   */
  void initialiseFrameEvents();

  // --------------------------------------------------------------------------
  //  RenderLoop functions
  // --------------------------------------------------------------------------
  /*! @brief Render */
  void renderLoopDefault();

  /*! @brief Render and update queued events. */
  void renderLoopEventQueue();

  // --------------------------------------------------------------------------
  //  Attributes
  // --------------------------------------------------------------------------
  /*! @brief Pointer to the GLFWwindow of this Controller. */
  GLFWwindow* p_window;

  /*! @brief Pointer to the state of this Controller. */
  state::State* p_state;

  /*! @brief Pointer to the Pipeline of this Controller. */
  pipeline::Pipeline* p_pipeline;

  /*! @brief Pointer to the GuiManager of this Controller. */
  gui::GuiManager* p_gui_manager;

  /*! @brief Clock object of this Controller. */
  Clock clock;

  /*! @brief Pointer to the ExecutiontimeLogger of this Controller. */
  logged::ExecutionTimeLogger* p_logger;
  
  // --------------------------------------------------------------------------
  // FrameEvents
  /*! @brief priority queue managing all execution events. */
  std::priority_queue<FrameEvent*,
                      std::vector<FrameEvent*>,
                      FrameEventCompare> event_queue;

  // --------------------------------------------------------------------------
  /*! @brief Scene path of this controller*/
  const std::string scene_path;

  /*! @brief Pointer to the current DrawMethod of this controller. */
  DrawMethod* current_draw_method;
 
  /*! @brief DrawMethods used during the execution of this nTiled. */
  std::vector<DrawMethod*> draw_methods;

  /*! @brief Whether this nTiled execution should close after finishing 
   *         executing.
   */
  bool should_close;
};

}
