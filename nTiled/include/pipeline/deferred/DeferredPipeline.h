#pragma once

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "pipeline\Pipeline.h"
#include "pipeline\deferred\shaders\DeferredShader.h"

namespace nTiled {
namespace pipeline {

class DeferredPipeline : public Pipeline {
public:
  DeferredPipeline(state::State& state);
  ~DeferredPipeline();

  // Render Methods
  virtual void render() override;

private:
  DeferredShader* p_deferred_shader;
};

}
}