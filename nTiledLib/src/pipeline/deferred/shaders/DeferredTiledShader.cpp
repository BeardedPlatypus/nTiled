#include "pipeline\deferred\shaders\DeferredTiledShader.h"

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <glm/gtc/type_ptr.hpp>

namespace nTiled {
namespace pipeline {

DeferredTiledShader::DeferredTiledShader(
    DeferredShaderId shader_id,
    const std::string& path_geometry_pass_vertex_shader,
    const std::string& path_geometry_pass_fragment_shader,
    const std::string& path_light_pass_vertex_shader,
    const std::string& path_light_pass_fragment_shader,
    const world::World& world,
    const state::View& view,
    GLint p_output_buffer,
    glm::uvec2 tile_size, 
    const TiledLightManagerBuilder& light_manager_builder) :
    DeferredShader(shader_id,
                   path_geometry_pass_vertex_shader,
                   path_geometry_pass_fragment_shader,
                   path_light_pass_vertex_shader,
                   path_light_pass_fragment_shader,
                   world,
                   view,
                   p_output_buffer),
    projector(BoxProjector()),
    p_light_manager(light_manager_builder.constructNewTiledLightManager(
      world, view, tile_size.x, tile_size.y, projector)) {
  glUseProgram(this->light_pass_sp);

  // set uniform variables
  // --------------------------------------------------------------------------
  GLuint n_tiles_x = this->p_light_manager->light_grid.n_x;

  GLint p_tile_size = glGetUniformLocation(this->light_pass_sp, "tile_size");
  GLint p_n_tiles_x = glGetUniformLocation(this->light_pass_sp, "n_tiles_x");

  glUniform2uiv(p_tile_size, 1, glm::value_ptr(tile_size));
  glUniform1ui(p_n_tiles_x, n_tiles_x);

  // initialise light management 
  // --------------------------------------------------------------------------
  // Construct Shader Storage Buffer Objects
  GLuint ssbo_handles[2];

  glGenBuffers(2, ssbo_handles);
  this->light_grid_buffer = ssbo_handles[0];
  this->light_index_buffer = ssbo_handles[1];

  // Construct LightGrid buffer
  // --------------------------
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, light_grid_buffer);
  glBufferData(GL_SHADER_STORAGE_BUFFER,
               sizeof(this->p_light_manager->light_grid.grid[0]) *
               this->p_light_manager->light_grid.n_tiles,
               this->p_light_manager->light_grid.grid,
               GL_DYNAMIC_DRAW);

  // Link LightGrid buffer
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, light_grid_buffer);

  // Construct LightIndex buffer
  // ---------------------------
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, light_index_buffer);
  glBufferData(GL_SHADER_STORAGE_BUFFER,
               sizeof(GLuint) * this->p_light_manager->light_grid.light_index_list.size(),
               this->p_light_manager->light_grid.light_index_list.data(),
               GL_DYNAMIC_DRAW);

  // Link LightIndex buffer
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, light_index_buffer);

  // remove link
  glBindBuffer(GL_UNIFORM_BUFFER, 0);

  glUseProgram(0);
}

void DeferredTiledShader::renderGeometryPass() {
  glUseProgram(this->geometry_pass_sp);
  this->renderGeometryPassObjects();
  glUseProgram(0);
}

void DeferredTiledShader::renderLightPass() {
  glUseProgram(this->light_pass_sp);
  this->p_light_manager->constructGridFrame();
  this->loadLightGrid();

  this->renderLightPassObjects();
  glUseProgram(0);
}

void DeferredTiledShader::loadLightGrid() {
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->light_grid_buffer);
  glBufferSubData(GL_SHADER_STORAGE_BUFFER,
                  0,
                  sizeof(this->p_light_manager->light_grid.grid[0]) *
                  this->p_light_manager->light_grid.n_tiles,
                  this->p_light_manager->light_grid.grid);
  // Load LightIndexBuffer
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, light_index_buffer);
  glBufferData(GL_SHADER_STORAGE_BUFFER,
               sizeof(GLuint) * this->p_light_manager->light_grid.light_index_list.size(),
               this->p_light_manager->light_grid.light_index_list.data(),
               GL_DYNAMIC_DRAW);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

}
}
