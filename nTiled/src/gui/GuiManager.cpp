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
GuiManager::GuiManager() {

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

  ImGuiIO& io = ImGui::GetIO();
  
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

void GuiManager::render() {
  ImGui::Render();
}

} // gui
} // nTiled