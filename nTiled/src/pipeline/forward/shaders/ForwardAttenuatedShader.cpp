#include "pipeline\forward\shaders\ForwardAttenuatedShader.h"

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <glm/gtc/type_ptr.hpp>

namespace nTiled {
namespace pipeline {

ForwardAttenuatedShader::ForwardAttenuatedShader(
    ForwardShaderId shader_id,
    const std::string& path_vertex_shader,
    const std::string& path_fragment_shader,
    const world::World& world,
    const state::View& view) : ForwardShader(shader_id,
                                             path_vertex_shader,
                                             path_fragment_shader,
                                             world,
                                             view) { }

void ForwardAttenuatedShader::render() {
  glUseProgram(this->shader);
  // render objects
  this->renderObjects();
  glUseProgram(0);
}

} // pipeline
} // nTiled
