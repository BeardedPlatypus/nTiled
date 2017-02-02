#include "gui\GuiManager.h"

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <GLFW\glfw3.h>


namespace nTiled {
namespace gui {

// ----------------------------------------------------------------------------
//  Constructor
// ----------------------------------------------------------------------------
GuiManager::GuiManager(state::State& state) : state(state) {

}

// ----------------------------------------------------------------------------
//  Interaction Methods
// ----------------------------------------------------------------------------
void GuiManager::init(GLFWwindow& window) {
  ImGui_ImplGlfwGL3_Init(&window, true);
}

void GuiManager::update() {
  // Poll events and write to ImGUI.
  glfwPollEvents();
  ImGui_ImplGlfwGL3_NewFrame();

  // Update user with user input
  ImGuiIO& io = ImGui::GetIO();
  this->updateCamera(io);

  // Draw elements
  this->drawTopMenu();
  
}

void GuiManager::render() {
  ImGui::Render();
}

// ----------------------------------------------------------------------------
//  Update methods
void GuiManager::updateCamera(const ImGuiIO& io) {
  if (((!io.WantCaptureMouse || !io.WantCaptureKeyboard) && io.MouseDown[0]) || 
      (!this->state.view.camera.isUserControlled())) {
    // we assume that we're interested in seeing if we need to update the
    // camera whenever none of the GUI elements is active, or when the 
    // camera does not depend on user_input
    if (focus == GuiFocus::Camera) {
      this->state.view.camera.update(io);
    } else {
      this->state.view.camera.toFocus(io);
      this->focus = GuiFocus::Camera;
    }
  } else if (focus == GuiFocus::Camera) {
    this->focus = GuiFocus::Gui;
  }
}

// ----------------------------------------------------------------------------
//  Draw methods
void GuiManager::drawTopMenu() {
  // menu
  if (ImGui::BeginMainMenuBar()) {
    // menu tab
    if (ImGui::BeginMenu("Menu")) {
      ImGui::EndMenu();
    }
    // examples tab
    if (ImGui::BeginMenu("Examples")) {
      ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();
  }
}

} // gui
} // nTiled