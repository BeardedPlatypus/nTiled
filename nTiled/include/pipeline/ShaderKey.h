#pragma once

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <string>

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "pipeline\forward\shaders\ForwardShaderId.h"
#include "pipeline\deferred\shaders\DeferredShaderId.h"
#include "pipeline\PipelineType.h"

namespace nTiled {
namespace pipeline {

/*!
 ShaderKey specifies a ShaderId and corresponding PipelineType
 */
struct  ShaderKey {
  // Constructors
  ShaderKey();
  ShaderKey(ForwardShaderId id);
  ShaderKey(DeferredShaderId id);

  // MemberTypes
  PipelineType type;
  ForwardShaderId forward_id;
  DeferredShaderId deferred_id;
};

} // pipeline
} // nTiled