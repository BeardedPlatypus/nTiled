#pragma once

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "ForwardShader.h"

namespace nTiled {
namespace pipeline {

class ForwardAttenuatedShader : public ForwardShader {
public:
  ForwardAttenuatedShader(ForwardShaderId shader_id,
                          const std::string& path_vertex_shader,
                          const std::string& path_fragment_shader,
                          const world::World& world,
                          const state::View& view);

  virtual void render() override;
};

} // pipeline
} // nTiled
