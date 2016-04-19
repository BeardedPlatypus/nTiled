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

#include "pipeline\deferred\shaders\DeferredTiledShader.h"


// ----------------------------------------------------------------------------
//  Defines
// ----------------------------------------------------------------------------
#define WIDTH 1200
#define HEIGHT 1200

#define SCENE_PATH std::string("C:/Users/Monthy/Documents/projects/thesis/scenes/scene-definitions/test_2/scene.json")

//#define VERT_PATH std::string("C:/Users/Monthy/Documents/projects/thesis/implementation_new/nTiled/nTiled/src/pipeline/shader-glsl/lambert_basic.vert")
//#define FRAG_PATH std::string("C:/Users/Monthy/Documents/projects/thesis/implementation_new/nTiled/nTiled/src/pipeline/shader-glsl/lambert_basic_attenuated.frag")
#define VERT_PATH_GEO std::string("C:/Users/Monthy/Documents/projects/thesis/implementation_new/nTiled/nTiled/src/pipeline/deferred/shaders-glsl/lambert_gbuffer.vert")
#define FRAG_PATH_GEO std::string("C:/Users/Monthy/Documents/projects/thesis/implementation_new/nTiled/nTiled/src/pipeline/deferred/shaders-glsl/lambert_gbuffer.frag")
#define VERT_PATH_LIGHT std::string("C:/Users/Monthy/Documents/projects/thesis/implementation_new/nTiled/nTiled/src/pipeline/deferred/shaders-glsl/lambert_light.vert")
#define FRAG_PATH_LIGHT std::string("C:/Users/Monthy/Documents/projects/thesis/implementation_new/nTiled/nTiled/src/pipeline/deferred/shaders-glsl/lambert_light_tiled.frag")
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

  //nTiled::pipeline::Pipeline* pipeline = new nTiled::pipeline::ForwardPipeline(state);
  nTiled::pipeline::DeferredTiledShader* shader = new nTiled::pipeline::DeferredTiledShader(
    nTiled::pipeline::DeferredShaderId::DeferredAttenuated,
    VERT_PATH_GEO,
    FRAG_PATH_GEO,
    VERT_PATH_LIGHT,
    FRAG_PATH_LIGHT,
    *(state.p_world),
    state.view,
    glm::uvec2(32, 32));

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
    shader->render();
    //pipeline->render();
    gui_manager.render();

    // Display on screen
    glfwSwapBuffers(window);
  }

  // Terminate program
  // -----------------
  delete shader;

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