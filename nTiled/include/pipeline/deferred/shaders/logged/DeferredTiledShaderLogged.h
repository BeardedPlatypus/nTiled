#pragma once

#include "pipeline\deferred\shaders\DeferredTiledShader.h"
#include "log\Logger.h"


namespace nTiled {
namespace pipeline {

class DeferredTiledShaderLogged : public DeferredTiledShader {
public:
  DeferredTiledShaderLogged(
    DeferredShaderId shader_id,
    const std::string& path_geometry_pass_vertex_shader,
    const std::string& path_geometry_pass_fragment_shader,
    const std::string& path_light_pass_vertex_shader,
    const std::string& path_light_pass_fragment_shader,
    const world::World& world,
    const state::View& view,
    GLint p_output_buffer,
    glm::uvec2 tile_size,
    const TiledLightManagerBuilder& light_manager_builder,
    logged::ExecutionTimeLogger& logger);

protected:
  virtual void renderGeometryPassObjects() override;
  virtual void renderLightPassObjects() override;
  virtual void loadLightGrid() override;

  logged::ExecutionTimeLogger& logger;
};

}
}
