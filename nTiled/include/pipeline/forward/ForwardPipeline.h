#pragma once

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "pipeline\Pipeline.h"
#include "pipeline\forward\shaders\ForwardShader.h"


namespace nTiled {
namespace pipeline {

class ForwardPipeline : public Pipeline {
public:
  ForwardPipeline(state::State& state);
  ~ForwardPipeline();

  virtual void initialiseShaders() override;
  // Render Methods
  virtual void render() override;

  virtual void setOutputBuffer(GLint p_output_buffer) override;
protected:
  virtual void constructShaderCatalog(); 

  std::vector<ForwardShader*> shader_catalog;
};

} // pipeline
} // nTiled

