/*! @file ForwardPipelineLogged.h 
 *  @brief ForwardPipelineLogged.h contains the 
 *         ForwardPipelineLogged, which is the logged version of the 
 *         ForwardPipeline.
 */
#pragma once

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "ForwardPipeline.h"
#include "log\Logger.h"


namespace nTiled {
namespace pipeline {

/*! @brief ForwardPipelineLogged is the logged version of the ForwardPipeline.
 *         It acts completely similar to ForwardPipeline but will log 
 *         the execution time with a ExecutionTimeLogger.
 */
class ForwardPipelineLogged : public ForwardPipeline {
public :
  /*! @brief Construct a new ForwardPipelineLogged with the specified State
   *         and ExecutionTimeLogger.
   * 
   * @param state Reference to the State this ForwardPipelineLogged will
   *              observe.
   * @param logger Reference to the ExecutionTimeLogger used to track the 
   *               execution time of functions of this ForwardPipelineLogged
   */
  ForwardPipelineLogged(state::State& state,
                        logged::ExecutionTimeLogger& logger);

protected:
  virtual void constructShaderCatalog() override;

  /*! @brief Reference to ExecutionTimeLogger of this ForwardPipelineLogged. */
  logged::ExecutionTimeLogger& logger;
};

} // pipeline
} // nTiled
