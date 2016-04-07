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

class GuiManager {
public:
  // ----------------------------------
  //  Constructor
  // ----------------------------------
  /*!
   * Construct a new GuiManager
   */
  GuiManager(state::State& state);

  // ----------------------------------
  //  Interaction methods
  // ----------------------------------
  void init(GLFWwindow& window);

  /*!
   * Update the state of this nTiled application through this GuiManager
   */
  void update();

  /*!
   * Render the state of this GuiManager on the screen
   */
  void render();

private:
  /*! The state this GuiManager interacts with */
  state::State& state;

  // input handling
  void updateCamera(const ImGuiIO& io);
  /*! Whether camera has focus or the GUI manager */
  GuiFocus focus;

  // draw elements
  void drawTopMenu();
};

enum class GuiFocus {
  Camera,
  Gui
};

} // gui
} // nTiled