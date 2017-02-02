/*! @file PipelineType.h
 *  @brief PipelineType.h contains the definition of all possible pipeline types
 *         specified through the enum class PipelineType
 */
#pragma once

namespace nTiled {
namespace pipeline {

/*! @brief PipelineType class specifies all possible pipelines. */
enum class PipelineType {
  Forward,
  Deferred,
};

} // pipeline
} // nTiled
