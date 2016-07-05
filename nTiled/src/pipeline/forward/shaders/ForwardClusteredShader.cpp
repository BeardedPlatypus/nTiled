#include "pipeline\forward\shaders\ForwardClusteredShader.h"

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <sstream>
#include <iostream>

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "pipeline\shader-util\LoadShaders.h"

// ----------------------------------------------------------------------------
// Defines
// ----------------------------------------------------------------------------
#define VERT_PATH_DEPTH std::string("C:/Users/Monthy/Documents/projects/thesis/implementation_new/nTiled/nTiled/src/pipeline/shader-glsl/lambert_basic.vert")
#define FRAG_PATH_DEPTH std::string("C:/Users/Monthy/Documents/projects/thesis/implementation_new/nTiled/nTiled/src/pipeline/shader-glsl/lambert_basic_attenuated.frag")


namespace nTiled {
namespace pipeline {

ForwardClusteredShader::ForwardClusteredShader(
    ForwardShaderId shader_id,
    const std::string& path_vertex_shader,
    const std::string& path_fragment_shader,
    const world::World& world,
    const state::View& view,
    GLint p_output_buffer,
    glm::uvec2 tile_size,
    const ClusteredLightManagerBuilder& light_manager_builder) :
  ForwardShader(shader_id,
                path_vertex_shader,
                path_fragment_shader,
                world,
                view,
                p_output_buffer),
  p_clustered_light_manager(
    light_manager_builder.constructNewClusteredLightManager(
      view, world, tile_size,
      attachDepthTexture(view.viewport.x, view.viewport.y))) {
  glUseProgram(this->shader);

  // set uniform variables
  // --------------------------------------------------------------------------
  GLuint n_tiles_x = this->view.viewport.x / tile_size.x;
  if (tile_size.x * n_tiles_x < this->view.viewport.x) {
    n_tiles_x++;
  }

  GLint p_tile_size = glGetUniformLocation(this->shader, "tile_size");
  GLint p_n_tiles_x = glGetUniformLocation(this->shader, "n_tiles_x");

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

  // Link usampler k_tex
  // -------------------
  GLint p_k_index_texture = glGetUniformLocation(this->shader,
                                                 "k_index_tex");
  glUniform1i(p_k_index_texture,
              GL_TEXTURE0);
  glUseProgram(0);

  // Construct depth pass shader
  // ---------------------------
  // Vertex Shader
  std::stringstream vert_shader_buffer =     readShader(VERT_PATH_DEPTH);
  GLuint vert_shader = compileShader(GL_VERTEX_SHADER,
                                     vert_shader_buffer.str());

  // Fragment Shader
  std::stringstream frag_shader_buffer = readShader(FRAG_PATH_DEPTH);

  GLuint frag_shader = compileShader(GL_FRAGMENT_SHADER,
                                     frag_shader_buffer.str());
  this->depth_pass_shader = createProgram(vert_shader, frag_shader);


  glm::mat4 perspective_matrix = view.camera.getPerspectiveMatrix();
  GLint p_camera_to_clip = glGetUniformLocation(this->depth_pass_shader,
                                                "camera_to_clip");

  glUseProgram(this->depth_pass_shader);
  glUniformMatrix4fv(p_camera_to_clip,
                     1,
                     GL_FALSE,
                     glm::value_ptr(perspective_matrix));
  glUseProgram(0);


}

void ForwardClusteredShader::render() {
  this->depthPass();
  this->p_clustered_light_manager->constructClusteringFrame();

  glUseProgram(this->shader);
  this->loadLightClustering();
  this->renderObjects();
  glUseProgram(0);
}


void ForwardClusteredShader::depthPass() {
  glUseProgram(this->depth_pass_shader);
  // TODO make sure this can be written as a default, and is restored upon finishing
  glEnable(GL_DEPTH_TEST);  
  glDepthFunc(GL_LESS);    
  glColorMask(0.0, 0.0, 0.0, 0.0); // Disable color, it's useless, we only want depth.
  glDepthMask(GL_TRUE);  
  glm::mat4 lookAt = this->view.camera.getLookAt();

  GLint p_modelToCamera = glGetUniformLocation(this->shader,
                                               "model_to_camera");

  for (PipelineObject* p_obj : this->ps_obj) {
    // Update model to camera
    // ----------------------
    glm::mat4 model_to_camera = lookAt * p_obj->transformation_matrix;

    glUniformMatrix4fv(p_modelToCamera,
                       1,
                       GL_FALSE,
                       glm::value_ptr(model_to_camera));

    // Render object
    // -------------
    glBindVertexArray(p_obj->vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
                 p_obj->element_buffer);
    glDrawElements(GL_TRIANGLES,
                   p_obj->n_elements,
                   GL_UNSIGNED_INT,
                   0);
  }
  glBindVertexArray(0);
  glUseProgram(0);
}


void ForwardClusteredShader::loadLightClustering() {
  const std::vector<GLuint>& summed_indices = 
    this->p_clustered_light_manager->getSummedIndicesData();
  const std::vector<glm::uvec2>& light_clusters = 
    this->p_clustered_light_manager->getLightClusterData();
  const std::vector<GLuint>& light_indices =
    this->p_clustered_light_manager->getLightIndexData();

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

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, this->k_index_map);

  glEnable(GL_DEPTH_TEST);  // We still want depth test
  glDepthFunc(GL_LEQUAL);   // EQUAL should work, too. (Only draw pixels if they are the closest ones)
  glColorMask(1.0, 1.0, 1.0, 1.0);
  glDepthMask(GL_FALSE);
}

// ----------------------------------------------------------------------------
GLuint attachDepthTexture(GLuint width,
                          GLuint height) {
  glBindFramebuffer(GL_DRAW_BUFFER, 0);

  GLuint p_depth_texture;
  glGenTextures(1, &p_depth_texture);
  glBindTexture(GL_TEXTURE_2D, p_depth_texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexImage2D(GL_TEXTURE_2D,
               0,
               GL_DEPTH_COMPONENT32F,
               width, height,
               0,
               GL_DEPTH_COMPONENT,
               GL_FLOAT,
               NULL
               );
  glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER,
                         GL_DEPTH_ATTACHMENT,
                         GL_TEXTURE_2D,
                         p_depth_texture,
                         0
                         );
  glBindTexture(GL_TEXTURE_2D, 0);
  return p_depth_texture;
}

} // pipeline
} // nTiled
