/*! @file DebugPipeline.h
 *  @brief DebugPipeline.h contains the definition of the DebugPipeline.
 */
#pragma once

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <glad\glad.h>

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "pipeline\Pipeline.h"
#include "pipeline\debug-view\shaders\DebugShader.h"
#include "pipeline\PipelineObject.h"

#include "pipeline\debug-view\shaders\DebugTileDisplayShader.h"

namespace nTiled {
namespace pipeline {

/*! DebugPipeline is responsible for rendering any debug information.
 */
class DebugPipeline : public Pipeline {
public:
  /*! @brief Construct a new DebugPipeline with the specified parameters. 
   *
   * @param frame_pipeline The pipeline that is responsible for 
   *                       rendering the regular image. 
   * @param state The state this DebugPipeline observes. 
   */
  DebugPipeline(Pipeline* frame_pipeline,
                state::State& state);

  /*! @brief Destruct this DebugPipeline*/
  ~DebugPipeline();

  virtual void render() override;

private:
  /*! @brief The pipeline that is responsible for rendering the regular image. */
  Pipeline* frame_pipeline;
  /*! @brief A vector containing pointers to all the DebugShaders in this
   *         DebugPipeline.
   */
  std::vector<DebugShader*> debug_shaders;

  /*! @brief Pointer to a PipelineObject quad covering the screen. */
  PipelineObject* display_quad;

  // framebuffer member

  /*! @brief Framebuffer object pointer */
  GLuint p_fbo;

  /*! @brief openGL pointer to the result of the pass */
  GLuint p_pass_result;
  /*! @brief openGL pointer to the display pipeline frame buffer. */
  GLuint display_pipeline_sp;
};

} // pipeline
} // nTiled