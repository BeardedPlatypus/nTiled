#pragma once

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "ForwardPipeline.h"
#include "log\LightCalculationsLogger.h"


namespace nTiled {
namespace pipeline {

class ForwardPipelineCounted : public ForwardPipeline {
public:
  ForwardPipelineCounted(state::State& state,
                         logged::LightCalculationsLogger& logger);

protected:
  virtual void constructShaderCatalog() override;

  /*! @brief Reference to THe LightCalculationsLogger of this */
  logged::LightCalculationsLogger& logger;
};

} // pipeline
} // nTiled
