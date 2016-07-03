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

#include "lodepng.h"
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
#define SCENE_PATH std::string("C:/Users/Monthy/Documents/projects/thesis/thesis-data-suite/scenes/indoor-spaceship/scene-def/test_path_export.json")
// ----------------------------------------------------------------------------
//  Function prototypes
// ----------------------------------------------------------------------------
void key_callback(GLFWwindow* window, 
                  int key, 
                  int scancode, 
                  int action, 
                  int mode);


void renderToView(GLFWwindow* window,
                  nTiled::pipeline::Pipeline* p_pipeline,
                  nTiled::gui::GuiManager& gui_manager);

void renderToMemory(GLFWwindow* window,
                    nTiled::pipeline::Pipeline* p_pipeline,
                    nTiled::gui::GuiManager& gui_manager,
                    nTiled::state::View& view);


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
  if (state.view.output->type == nTiled::state::OutputType::Display) {
    renderToView(window,
                 p_pipeline,
                 gui_manager);
  } else if (state.view.output->type == nTiled::state::OutputType::Memory) {
    renderToMemory(window,
                   p_pipeline,
                   gui_manager,
                   state.view);
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


void renderToView(GLFWwindow* window,
                  nTiled::pipeline::Pipeline* p_pipeline,
                  nTiled::gui::GuiManager& gui_manager ) {
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
}


void renderToMemory(GLFWwindow* window,
                    nTiled::pipeline::Pipeline* p_pipeline,
                    nTiled::gui::GuiManager& gui_manager,
                    nTiled::state::View& view) {
  // FIXME: clean up

  // -----------------------------------------------------------------
  // Define Output Buffer
  const size_t n_values = view.viewport.x * view.viewport.y * 4;
  GLubyte* raw_pixel_buffer = new GLubyte[n_values];
  GLubyte* output_buffer = new GLubyte[n_values];

  // -----------------------------------------------------------------
  // Render initial frames
  for (int i = 0; i < view.output->frame_start + 1; i++) {
    gui_manager.update();

    // Clear colour buffer
    glClearColor(0, 0, 0, 1);
    glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // render nTiled components
    p_pipeline->render();

    // Display on screen
    glfwSwapBuffers(window);
  }

  // -----------------------------------------------------------------
  // Render frames to memory
  for (int i = view.output->frame_start + 1; 
       i < view.output->frame_end + 1; 
       i++) {
    // update nTiled components
    gui_manager.update();

    // Clear colour buffer
    glClearColor(0, 0, 0, 1);
    glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // render nTiled components
    p_pipeline->render();

    // Display on screen
    glfwSwapBuffers(window);


    //Read current buffer
    glReadPixels(0, 0,
                 view.viewport.x, view.viewport.y,
                 GL_RGBA, GL_UNSIGNED_BYTE,
                 raw_pixel_buffer);

    // Dirty y-axis invert
    int index_output;
    int index_pixel;
    for (int y = 0; y < view.viewport.y; y++) {
      for (int x = 0; x < view.viewport.x; x++) {
        index_output = (y * view.viewport.x + x) * 4;
        index_pixel = ((view.viewport.y - 1 - y) * view.viewport.x + x) * 4;

        output_buffer[index_output + 0] = raw_pixel_buffer[index_pixel + 0];
        output_buffer[index_output + 1] = raw_pixel_buffer[index_pixel + 1];
        output_buffer[index_output + 2] = raw_pixel_buffer[index_pixel + 2];
        output_buffer[index_output + 3] = raw_pixel_buffer[index_pixel + 3];
      }
    }

    // invert in y-axis

    std::string image_path = 
      view.output->image_base_path + "frame_" + std::to_string(i) + ".png";
    unsigned error = lodepng::encode(image_path,
                                     output_buffer,
                                     view.viewport.x,
                                     view.viewport.y);

    if (error) std::cout << "encoder error " << error << ": " << lodepng_error_text(error) << std::endl;
  }

  // -----------------------------------------------------------------
  // Display application after rendering
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
}