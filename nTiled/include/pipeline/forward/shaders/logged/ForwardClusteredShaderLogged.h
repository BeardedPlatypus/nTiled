#pragma once

#include "pipeline\forward\shaders\ForwardClusteredShader.h"
#include "log\Logger.h"


namespace nTiled {
namespace pipeline {

class ForwardClusteredShaderLogged : public ForwardClusteredShader {
public:
  ForwardClusteredShaderLogged(
    ForwardShaderId shader_id,
    const std::string& path_vertex_shader,
    const std::string& path_fragment_shader,
    const world::World& world,
    const state::View& view,
    GLint p_output_buffer,
    glm::uvec2 tile_size,
    const ClusteredLightManagerBuilder& light_manager_builder,
    logged::ExecutionTimeLogger& logger);

protected:
  virtual void renderObjects() override;
  virtual void depthPass() override;
  virtual void loadLightClustering() override;

  logged::ExecutionTimeLogger& logger;
};

}
}