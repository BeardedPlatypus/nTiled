#pragma once

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <map>

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "pipeline\shaders\forward-shading\ForwardShader.h"
#include "pipeline\shaders\deferred-shading\DeferredShader.h"
#include "pipeline\shaders\ShaderDefinitions.h"

#include "pipeline\PipelineType.h"


namespace nTiled {
namespace state {

/*!
 Shading contains all attributes related to the shading of the State
 */
struct Shading {
  // Constructor
  Shading(std::map<pipeline::ForwardShaderId,
                   pipeline::ForwardShader*> forward_shader_catalog,
          std::map<pipeline::DeferredShaderId,
                   pipeline::DeferredShader*> deferred_shader_catalog,
          pipeline::PipelineType pipeline_type);
  //  Destructor
  ~Shading();

  // Shaders
  std::map<pipeline::ForwardShaderId,
           pipeline::ForwardShader*> forward_shader_catalog;
  std::map<pipeline::DeferredShaderId,
           pipeline::DeferredShader*> deferred_shader_catalog;

  // Pipeline type
  pipeline::PipelineType pipeline_type;
};

} // state
} // nTiled
