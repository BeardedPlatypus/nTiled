#pragma once

#include "pipeline\forward\shaders\ForwardAttenuatedShader.h"
#include "log\Logger.h"


namespace nTiled {
namespace pipeline {

class ForwardAttenuatedShaderLogged : public ForwardAttenuatedShader {
public:
  ForwardAttenuatedShaderLogged(
    ForwardShaderId shader_id,
    const std::string& path_vertex_shader,
    const std::string& path_fragment_shader,
    const world::World& world,
    const state::View& view,
    GLint p_output_buffer,
    logged::ExecutionTimeLogger& logger);

protected:
  virtual void renderObjects() override;

  logged::ExecutionTimeLogger& logger;
};

} // pipeline
} // nTiled
