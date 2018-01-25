#pragma once

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <glad\glad.h>
#include <string>
#include <vector>

// ----------------------------------------------------------------------------
//  nTiled Headers
// ----------------------------------------------------------------------------
#include "main/Clock.h"


namespace nTiled {
namespace logged {

class LightCalculationsLogger {
public:
  /*! @brief Construct a new LightCalculationsLogger 
   */
  LightCalculationsLogger(const Clock& clock,
                          std::string output_path,
                          unsigned int width,
                          unsigned int height);

  /*! @brief Destruct this LightCalculationsLogger*/
  ~LightCalculationsLogger();

  // --------------------------------------------------------------------------
  //  Activate | Deactivate
  // --------------------------------------------------------------------------
  void activate();
  void deactivate();

  // --------------------------------------------------------------------------
  //  Get methods
  /*! @brief Get the openGL pointer to the Frame Buffer Object (FBO)
   *
   * @return openGL pointer to the FBO
   */
  GLuint getPointerFBO() { return this->p_fbo; }

  // --------------------------------------------------------------------------
  //  Interaction methods

  /*! @brief Perform pre-rendering operations
   */
  void preRender();

  /*! @brief Perform post-rendering operations
   */
  void postRender();

  /*! @brief Extract the calculated number of calculations per pixel and
   *         write to disk.
   */
  void extractCalculations();

  /*! @brief Export the number of calculations to a json file
   */
  void exportLog();

private:
  bool is_active;

  //  output properties
  // --------------------------------------------------------------------------
  /*! @brief The Clock of this LightCalculationsLogger. */
  const Clock& clock;
  /*! @brief The output path of this LightCalculationsLogger. */
  const std::string output_path;

  std::vector<std::pair<unsigned long, unsigned long>> n_light_calc_data;

  // GBuffer attributes
  // --------------------------------------------------------------------------
  /*! @brief Width of the textures of this LightCalculationsLogger. */
  unsigned int width;
  /*! @brief Height of the textures of this LightCalculationsLogger. */
  unsigned int height;

  //  openGL properties
  // --------------------------------------------------------------------------
  /*! @brief Frame Buffer Object pointer of this LightCalculationsLogger.*/
  GLuint p_fbo;

  /*! @brief Pointer to the calc texture of this LightCalculationsLogger.*/
  GLuint p_calc_texture;
  GLuint p_depth_texture;
};

}
}