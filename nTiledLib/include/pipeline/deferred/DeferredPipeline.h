/*! @file DeferredPipeline.h 
 *  @brief DeferrefPipeline.h contains the defenition of the DeferredPipeline
 *         which handles the rendering with DeferredShaders.
 */
#pragma once

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "pipeline\Pipeline.h"
#include "pipeline\deferred\shaders\DeferredShader.h"

namespace nTiled {
namespace pipeline {

/*! @brief DeferredPipeline handles the rendering of all Deferred shading
 *         It inherits from Pipeline to offer a consistent interface.
 */
class DeferredPipeline : public Pipeline {
public:
  /*! @brief Construct a new DeferredPipeline with the specified state
   *
   * @param state Refrence to the State this DeferredPipeline observes.
   */
  DeferredPipeline(state::State& state);

  /*! @brief Destruct this DeferredPipeline */
  ~DeferredPipeline();

  virtual void initialiseShaders() override;
  // Render Methods
  virtual void render() override;

  virtual void setOutputBuffer(GLint p_output_buffer) override;
protected:
  /*! @brief Construct the DeferredShader of this DeferredPipeline. */
  virtual void constructShader(); 

  /*! @brief Pointer to the DeferredShader of this DeferredPipeline. */
  DeferredShader* p_deferred_shader;
};

}
}