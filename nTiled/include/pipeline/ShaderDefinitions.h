#pragma once

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <string>

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "pipeline\forward\shaders\ForwardShaderId.h"
#include "pipeline\PipelineType.h"

namespace nTiled {
namespace pipeline {

/*!
 * Enum specifying all the possible deferred shaders
 */
enum class DeferredShaderId {
  //  Regular Deferred Shaders
  // --------------------------------------------------------------------
  DeferredBasic,
  DeferredAttenuated,

  // Tiled Forward Shaders
  // --------------------------------------------------------------------
  DeferredTiled
};

// FIXME rewrite this to something more elegant
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