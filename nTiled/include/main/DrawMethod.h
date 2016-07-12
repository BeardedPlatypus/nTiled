#pragma once

#include "pipeline\Pipeline.h"
#include "gui\GuiManager.h"

#include "Clock.h"

namespace nTiled {

// ----------------------------------------------------------------------------
//  DrawMethods
// ----------------------------------------------------------------------------
class DrawMethod {
public:
  ~DrawMethod() {}
  virtual void draw(GLFWwindow* window,
                    pipeline::Pipeline* p_pipeline,
                    gui::GuiManager* gui_manager,
                    state::View& view,
                    const Clock& clock) const = 0;
};


class DrawToMemory : public DrawMethod {
public:
  DrawToMemory(std::string path,
               state::View& view);
  ~DrawToMemory();

  /*! 
   Draw the current frame on the screen and write the result to 
   memory.

   Param:
     window (GLFWwindow*): pointer to the openGL window
     p_pipeline (Pipeline*): pointer to the pipeline
     gui_manager (GuiManager&): the gui
   */
  virtual void draw(GLFWwindow* window,
                    pipeline::Pipeline* p_pipeline,
                    gui::GuiManager* gui_manager,
                    state::View& view,
                    const Clock& clock) const override;

private:
  const std::string path;

  GLubyte* p_raw_pixel_buffer;
  GLubyte* p_output_buffer;
};


class DrawToView : public DrawMethod {
public:
  /*! 
   Draw the current frame on the screen

   Param:
     window (GLFWwindow*): pointer to the openGL window
     p_pipeline (Pipeline*): pointer to the pipeline
     gui_manager (GuiManager&): the gui
   */
  virtual void draw(GLFWwindow* window,
                    pipeline::Pipeline* p_pipeline,
                    gui::GuiManager* gui_manager,
                    state::View& view,
                    const Clock& clock) const override;
};

} // nTiled
