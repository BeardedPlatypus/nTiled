// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
//  openGL
#include <glad\glad.h>
#include <GLFW\glfw3.h>

//  output
#include <iostream>

// GUI
#include <imgui.h>
#include "gui/imgui_impl_glfw_gl3.h"


// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "gui\GuiManager.h"
#include "world\World.h"
#include "state\State.h"
#include "pipeline\forward\ForwardPipeline.h"
#include "pipeline\deferred\DeferredPipeline.h"
#include "pipeline\debug-view\DebugPipeline.h"


// ----------------------------------------------------------------------------
//  Defines
// ----------------------------------------------------------------------------
#define WIDTH 1200
#define HEIGHT 1200

#define SCENE_PATH std::string("C:/Users/Monthy/Documents/projects/thesis/scenes/scene-definitions/test_2/scene.json")

// ----------------------------------------------------------------------------
//  Function prototypes
// ----------------------------------------------------------------------------
void key_callback(GLFWwindow* window, 
                  int key, 
                  int scancode, 
                  int action, 
                  int mode);


// ----------------------------------------------------------------------------
//  Main
// ----------------------------------------------------------------------------
int main() {
  // Create window
  // -------------
  glfwInit();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  GLFWwindow* window = glfwCreateWindow(WIDTH,                     // width
                                        HEIGHT,                    // height
                                        "nTiled - openGL viewer",  // name
                                        NULL,                      // p_monitor
                                        NULL);                     // p_share
  glfwMakeContextCurrent(window);

  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }

  // Set Key callback
  // ----------------
  glfwSetKeyCallback(window, key_callback);

  // load glad
  // ---------
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize OpenGL context" << std::endl;
    return -1;
  }

  glViewport(0, 0, WIDTH, HEIGHT);

  // nTiled components
  // -----------------
  nTiled::state::State state = nTiled::state::constructStateFromJson(SCENE_PATH);

  nTiled::pipeline::Pipeline* p_render_pipeline;

  if (state.shading.pipeline_type == nTiled::pipeline::PipelineType::Forward) {
     p_render_pipeline = new nTiled::pipeline::ForwardPipeline(state);
  } else {
    p_render_pipeline = new nTiled::pipeline::DeferredPipeline(state);
  }

  nTiled::pipeline::Pipeline* p_pipeline;
  if (state.shading.is_debug) {
    p_pipeline = new nTiled::pipeline::DebugPipeline(p_render_pipeline,
                                                     state);
  } else {
    p_pipeline = p_render_pipeline;
  }



  nTiled::gui::GuiManager gui_manager = nTiled::gui::GuiManager(state);
  gui_manager.init(*window);

  // Render Loop
  // -----------
  while (!glfwWindowShouldClose(window)) {
    // update nTiled components
    gui_manager.update();

    // Clear colour buffer
    glClearColor(0, 0, 0, 1);
    glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // render nTiled components
    p_pipeline->render();
    gui_manager.render();

    // Display on screen
    glfwSwapBuffers(window);
  }

  // Terminate program
  // -----------------
  // clean up
  delete p_pipeline;
  if (state.shading.is_debug) {
    delete p_render_pipeline;
  }

  glfwTerminate();
  return 0;
}


void key_callback(GLFWwindow* window, 
                  int key, 
                  int scancode, 
                  int action, 
                  int mode) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
}