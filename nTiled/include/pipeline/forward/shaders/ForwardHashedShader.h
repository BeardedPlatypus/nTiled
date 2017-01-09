#pragma once

#include "ForwardShader.h"
#include "pipeline\light-management\hashed\HashedLightManager.h"

namespace nTiled {
namespace pipeline {

class ForwardHashedShader : public ForwardShader {
public:
  ForwardHashedShader(ForwardShaderId shader_id,
                      const std::string& path_vertex_shader,
                      const std::string& path_fragment_shader,
                      const world::World& world,
                      const state::View& view,
                      GLint p_output_buffer,
                      const hashed::HashedLightManagerBuilder& light_manager_builder,
                      const hashed::HashedConfig& config);

  virtual void render() override;

protected:
  virtual void loadShaders(const std::string& path_vert_shader,
                           const std::string& path_frag_shader);

private:
  hashed::HashedLightManager* p_light_manager;

  // openGL variables
  GLuint light_index_buffer;
  GLuint* hash_map_textures;
  GLuint* leaf_map_textures;
};

}
}
