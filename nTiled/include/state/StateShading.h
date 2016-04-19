#pragma once

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <vector>

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "pipeline\forward\shaders\ForwardShaderId.h"
#include "pipeline\deferred\shaders\DeferredShaderId.h"

#include "pipeline\PipelineType.h"


namespace nTiled {
namespace state {

/*!
 Shading contains all attributes related to the shading of the State
 */
struct Shading {
  // Constructor
  Shading(std::vector<pipeline::ForwardShaderId> forward_shader_ids,
          bool is_debug);
  Shading(pipeline::DeferredShaderId deferred_shader_id,
          bool is_debug);

  // Shaders
  std::vector<pipeline::ForwardShaderId> forward_shader_ids;
  pipeline::DeferredShaderId deferred_shader_id;

  // Pipeline type
  pipeline::PipelineType pipeline_type;

  bool is_debug;
};

} // state
} // nTiled
