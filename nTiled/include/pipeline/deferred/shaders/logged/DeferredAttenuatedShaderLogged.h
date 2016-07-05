#pragma once

#include "pipeline\deferred\shaders\DeferredAttenuatedShader.h"
#include "log\Logger.h"


namespace nTiled {
namespace pipeline {

class DeferredAttenuatedShaderLogged : public DeferredAttenuatedShader {
public:
  DeferredAttenuatedShaderLogged(
    DeferredShaderId shader_id,
    const std::string& path_geometry_pass_vertex_shader,
    const std::string& path_geometry_pass_fragment_shader,
    const std::string& path_light_pass_vertex_shader,
    const std::string& path_light_pass_fragment_shader,
    const world::World& world,
    const state::View& view,
    GLint p_output_buffer,
    logged::ExecutionTimeLogger& logger);

protected:
  virtual void renderGeometryPassObjects() override;
  virtual void renderLightPassObjects() override;

  logged::ExecutionTimeLogger& logger;
};


}
}
