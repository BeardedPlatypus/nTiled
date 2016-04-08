#pragma once

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <map>

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "pipeline\Pipeline.h"

// Forward Shaders
#include "pipeline\forward\shaders\ForwardShader.h"


namespace nTiled {
namespace pipeline {

class ForwardPipeline : public Pipeline {
public:
  ForwardPipeline(state::State& state);

  // Render Methods
  void render();
private:
  std::map<
};

} // pipeline
} // nTiled

