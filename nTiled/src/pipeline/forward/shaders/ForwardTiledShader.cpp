#include "pipeline\forward\shaders\ForwardTiledShader.h"

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <glm/gtc/type_ptr.hpp>

namespace nTiled {
namespace pipeline {

ForwardTiledShader::ForwardTiledShader(ForwardShaderId shader_id,
                                       const std::string& path_vertex_shader,
                                       const std::string& path_fragment_shader,
                                       const world::World& world,
                                       const state::View& view,
                                       GLint p_output_buffer,
                                       glm::uvec2 tile_size) :
    ForwardShader(shader_id,
                  path_vertex_shader,
                  path_fragment_shader,
                  world,
                  view,
                  p_output_buffer),
    projector(BoxProjector()),
    light_manager(TiledLightManager(world, view,
                                    tile_size.x, tile_size.y,
                                    projector)) {
  glUseProgram(this->shader);

  // set uniform variables
  // --------------------------------------------------------------------------
  GLuint n_tiles_x = this->light_manager.light_grid.n_x;

  GLint p_tile_size = glGetUniformLocation(this->shader, "tile_size");
  GLint p_n_tiles_x = glGetUniformLocation(this->shader, "n_tiles_x");

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
               sizeof(this->light_manager.light_grid.grid[0]) *
               this->light_manager.light_grid.n_tiles,
               this->light_manager.light_grid.grid,
               GL_DYNAMIC_DRAW);

  // Link LightGrid buffer
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, light_grid_buffer);

  // Construct LightIndex buffer
  // ---------------------------
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, light_index_buffer);
  glBufferData(GL_SHADER_STORAGE_BUFFER,
               sizeof(GLuint) * this->light_manager.light_grid.light_index_list.size(),
               this->light_manager.light_grid.light_index_list.data(),
               GL_DYNAMIC_DRAW);

  // Link LightIndex buffer
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, light_index_buffer);

  // remove link
  glBindBuffer(GL_UNIFORM_BUFFER, 0);

  glUseProgram(0);
}


void ForwardTiledShader::render() {
  glUseProgram(this->shader);
  // Light-Management
  // ----------------
  // construct the light grid of tiled shading
  this->light_manager.constructGridFrame();

  // load the tiled datastructures into memory
  // Load LightGridBuffer
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->light_grid_buffer);
  glBufferSubData(GL_SHADER_STORAGE_BUFFER,
                  0,
                  sizeof(this->light_manager.light_grid.grid[0]) *
                  this->light_manager.light_grid.n_tiles,
                  this->light_manager.light_grid.grid);
  // Load LightIndexBuffer
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, light_index_buffer);
  glBufferData(GL_SHADER_STORAGE_BUFFER,
               sizeof(GLuint) * this->light_manager.light_grid.light_index_list.size(),
               this->light_manager.light_grid.light_index_list.data(),
               GL_DYNAMIC_DRAW);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);

  // Object-Rendering
  // ----------------
  this->renderObjects();
  glUseProgram(0);
}
} // pipeline
} // nTiled
