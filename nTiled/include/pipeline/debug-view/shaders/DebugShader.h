/*! @file DebugShader.h
 *  @brief DebugShader.h contains the definition of the DebugShader.
 */
#pragma once

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <glad\glad.h>

namespace nTiled {
namespace pipeline {

/*! @brief DebugShader is responsible for rendering any Display shader.
 */
class DebugShader {
public:
  /*! @brief Construct a new DebugShader at the given z_value. 
   *
   * @param z_value The depth at which this DebugShader is rendered.
   */
  DebugShader(GLfloat z_value);

  /*! @brief Render this DebugShader
   */
  virtual void render();
protected:
  /*! @brief the z value at which this DebugShader is rendered. */
  GLfloat z_value;
};

} // pipeline
} // nTiled