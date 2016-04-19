#pragma once

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "pipeline\Pipeline.h"

namespace nTiled {
namespace pipeline {

class DeferredPipeline : public Pipeline {
public:
  DeferredPipeline(state::State& state);
  ~DeferredPipeline();

  // Render Methods
  virtual void render() override;
};

}
}