/*! @file GuiManager.h
 *  @brief GuiManager.h contains the definition of the GuiManager class as 
 *         well as supporting classes.
 */
#pragma once

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <imgui.h>
#include "gui/imgui_impl_glfw_gl3.h"

#include <vector>

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "state\State.h"


namespace nTiled {
namespace gui {

enum class GuiFocus;

/*! @brief The GuiManager manages the dear, Imgui user interface of nTiled.
 */
class GuiManager {
public:
  // ----------------------------------
  //  Constructor
  // ----------------------------------
  /*! @brief Construct a new GuiManager with the given State
   *  
   * @param state The state of a nTiled execution
   */
  GuiManager(state::State& state);

  // ----------------------------------
  //  Interaction methods
  // ----------------------------------
  /*! @brief Initialise this GuiManager with the given openGL context
   * 
   * @param window The openGL window created with GLFW
   */
  void init(GLFWwindow& window);

  /*! @brief Update the state of this nTiled application through this 
   *         GuiManager.
   */
  void update();

  /*! @brief Render the gui managed by this GuiManager to the screen
   */
  void render();

private:
  /*! @brief The state this GuiManager interacts with */
  state::State& state;

  // input handling
  /*! @brief Update the Camera of this nTiled execution with collected 
   *         user input
   * 
   * @param io The data collected by dear, ImGui library.
   */
  void updateCamera(const ImGuiIO& io);

  /*! @brief Whether camera has focus or the GUI manager */
  GuiFocus focus;

  // draw elements
  /*! @brief Draw the TopMenu bar*/
  void drawTopMenu();
};

/*! @brief GuiFocus describes which part of the user-interface has focus.
 */
enum class GuiFocus {
  Camera,
  Gui
};

} // gui
} // nTiled