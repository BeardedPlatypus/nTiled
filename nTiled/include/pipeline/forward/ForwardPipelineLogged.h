#pragma once

#include "ForwardPipeline.h"
#include "log\Logger.h"


namespace nTiled {
namespace pipeline {


class ForwardPipelineLogged : public ForwardPipeline {
public :
  ForwardPipelineLogged(state::State& state,
                        logged::ExecutionTimeLogger& logger);

protected:
  virtual void constructShaderCatalog() override;
  logged::ExecutionTimeLogger& logger;
};

}
}
