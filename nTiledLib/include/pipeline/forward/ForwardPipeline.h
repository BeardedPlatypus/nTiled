/*! @file ForwardPipeline.h 
 *  @brief ForwardPipeline.h contains the definition of the ForwardPipeline
 *         which handles the rendering with ForwardShaders.
 */
#pragma once

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "pipeline\Pipeline.h"
#include "pipeline\forward\shaders\ForwardShader.h"


namespace nTiled {
namespace pipeline {

/*! @brief ForwardPipeline handles the rendering of all Forward shading
 *         It inherits from Pipeline to offer a consistent interface.
 */
class ForwardPipeline : public Pipeline {
public:
  /*! @brief Construct a new ForwardPipeline with the specified State
   * 
   * @param state Reference to the State this ForwardPipeline observes.
   */
  ForwardPipeline(state::State& state);

  /*! @brief Destruct this ForwardPipeline
   */
  ~ForwardPipeline();

  virtual void initialiseShaders() override;
  // Render Methods
  virtual void render() override;

  virtual void setOutputBuffer(GLint p_output_buffer) override;
protected:
  /*! @brief Construct the Shader catalog of this ForwardPipeline. 
   */
  virtual void constructShaderCatalog(); 

  /*! @brief Vector containing pointers to all ForwardShaders in this 
   *         ForwardPipeline. */
  std::vector<ForwardShader*> shader_catalog;
};

} // pipeline
} // nTiled

