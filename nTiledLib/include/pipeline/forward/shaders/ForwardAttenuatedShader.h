/*! @file ForwardAttenuatedShader.h 
 *  @brief ForwardAttenuatedShader.h defines the ForwardAttenuatedShader
 *         which implemnets the naive method to render in a Forward Pipeline.
 */
#pragma once

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "ForwardShader.h"

namespace nTiled {
namespace pipeline {

/*! @brief ForwardAttenuatedShader implements the naive algorithm of 
 *         rendering objects within a Forward pipeline.
 *         It inherits from the ForwardShader.
 */
class ForwardAttenuatedShader : public ForwardShader {
public:
  ForwardAttenuatedShader(ForwardShaderId shader_id,
                          const std::string& path_vertex_shader,
                          const std::string& path_fragment_shader,
                          const world::World& world,
                          const state::View& view,
                          GLint p_output_buffer);

  virtual void render() override;
};

} // pipeline
} // nTiled
