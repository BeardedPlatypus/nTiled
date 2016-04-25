#pragma once

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "DeferredShader.h"
#include "pipeline\light-management\clustered\compute-client\KeyComputeShader.h"

namespace nTiled {
namespace pipeline {

class DeferredClusteredShader : public DeferredShader {
public:
  DeferredClusteredShader(DeferredShaderId shader_id,
                          const std::string& path_geometry_pass_vertex_shader,
                          const std::string& path_geometry_pass_fragment_shader,
                          const std::string& path_light_pass_vertex_shader,
                          const std::string& path_light_pass_fragment_shader,
                          const world::World& world,
                          const state::View& view,
                          GLint p_output_buffer,
                          glm::uvec2 tile_size);
  ~DeferredClusteredShader();

  virtual void renderGeometryPass() override;
  virtual void renderLightPass() override;

protected:
  // clustered shading elements
  clustered::KeyComputeShader* p_shader;
};

}
}
