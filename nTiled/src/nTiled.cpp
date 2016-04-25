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

#include "pipeline\light-management\clustered\compute-client\KeyComputeShader.h"
#include "pipeline\deferred\shaders\DeferredClusteredShader.h"
#include "pipeline\light-management\clustered\compute-client\TestComputeShader.h"


// ----------------------------------------------------------------------------
//  Defines
// ----------------------------------------------------------------------------
#define SCENE_PATH std::string("C:/Users/Monthy/Documents/projects/thesis/scenes/scene-definitions/test_2/scene.json")

#define VERT_PATH_GEO std::string("C:/Users/Monthy/Documents/projects/thesis/implementation_new/nTiled/nTiled/src/pipeline/deferred/shaders-glsl/lambert_gbuffer.vert")
#define FRAG_PATH_GEO std::string("C:/Users/Monthy/Documents/projects/thesis/implementation_new/nTiled/nTiled/src/pipeline/deferred/shaders-glsl/lambert_gbuffer.frag")

#define VERT_PATH_LIGHT std::string("C:/Users/Monthy/Documents/projects/thesis/implementation_new/nTiled/nTiled/src/pipeline/deferred/shaders-glsl/lambert_light.vert")

#define FRAG_PATH_LIGHT_ATTENUATED std::string("C:/Users/Monthy/Documents/projects/thesis/implementation_new/nTiled/nTiled/src/pipeline/deferred/shaders-glsl/lambert_light_attenuated.frag")
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
  // load state from scene.json
  // --------------------------
  nTiled::state::State state = nTiled::state::constructStateFromJson(SCENE_PATH);

  // Create window
  // -------------
  glfwInit();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  GLFWwindow* window = glfwCreateWindow(state.view.viewport.x,     // width
                                        state.view.viewport.y,     // height
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

  glViewport(0, 0, state.view.viewport.x, state.view.viewport.y);

  // nTiled components
  // -----------------

  /*
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
  */

  /*
  nTiled::pipeline::clustered::KeyComputeShader* p_shader =
    new nTiled::pipeline::clustered::KeyComputeShader(0,
                                                      state.view,
                                                      glm::uvec2(32, 32));
                                                      */
  nTiled::pipeline::DeferredClusteredShader* p_shader = 
    new nTiled::pipeline::DeferredClusteredShader(
      nTiled::pipeline::DeferredShaderId::DeferredAttenuated,
      VERT_PATH_GEO,
      FRAG_PATH_GEO,
      VERT_PATH_LIGHT,
      FRAG_PATH_LIGHT_ATTENUATED,
      *(state.p_world),
      state.view,
      0,
      glm::uvec2(32, 32));

   /*
  nTiled::pipeline::clustered::TestComputeShader* p_shader =
    new nTiled::pipeline::clustered::TestComputeShader();
    */

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
    //p_pipeline->render();
    p_shader->render();
    gui_manager.render();

    // Display on screen
    glfwSwapBuffers(window);
  }

  // Terminate program
  // -----------------
  // clean up
  /*
  delete p_pipeline;
  if (state.shading.is_debug) {
    delete p_render_pipeline;
  }
  */
  delete p_shader;

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