#include "pipeline\light-management\clustered\compute-client\KeySortAndCompactShader.h"

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
//  shader source manipulation
#include <fstream>
#include <sstream>
#include <iostream>

// ----------------------------------------------------------------------------
//  nTiled headers 
// ----------------------------------------------------------------------------
#include "pipeline\shader-util\LoadShaders.h"

// ----------------------------------------------------------------------------
// Define
// ----------------------------------------------------------------------------
#define KEY_SORT_SHADER_PATH std::string("C:/Users/Monthy/Documents/projects/thesis/implementation_new/nTiled/nTiled/src/pipeline/light-management/clustered/compute-glsl/key_sort_and_compact.glsl")

namespace nTiled {
namespace pipeline {
namespace clustered {

// Constructor | Destructor
// ----------------------------------------------------------------------------
KeySortAndCompactShader::KeySortAndCompactShader(GLuint k_tex,
                                                 const state::View& view,
                                                 glm::uvec2 tile_size) :
    k_texture(k_tex),
    tile_size(tile_size), 
    n_tiles(glm::uvec2((unsigned int) std::ceil(view.viewport.x / tile_size.x),
                       (unsigned int) std::ceil(view.viewport.y / tile_size.y))),
    viewport(view.viewport) {
  // Compile compute shader
  // ----------------------
  this->loadComputeShader();

  // Set Textures of this compute shader
  // -----------------------------------
  // clusters index
  GLuint textures[3];

  glGenTextures(3, textures);
  this->cluster_index_texture = textures[0];
  this->unique_clusters_texture = textures[1];
  this->n_clusters_texture = textures[2];
  
  this->specifyTexture(this->cluster_index_texture,
                       GL_R16UI,
                       this->viewport);
  this->specifyTexture(this->unique_clusters_texture,
                       GL_R16UI,
                       this->viewport);
  this->specifyTexture(this->n_clusters_texture,
                       GL_R16UI,
                       this->n_tiles);

  // Linke created textures to program
  GLuint p_k_tex = glGetUniformLocation(this->key_sort_shader,
                                        "k_tex");
  GLuint p_cluster_index_texture = glGetUniformLocation(this->key_sort_shader,
                                                        "cluster_index_tex");
  GLuint p_unique_clusters_tex = glGetUniformLocation(this->key_sort_shader,
                                                      "unique_clusters_tex");
  GLuint p_n_clusters_tex= glGetUniformLocation(this->key_sort_shader,
                                                "n_clusters_tex");

  glUseProgram(this->key_sort_shader);
  glUniform1i(p_k_tex, 0);
  glUniform1i(p_cluster_index_texture, 1);
  glUniform1i(p_unique_clusters_tex, 2);
  glUniform1i(p_n_clusters_tex, 3);
  glUseProgram(0);
}

KeySortAndCompactShader::~KeySortAndCompactShader() {
}

// Execute
// ----------------------------------------------------------------------------
void KeySortAndCompactShader::execute() {
  // Execute compute shader
  // ----------------------
  glUseProgram(this->key_sort_shader);
  // Bind images for use
  glBindImageTexture(0, this->k_texture,
                     0, GL_FALSE, 0,
                     GL_READ_ONLY, GL_R16UI);
  glBindImageTexture(1, this->cluster_index_texture,
                     0, GL_FALSE, 0,
                     GL_WRITE_ONLY, GL_R16UI);
  glBindImageTexture(2, this->unique_clusters_texture, 
                     0, GL_FALSE, 0,
                     GL_WRITE_ONLY, GL_R16UI);
  glBindImageTexture(3, this->n_clusters_texture,
                     0, GL_FALSE, 0,
                     GL_WRITE_ONLY, GL_R16UI);
 
  // Execute compute shader
  glDispatchCompute(this->n_tiles.x, this->n_tiles.y, 1);
  glMemoryBarrier(GL_ALL_BARRIER_BITS);

  // Unbind images after use
  glBindImageTexture(0, 0,
                     0, GL_FALSE, 0,
                     GL_READ_WRITE, GL_R16UI);
  glBindImageTexture(1, 0,
                     0, GL_FALSE, 0,
                     GL_READ_WRITE, GL_R16UI);
  glBindImageTexture(2, 0,
                     0, GL_FALSE, 0,
                     GL_READ_WRITE, GL_R16UI);
  glBindImageTexture(3, 0,
                     0, GL_FALSE, 0,
                     GL_READ_WRITE, GL_R16UI);
  glUseProgram(0);

  // Extract relevant values
  // -----------------------
  // extract
  GLushort* unique_indices = new GLushort[viewport.x * viewport.y];
  glBindTexture(GL_TEXTURE_2D, this->unique_clusters_texture);
  glGetTexImage(GL_TEXTURE_2D,
                0,
                GL_RED_INTEGER,
                GL_UNSIGNED_SHORT,
                unique_indices);

  GLushort* n_indices = new GLushort[n_tiles.x * n_tiles.y];
  glBindTexture(GL_TEXTURE_2D, this->n_clusters_texture);
  glGetTexImage(GL_TEXTURE_2D,
                0,
                GL_RED_INTEGER,
                GL_UNSIGNED_SHORT,
                unique_indices);
  glBindTexture(GL_TEXTURE_2D, 0);
  // format
  this->n_indices_tiles = std::vector<GLushort>();
  this->k_values_tiles = std::vector<GLushort>();

  GLushort n_indices_tile;
  unsigned int offset;
  for (unsigned int i = 0; i < this->n_tiles.x * this->n_tiles.y; i++) {
    n_indices_tile = n_indices[i];
    this->n_indices_tiles.push_back(n_indices_tile);
    offset = i * (this->tile_size.x * this->tile_size.y);
    
    for (unsigned int j = 0; j < n_indices_tile; j++) {
      this->k_values_tiles.push_back(unique_indices[offset + j]);
    }
  }

  delete[] unique_indices;
  delete[] n_indices;
}

// Load shader
// ----------------------------------------------------------------------------
void KeySortAndCompactShader::loadComputeShader() {
  std::stringstream compute_shader_buffer;

  // lines to be updated
  std::string define_tile_size_x = "#define TILE_SIZE_X ";
  std::string str_tile_size_x = std::to_string(tile_size.x);
  bool tile_size_x_updated = false;

  std::string define_tile_size_y = "#define TILE_SIZE_Y ";
  std::string str_tile_size_y = std::to_string(tile_size.y);
  bool tile_size_y_updated = false;

  GLuint total_size = tile_size.x * tile_size.y;
  std::string define_total_size = "#define TOTAL_SIZE ";
  std::string str_total_size = std::to_string(total_size);
  bool total_size_updated = false;

  GLuint n_steps = log(total_size) / log(2);
  std::string define_n_steps = "#define N_STEPS ";
  std::string str_n_steps = std::to_string(n_steps);
  bool n_steps_updated = false;
  
  // construct shader internally
  std::ifstream file_in(KEY_SORT_SHADER_PATH, std::ifstream::in);

  for (std::string line; std::getline(file_in, line);) {
    if (!tile_size_x_updated && 
        line.compare(0, define_tile_size_x.size(), define_tile_size_x) == 0) {
      compute_shader_buffer << define_tile_size_x << str_tile_size_x << std::endl;
      tile_size_x_updated = true;
    } else if (!tile_size_y_updated &&
               line.compare(0, define_tile_size_y.size(), define_tile_size_y) == 0) {
      compute_shader_buffer << define_tile_size_y << str_tile_size_y << std::endl;
      tile_size_y_updated = true;
    } else if (!total_size_updated &&
               line.compare(0, define_total_size.size(), define_total_size) == 0) {
      compute_shader_buffer << define_total_size << str_total_size << std::endl;
      total_size_updated = true;
    } else if (!n_steps_updated && 
               line.compare(0, define_n_steps.size(), define_n_steps) == 0) {
      compute_shader_buffer << define_n_steps << str_total_size << std::endl;
      n_steps_updated = true;
    } else {
      compute_shader_buffer << line << std::endl;
    }
  }

  this->key_sort_shader = createComputeProgram(compute_shader_buffer.str());
}

void KeySortAndCompactShader::specifyTexture(GLuint tex_name,
                                             GLenum format,
                                             glm::uvec2 dimensions) {
  glBindTexture(GL_TEXTURE_2D, tex_name);

  //   Set texture wrapping
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
 
  //   Set one to one mapping
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 1);

  //   Specify texture representation
  glTexStorage2D(GL_TEXTURE_2D,
                 GLint(1),
                 format,
                 dimensions.x, dimensions.y);

  //   Unbind texture
  glBindTexture(GL_TEXTURE_2D, 0);
}

}
}
}