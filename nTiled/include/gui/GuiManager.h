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


namespace nTiled {
namespace gui {

class GuiManager {
public:
  // ----------------------------------
  //  Constructor
  // ----------------------------------
  /*!
   * Construct a new GuiManager
   */
  GuiManager();

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
  // elements



};

} // gui
} // nTiled