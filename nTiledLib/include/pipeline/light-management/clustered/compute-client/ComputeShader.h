/*! @file ComputeShader.h 
 *  @brief ComputeShader.h contains the ComputeShader class which acts
 *         as an interface for all ComputeShaders used in nTiled. 
 */
#pragma once

namespace nTiled {
namespace pipeline {
namespace clustered {

/*! @brief ComputeShader is the base class of all ComputeShaders in nTiled
 *         it provides the execute method with which the ComputeShader can
 *         be executed.
 */
class ComputeShader {
public:
  /*! @brief Execute this ComputeShader. */
  virtual void execute() { }
};

} // clustered
} // pipeline
} // nTiled
