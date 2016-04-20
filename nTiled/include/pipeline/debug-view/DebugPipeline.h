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

namespace nTiled {
namespace pipeline {

class DebugPipeline : public Pipeline {
public:
  DebugPipeline(Pipeline* frame_pipeline,
                state::State& state);
  ~DebugPipeline();

  virtual void render() override;

private:
  Pipeline* frame_pipeline;
  std::vector<DebugShader*> debug_shaders;
  PipelineObject* display_quad;

  // framebuffer member
  GLuint p_fbo;
  GLuint p_pass_result;
  GLuint display_pipeline_sp;

};

} // pipeline
} // nTiled