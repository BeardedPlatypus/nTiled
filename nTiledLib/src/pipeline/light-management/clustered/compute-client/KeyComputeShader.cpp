#include "pipeline\light-management\clustered\compute-client\KeyComputeShader.h"

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
//  shader source manipulation
#include <fstream>
#include <sstream>
#include <iostream>

//  shader loading
#include <glm/gtc/type_ptr.hpp>

//  mathematics
#include <cmath>
#include <glm/gtc/matrix_inverse.hpp>

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
//#define KEY_COMPUTE_SHADER_PATH std::string("C:/Users/Monthy/Documents/projects/thesis/implementation_new/nTiled/nTiled/src/pipeline/light-management/clustered/compute-glsl/key_compute.glsl")
#define KEY_COMPUTE_SHADER_PATH std::string("../nTiledLib/src/pipeline/light-management/clustered/compute-glsl/key_compute.glsl")

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
  // Compile compute shader
  // ----------------------
  this->loadComputeShader();

  // Set Textures of this compute shader
  // -----------------------------------
  //   Generate texture
  glGenTextures(1, &(this->k_texture));
  glBindTexture(GL_TEXTURE_2D, this->k_texture);

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
                 GL_R16UI,
                 view.viewport.x, view.viewport.y);

  //   Unbind texture
  glBindTexture(GL_TEXTURE_2D, 0);

  //   Link created output texture with program
  GLuint p_k_tex = glGetUniformLocation(this->key_compute_shader,
                                        "k_tex");
  glUseProgram(this->key_compute_shader);
  glUniform1i(p_k_tex, 0);
  glUseProgram(0);

  // Link uniform values with program
  // --------------------------------
  //   depth values
  GLuint p_depth_tex = glGetUniformLocation(this->key_compute_shader,
                                            "depth_tex");

  glm::mat4 inv_perspective_matrix = glm::inverse(view.camera.getPerspectiveMatrix());
  GLuint p_inv_perspective_matrix = glGetUniformLocation(this->key_compute_shader,
                                                         "inv_perspective_matrix");
  glm::vec4 viewport = glm::vec4(0.0, 0.0, view.viewport);
  GLuint p_viewport = glGetUniformLocation(this->key_compute_shader,
                                           "viewport");

  //   k inv denominator
  GLfloat theta = 0.5 * math::to_radians(view.camera.getFoV());
  GLfloat tile_width_percentage = (float)tile_size.x / (float)view.viewport.x;
  GLfloat k_inv_denominator = 1.0f / (log(1 + (2 * tan(theta) *
                                                   tile_width_percentage)));
  GLint p_k_inv_denominator = glGetUniformLocation(this->key_compute_shader,
                                                   "k_inv_denominator");

  //   z plane
  GLfloat near_plane_z = view.camera.getDepthrange().x;
  GLuint p_near_plane_z = glGetUniformLocation(this->key_compute_shader,
                                               "near_plane_z");

  //   link to shader
  glUseProgram(this->key_compute_shader);
  glUniform1i(p_depth_tex, GL_TEXTURE2);

  glUniformMatrix4fv(p_inv_perspective_matrix,
                     1, GL_FALSE,
                     glm::value_ptr(inv_perspective_matrix));

  glUniform4fv(p_viewport, 1, glm::value_ptr(viewport));

  glUniform1f(p_k_inv_denominator, k_inv_denominator);
  glUniform1f(p_near_plane_z, near_plane_z);
  glUseProgram(0);

  // Load Debug Capabilities
  // -----------------------
  // Todo make this all DEBUG define
  this->loadDebugVisualiseShaders();
}

KeyComputeShader::~KeyComputeShader() {
  glDeleteTextures(1, &(this->k_texture));

  delete this->display_quad;
}

void KeyComputeShader::execute() {
  glUseProgram(this->key_compute_shader);

  // Bind the output texture
  glBindImageTexture(0,                // Binding image unit
                     this->k_texture,  // Texture to be bound
                     0,                // Level
                     GL_FALSE,         // Layered?
                     0,                // specified layer
                     GL_WRITE_ONLY,    // Access method
                     GL_R16UI);        // Format

  // Bind depth texture
  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D, this->depth_texture);

  /*
  GLfloat z_value[1024 * 4];
  glGetTexImage(GL_TEXTURE_2D,
                0,
                GL_DEPTH_COMPONENT,
                GL_FLOAT,
                z_value);
                */

  // Compute results
  glDispatchCompute(this->n_tiles.x, this->n_tiles.y, 1);
  glMemoryBarrier(GL_ALL_BARRIER_BITS);

  // Bind texture
  glBindImageTexture(0,
                     0,
                     0,
                     GL_FALSE,
                     0,
                     GL_READ_WRITE,
                     GL_R16UI);

  // Check if values are correctly written
  /*
  GLushort value[1024];
  glBindTexture(GL_TEXTURE_2D, this->k_texture);
  glGetTexImage(GL_TEXTURE_2D,
                0,
                GL_RED_INTEGER,
                GL_UNSIGNED_SHORT,
                value);
  glBindTexture(GL_TEXTURE_2D, 0);
  */

  glUseProgram(0);
}


void KeyComputeShader::loadComputeShader() {
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
}

void KeyComputeShader::debugVisualise() {
  // compute the highest k
  unsigned int n_pixels = viewport.x * viewport.y;
  GLushort* values = new GLushort[n_pixels];
  glBindTexture(GL_TEXTURE_2D, this->k_texture);
  glGetTexImage(GL_TEXTURE_2D,
                0,
                GL_RED_INTEGER,
                GL_UNSIGNED_SHORT,
                values);

  GLushort k_max = 0;
  for (unsigned int i = 0; i < n_pixels; i++) {
    if (values[i] > k_max) {
      k_max = values[i];
    }
  }

  delete[] values;

  GLfloat k_step = 1.0f / k_max;


  glUseProgram(this->key_visualise_shader);
  // set k step in shader
  GLuint p_k_step = glGetUniformLocation(this->key_visualise_shader,
                                         "k_step");
  glUniform1f(p_k_step, k_step);

  // activate k_texture
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, this->k_texture);

  // Render the view
  glBindVertexArray(this->display_quad->vao);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
               this->display_quad->element_buffer);
  glDrawElements(GL_TRIANGLES,
                 this->display_quad->n_elements,
                 GL_UNSIGNED_SHORT,
                 0);

  glBindTexture(GL_TEXTURE_2D, 0);
  glUseProgram(0);
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
  GLuint p_viewport = glGetUniformLocation(this->key_visualise_shader,
                                           "viewport");
  GLuint p_k_tex = glGetUniformLocation(this->key_visualise_shader,
                                        "k_tex");


  glUseProgram(this->key_visualise_shader);
  glUniform2f(p_tile_size,
              tile_size_f.x,
              tile_size_f.y);
  glUniform2f(p_colour_step,
              colour_step.x,
              colour_step.y);
  glUniform2f(p_viewport,
              this->viewport.x,
              this->viewport.y);

  glUniform1i(p_k_tex,
              GL_TEXTURE0);

  glUseProgram(0);

  this->display_quad = constructQuad();
}

}
}
}