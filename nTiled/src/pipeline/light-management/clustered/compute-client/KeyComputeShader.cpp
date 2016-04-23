#include "pipeline\light-management\clustered\compute-client\KeyComputeShader.h"

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
//  shader source manipulation
#include <fstream>
#include <sstream>
#include <iostream>

//  mathematics
#include <cmath>

// debug
#include <vector>

// ----------------------------------------------------------------------------
//  nTiled headers 
// ----------------------------------------------------------------------------
#include "pipeline\shader-util\LoadShaders.h"
#include "math\util.h"

#include "pipeline\pipeline-util\ConstructQuad.h"

// ----------------------------------------------------------------------------
//  defines
// ----------------------------------------------------------------------------
#define KEY_COMPUTE_SHADER_PATH std::string("C:/Users/Monthy/Documents/projects/thesis/implementation_new/nTiled/nTiled/src/pipeline/light-management/clustered/compute-glsl/key_compute.glsl")

// debug
#define VERT_SHADER_PATH_DEBUG std::string("C:/Users/Monthy/Documents/projects/thesis/implementation_new/nTiled/nTiled/src/pipeline/light-management/clustered/debug-visualise-glsl/key_visualise.vert")
#define FRAG_SHADER_PATH_DEBUG std::string("C:/Users/Monthy/Documents/projects/thesis/implementation_new/nTiled/nTiled/src/pipeline/light-management/clustered/debug-visualise-glsl/key_visualise.frag")


namespace nTiled {
namespace pipeline {
namespace clustered {

KeyComputeShader::KeyComputeShader(GLuint depth_texture,
                                   const state::View& view,
                                   glm::uvec2 tile_size) :
    depth_texture(depth_texture),
    tile_size(tile_size), 
    n_tiles(glm::uvec2((unsigned int) std::ceil(view.viewport.x / tile_size.x),
                       (unsigned int) std::ceil(view.viewport.y / tile_size.y))),
    viewport(view.viewport) {
  //  Compile compute shader
  // --------------------------------------------------------------------------
  std::stringstream compute_shader_buffer;

  // lines to be updated
  std::string define_tile_size_x = "#define TILE_SIZE_X ";
  std::string str_tile_size_x = std::to_string(tile_size.x);
  bool tile_size_x_updated = false;

  std::string define_tile_size_y = "#define TILE_SIZE_Y ";
  std::string str_tile_size_y = std::to_string(tile_size.y);
  bool tile_size_y_updated = false;

  // construct shader internally
  std::ifstream file_in(KEY_COMPUTE_SHADER_PATH, std::ifstream::in);

  for (std::string line; std::getline(file_in, line);) {
    if (!tile_size_x_updated && 
        line.compare(0, define_tile_size_x.size(), define_tile_size_x) == 0) {
      compute_shader_buffer << define_tile_size_x << str_tile_size_x << std::endl;
      tile_size_x_updated = true;
    } else if (!tile_size_y_updated &&
               line.compare(0, define_tile_size_y.size(), define_tile_size_y) == 0) {
      compute_shader_buffer << define_tile_size_y << str_tile_size_y << std::endl;
      tile_size_y_updated = true;
    } else {
      compute_shader_buffer << line << std::endl;
    }
  }

  this->key_compute_shader = createComputeProgram(compute_shader_buffer.str());

  // Generate output texture
  // --------------------------------------------------------------------------
  glGenTextures(1, &(this->k_texture));
  glBindTexture(GL_TEXTURE_2D, this->k_texture);
  glTexImage2D(GL_TEXTURE_2D,                    // target
               0,                                // mipmap level
               GL_R16,                           // internal format
               view.viewport.x, view.viewport.y, // texture dimensions
               0,                                // border
               GL_RED,                           // format
               GL_UNSIGNED_SHORT,                // type
               NULL                              // data
               );
  // set 1 to 1 mapping so no interpolation between pixels occurs
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  // Update uniform variables of Compute shader program
  // --------------------------------------------------------------------------
  GLfloat theta = 0.5 * math::to_radians(view.camera.getFoV());
  GLfloat tile_width_percentage = (float)tile_size.x / (float)view.viewport.x;
  GLfloat k_inv_denominator = 1.0f / (log(1 + (2 * tan(theta) *
                                                   tile_width_percentage)));

  GLfloat near_plane_z = view.camera.getDepthrange().x;

  // memory locations
  // uniform floats
  GLint p_k_inv_denominator = glGetUniformLocation(this->key_compute_shader,
                                                   "k_inv_denominator");
  GLint p_near_plane_z = glGetUniformLocation(this->key_compute_shader,
                                              "near_plane_z");

  GLint p_depth_texture = glGetUniformLocation(this->key_compute_shader,
                                               "depth_tex");
  GLint p_k_tex = glGetUniformLocation(this->key_compute_shader,
                                       "k_tex");

  glUseProgram(this->key_compute_shader);
  // set uniform floats
  glUniform1f(p_k_inv_denominator,
              k_inv_denominator);
  glUniform1f(p_near_plane_z,
              near_plane_z);

  // set textures
  glUniform1i(p_depth_texture,
              GL_TEXTURE0);
  glUniform1i(p_k_tex,
              GL_TEXTURE1);
  glUseProgram(0);

  this->loadDebugVisualiseShaders();
}

KeyComputeShader::~KeyComputeShader() {
  glDeleteTextures(1, &(this->k_texture));

  delete this->display_quad;
}

void KeyComputeShader::execute() {
  glUseProgram(this->key_compute_shader);
  // Bind textures
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, this->depth_texture);

  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, this->k_texture);

  // Execute Compute Shader
  glDispatchCompute(this->n_tiles_x,
                    this->n_tiles_y,
                    0);

  glUseProgram(0);
}

void KeyComputeShader::debugVisualise() {
  // determine max k
  // loop over tiles
  /*
  std::vector<GLushort> k_max_values = std::vector<unsigned short>();

  GLushort k_max_tile;
  GLushort k_current;
  */
  //GLushort k_values[] = new GLushort[viewport.x * viewport.y];
  std::vector<GLushort> k_values = std::vector<GLushort>(viewport.x * viewport.y);
  
  glBindTexture(GL_TEXTURE_2D, this->k_texture);
  glGetTexImage(GL_TEXTURE_2D,
                0,
                GL_RED,
                GL_UNSIGNED_SHORT,
                k_values.data());
  glBindTexture(GL_TEXTURE_2D, 0);

  /*
  for (unsigned int y = 0; y < this->n_tiles.y; y++) {
    for (unsigned int x = 0; x < this->n_tiles_x; x++) {
      k_max_tile = 0;
      for (unsigned short tile_y = 0; tile_y < this->tile_size.y; tile_y++) {
        for (unsigned short tile_x = 0; tile_x < this->tile_size.x; tile_x++) {
        }
      }
      k_max_values.push_back(k_max_tile);
    }
  }
  */

  glUseProgram(this->key_visualise_shader);
  // render k_tex
  glBindVertexArray(this->display_quad->vao);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
               this->display_quad->element_buffer);
  glDrawElements(GL_TRIANGLES,
                 this->display_quad->n_elements,
                 GL_UNSIGNED_SHORT, 0);
  glUseProgram(0);

  //delete[] k_values;
}

void KeyComputeShader::loadDebugVisualiseShaders() {
  // compile shaders
  // Vertex Shader
  std::stringstream vert_buffer = readShader(VERT_SHADER_PATH_DEBUG);
  GLuint vert_shader = compileShader(GL_VERTEX_SHADER,
                                          vert_buffer.str());

  // Fragment Shader
  std::stringstream frag_buffer = readShader(FRAG_SHADER_PATH_DEBUG);
  GLuint frag_shader = compileShader(GL_FRAGMENT_SHADER,
                                            frag_buffer.str());

  this->key_visualise_shader = createProgram(vert_shader, 
                                             frag_shader);
  // load display quad
  this->display_quad = constructQuad();

  // set uniform variables
  glm::vec2 tile_size_f = this->tile_size;
  glm::vec2 colour_step = glm::vec2(1.0 / n_tiles.x, 
                                    1.0 / n_tiles.y);

  GLuint p_tile_size = glGetUniformLocation(this->key_visualise_shader,
                                            "tile_size");
  GLuint p_colour_step = glGetUniformLocation(this->key_visualise_shader,
                                              "colour_step");

  glUseProgram(this->key_visualise_shader);
  glUniform2f(p_tile_size,
              tile_size_f.x,
              tile_size_f.y);
  glUniform2f(p_colour_step,
              colour_step.x,
              colour_step.y);
  glUseProgram(0);
}

}
}
}