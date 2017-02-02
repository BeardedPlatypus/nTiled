/*! @file DeferredPipelineLogged.h 
 *  @brief DeferredPipelineLogged.h contains the 
 *         DeferredPipelineLogged, which is the logged version of the 
 *         DeferredPipeline.
 */
#pragma once

#include "pipeline\deferred\DeferredPipeline.h"
#include "log\Logger.h"


namespace nTiled {
namespace pipeline {

/*! @brief DeferredPipelineLogged is the logged version of the DeferredPipeline.
 *         It acts completely similar to DeferredPipeline but will log 
 *         the execution time with a ExecutionTimeLogger.
 */
class DeferredPipelineLogged : public DeferredPipeline {
public:
  /*! @brief Construct a new DeferredPipelineLogged with the specified State
   *         and ExecutionTimeLogger.
   * 
   * @param state Reference to the State this DeferredPipelineLogged will
   *              observe.
   * @param logger Reference to the ExecutionTimeLogger used to track the 
   *               execution time of functions of this DeferredPipelineLogged
   */
  DeferredPipelineLogged(state::State& state,
                         logged::ExecutionTimeLogger& logger);

protected:
  virtual void constructShader() override;

  /*! @brief Reference to ExecutionTimeLogger of this ForwardPipelineLogged. */
  logged::ExecutionTimeLogger& logger;
};

} // pipeline
} // nTiled