#include "pipeline\deferred\shaders\DeferredClusteredShader.h"

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <glm/gtc/type_ptr.hpp>

namespace nTiled {
namespace pipeline {

DeferredClusteredShader::DeferredClusteredShader(
    DeferredShaderId shader_id,
    const std::string& path_geometry_pass_vertex_shader,
    const std::string& path_geometry_pass_fragment_shader,
    const std::string& path_light_pass_vertex_shader,
    const std::string& path_light_pass_fragment_shader,
    const world::World& world,
    const state::View& view,
    GLint p_output_buffer,
    glm::uvec2 tile_size) : 
  DeferredShader(shader_id,
                 path_geometry_pass_vertex_shader,
                 path_geometry_pass_fragment_shader,
                 path_light_pass_vertex_shader,
                 path_light_pass_fragment_shader,
                 world,
                 view,
                 p_output_buffer),
  clustered_light_manager(ClusteredLightManager(view,
                                                world,
                                                tile_size,
                                                this->gBuffer.getPointerDepthTexture())) { 
  glUseProgram(this->light_pass_sp);

  // set uniform variables
  // --------------------------------------------------------------------------
  GLuint n_tiles_x = this->view.viewport.x / tile_size.x;
  if (tile_size.x * n_tiles_x < this->view.viewport.x) {
    n_tiles_x++;
  }

  GLint p_tile_size = glGetUniformLocation(this->light_pass_sp, "tile_size");
  GLint p_n_tiles_x = glGetUniformLocation(this->light_pass_sp, "n_tiles_x");

  glUniform2uiv(p_tile_size, 1, glm::value_ptr(tile_size));
  glUniform1ui(p_n_tiles_x, n_tiles_x);

  // initialise light management 
  // --------------------------------------------------------------------------
  // Construct Shader Storage Buffer Objects
  GLuint ssbo_handles[3];

  glGenBuffers(3, ssbo_handles);
  this->summed_indices_buffer = ssbo_handles[0];
  this->light_cluster_buffer = ssbo_handles[1];
  this->light_index_buffer = ssbo_handles[2];

  // Construct LightGrid buffer
  // --------------------------
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, this->summed_indices_buffer);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, this->light_cluster_buffer);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, this->light_index_buffer);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
  glUseProgram(0);
}

DeferredClusteredShader::~DeferredClusteredShader() {
}

void DeferredClusteredShader::renderGeometryPass() {
  glUseProgram(this->geometry_pass_sp);
  this->renderGeometryPassObjects();
  glUseProgram(0);
}

void DeferredClusteredShader::renderLightPass() {
  // construct light pass information
  // --------------------------------
  this->clustered_light_manager.constructClusteringFrame();

  // Get values from clustered light manager
  // ---------------------------------------
  const std::vector<GLuint>& summed_indices = 
    this->clustered_light_manager.getSummedIndicesData();
  const std::vector<glm::uvec2>& light_clusters = 
    this->clustered_light_manager.getLightClusterData();
  const std::vector<GLuint>& light_indices =
    this->clustered_light_manager.getLightIndexData();

  // Load values into openGL memory
  // ------------------------------
  glUseProgram(this->light_pass_sp);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->summed_indices_buffer);
  glBufferData(GL_SHADER_STORAGE_BUFFER,
               sizeof(GLuint) * summed_indices.size(),
               summed_indices.data(),
               GL_DYNAMIC_DRAW);

  glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->light_cluster_buffer);
  glBufferData(GL_SHADER_STORAGE_BUFFER,
               sizeof(light_clusters[0]) * light_clusters.size(),
               light_clusters.data(),
               GL_DYNAMIC_DRAW);

  glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->light_index_buffer);
  glBufferData(GL_SHADER_STORAGE_BUFFER,
               sizeof(GLuint) * light_indices.size(), 
               light_indices.data(),
               GL_DYNAMIC_DRAW);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

  glActiveTexture(GL_TEXTURE3);
  glBindTexture(GL_TEXTURE_2D, this->k_index_map);
  glActiveTexture(GL_TEXTURE0);

  // Object-Rendering
  // ----------------
  this->renderLightPassObjects();
  glUseProgram(0);
}

}
}
