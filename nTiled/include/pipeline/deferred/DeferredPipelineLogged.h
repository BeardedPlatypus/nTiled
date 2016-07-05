#pragma once

#include "pipeline\deferred\DeferredPipeline.h"
#include "log\Logger.h"


namespace nTiled {
namespace pipeline {

class DeferredPipelineLogged : public DeferredPipeline {
public:
  DeferredPipelineLogged(state::State& state,
                         logged::ExecutionTimeLogger& logger);

protected:
  virtual void constructShader() override;
  logged::ExecutionTimeLogger& logger;
};

}
}