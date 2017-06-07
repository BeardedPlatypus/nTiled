#pragma once

#include "pipeline\deferred\DeferredPipeline.h"
#include "log\LightCalculationsLogger.h"

namespace nTiled {
namespace pipeline {

class DeferredPipelineCounted : public DeferredPipeline {
public:
  DeferredPipelineCounted(state::State& state,
                          logged::LightCalculationsLogger & logger);

protected:
  virtual void constructShader() override;
  logged::LightCalculationsLogger& logger;
};

} // pipeline
} // nTiled