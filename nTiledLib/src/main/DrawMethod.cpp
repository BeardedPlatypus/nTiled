#include "main\DrawMethod.h"

#include <GLFW\glfw3.h>
#include <iostream>
#include "lodepng.h"

namespace nTiled {

// ----------------------------------------------------------------------------
//  DrawToMemory
// ----------------------------------------------------------------------------
DrawToMemory::DrawToMemory(std::string path,
                           state::View& view) : 
    path(path),
    p_raw_pixel_buffer(new GLubyte[view.viewport.x * view.viewport.y * 4]),
    p_output_buffer(new GLubyte[view.viewport.x * view.viewport.y * 4]) {
}

DrawToMemory::~DrawToMemory() {
  delete this->p_output_buffer;
  delete this->p_raw_pixel_buffer;
}

void DrawToMemory::draw(GLFWwindow* window,
                        pipeline::Pipeline* p_pipeline,
                        gui::GuiManager* gui_manager,
                        state::View& view,
                        const Clock& clock) const {
  // update nTiled components
  gui_manager->update();

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
               this->p_raw_pixel_buffer);

  // Dirty y-axis invert
  int index_output;
  int index_pixel;
  for (int y = 0; y < view.viewport.y; y++) {
    for (int x = 0; x < view.viewport.x; x++) {
      index_output = (y * view.viewport.x + x) * 4;
      index_pixel = ((view.viewport.y - 1 - y) * view.viewport.x + x) * 4;

      this->p_output_buffer[index_output + 0] = this->p_raw_pixel_buffer[index_pixel + 0];
      this->p_output_buffer[index_output + 1] = this->p_raw_pixel_buffer[index_pixel + 1];
      this->p_output_buffer[index_output + 2] = this->p_raw_pixel_buffer[index_pixel + 2];
      this->p_output_buffer[index_output + 3] = this->p_raw_pixel_buffer[index_pixel + 3];
    }
  }

  // invert in y-axis

  std::string image_path =
    view.output->image_base_path + "frame_" + std::to_string(clock.getCurrentFrame()) + ".png";
  unsigned error = lodepng::encode(image_path,
                                   p_output_buffer,
                                   view.viewport.x,
                                   view.viewport.y);

  if (error) std::cout << "encoder error " << error << ": " << lodepng_error_text(error) << std::endl;
}


// ----------------------------------------------------------------------------
//  DrawToView
// ----------------------------------------------------------------------------
void DrawToView::draw(GLFWwindow* window,
                        pipeline::Pipeline* p_pipeline,
                        gui::GuiManager* gui_manager,
                        state::View& view,
                        const Clock& clock) const {
    // update nTiled components
    gui_manager->update();

    // Clear colour buffer
    glClearColor(0, 0, 0, 1);
    glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // render nTiled components
    p_pipeline->render();
    gui_manager->render();

    // Display on screen
    glfwSwapBuffers(window);
}

} // nTiled