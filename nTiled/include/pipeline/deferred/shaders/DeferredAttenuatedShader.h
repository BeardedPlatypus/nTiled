#pragma once

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "DeferredShader.h"

namespace nTiled {
namespace pipeline {

class DeferredAttenuatedShader : public DeferredShader {
public:
  DeferredAttenuatedShader(DeferredShaderId shader_id,
                           const std::string& path_geometry_pass_vertex_shader,
                           const std::string& path_geometry_pass_fragment_shader,
                           const std::string& path_light_pass_vertex_shader,
                           const std::string& path_light_pass_fragment_shader,
                           const world::World& world,
                           const state::View& view,
                           GLint p_output_buffer);

  virtual void renderGeometryPass() override;
  virtual void renderLightPass() override;
};

}
}
