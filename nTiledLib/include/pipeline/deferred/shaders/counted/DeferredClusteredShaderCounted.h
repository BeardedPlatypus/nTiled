#pragma once

#include "pipeline\deferred\shaders\DeferredClusteredShader.h"
#include "log\LightCalculationsLogger.h"


namespace nTiled {
namespace pipeline {

class DeferredClusteredShaderCounted : public DeferredClusteredShader {
public:
  DeferredClusteredShaderCounted(
    DeferredShaderId shader_id,
    const std::string& path_geometry_pass_vertex_shader,
    const std::string& path_geometry_pass_fragment_shader,                          
    const std::string& path_light_pass_vertex_shader,
    const std::string& path_light_pass_fragment_shader,
    const world::World& world,
    const state::View& view,
    GLint p_output_buffer,
    glm::uvec2 tile_size,
    const ClusteredLightManagerBuilder& light_manager_builder,
    logged::LightCalculationsLogger& logger);

protected:
  virtual void renderLightPassObjects() override;

  logged::LightCalculationsLogger& logger;
};

}
}
