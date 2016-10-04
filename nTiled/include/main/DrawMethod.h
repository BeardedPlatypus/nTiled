/*! @file DrawMethod.h
 *  @brief DrawMethod.h contains all the different DrawMethods used in 
 *         nTiled.
 */
#pragma once

#include "pipeline\Pipeline.h"
#include "gui\GuiManager.h"

#include "Clock.h"

namespace nTiled {

// ----------------------------------------------------------------------------
//  DrawMethods
// ----------------------------------------------------------------------------
/*! @brief Base DrawMethod, any DrawMethod functionality needs to be derived
 *         from this base class.
 */
class DrawMethod {
public:
  /*! @brief Destruct this DrawMethod. */
  ~DrawMethod() {}
  /*! @brief Draw the current frame with the given parameters.
   */
  virtual void draw(GLFWwindow* window,
                    pipeline::Pipeline* p_pipeline,
                    gui::GuiManager* gui_manager,
                    state::View& view,
                    const Clock& clock) const = 0;
};


/*! @brief Draw to memory DrawMethod. Draws the current frame 
 *         and writes the result to storage.
 */
class DrawToMemory : public DrawMethod {
public:
  /*! @brief Construct a new DrawToMemory with the given parameters.
   * 
   * @param path Path to the directory where each image should be saved.
   * @param view Reference to the View this DrawMethod uses.
   */
  DrawToMemory(std::string path,
               state::View& view);

  ~DrawToMemory();

  /*! @brief Draw the current frame on the screen and write the result to 
   *         memory.
   * 
   * @param window  pointer to the openGL window
   * @param p_pipeline pointer to the pipeline
   * @param gui_manager the gui
   * @param view Reference to the View 
   * @param clock Clock object this DrawMethod observes.
   */
  virtual void draw(GLFWwindow* window,
                    pipeline::Pipeline* p_pipeline,
                    gui::GuiManager* gui_manager,
                    state::View& view,
                    const Clock& clock) const override;

private:
  /*! @brief Path to the directory this DrawMethod saves frames to. */
  const std::string path;

  /*! @brief pointer to the array of raw pixel values. */
  GLubyte* p_raw_pixel_buffer;
  /*! @brief pointer to the array of the output buffer. */
  GLubyte* p_output_buffer;
};


/*! @brief DrawMethod to only draw the frame to the display.
 */
class DrawToView : public DrawMethod {
public:
  /*! @brief Draw the current frame on the screen
   *
   * @param window  pointer to the openGL window
   * @param p_pipeline pointer to the pipeline
   * @param gui_manager the gui
   * @param view Reference to the View 
   * @param clock Clock object this DrawMethod observes.
   */
  virtual void draw(GLFWwindow* window,
                    pipeline::Pipeline* p_pipeline,
                    gui::GuiManager* gui_manager,
                    state::View& view,
                    const Clock& clock) const override;
};

} // nTiled
