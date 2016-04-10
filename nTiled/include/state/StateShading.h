#pragma once

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <vector>

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "pipeline\forward\shaders\ForwardShaderId.h"

#include "pipeline\PipelineType.h"


namespace nTiled {
namespace state {

/*!
 Shading contains all attributes related to the shading of the State
 */
struct Shading {
  // Constructor
  Shading(std::vector<pipeline::ForwardShaderId> forward_shader_ids,
          pipeline::PipelineType pipeline_type);

  // Shaders
  std::vector<pipeline::ForwardShaderId> forward_shader_ids;

  // Pipeline type
  pipeline::PipelineType pipeline_type;
};

} // state
} // nTiled
