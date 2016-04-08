#include "pipeline\Shader.h"

#include <string>
#include <glm/gtc/type_ptr.hpp>

#include "pipeline\shader-util\LoadShaders.h"

//#define VERT_PATH std::string("C:/Users/Monthy/Documents/projects/thesis/implementation_new/nTiled/nTiled/src/pipeline/shader-glsl/solid.vert")
//#define FRAG_PATH std::string("C:/Users/Monthy/Documents/projects/thesis/implementation_new/nTiled/nTiled/src/pipeline/shader-glsl/solid.frag")
#define VERT_PATH std::string("C:/Users/Monthy/Documents/projects/thesis/implementation_new/nTiled/nTiled/src/pipeline/shader-glsl/lambert_basic.vert")
#define FRAG_PATH std::string("C:/Users/Monthy/Documents/projects/thesis/implementation_new/nTiled/nTiled/src/pipeline/shader-glsl/lambert_basic_attenuated.frag")

#define WIDTH 1200
#define HEIGHT 1200

namespace nTiled {
namespace pipeline {

Shader::Shader(world::World& world,
               state::View& view) : world(world),
                                    view(view) {
  for (world::Object* p_obj : this->world.p_objects) {
    // setup buffers
    GLuint vbo_handles[4];
    glGenBuffers(4, vbo_handles);
    GLuint position_buffer = vbo_handles[0];
    GLuint normal_buffer = vbo_handles[1];
    GLuint uv_buffer = vbo_handles[2];
    GLuint element_buffer = vbo_handles[3];
    GLuint vao;

    // setup vertex array object
    glGenVertexArrays(1, &(vao));
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    // set up position buffer
    glBindBuffer(GL_ARRAY_BUFFER, position_buffer);
    glBufferData(GL_ARRAY_BUFFER,
                 p_obj->mesh.vertices.size() * sizeof(glm::vec4), //* 4 * sizeof(GLfloat),
                 &(p_obj->mesh.vertices[0]),
                 GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);

    // set up normal buffer
    glBindBuffer(GL_ARRAY_BUFFER, normal_buffer);
    if ((*p_obj).mesh.normals.size() > 0) {
      glBufferData(GL_ARRAY_BUFFER,
                   p_obj->mesh.normals.size() * sizeof(glm::vec3), //* 4 * sizeof(GLfloat),
                   &(p_obj->mesh.normals[0]),
                   GL_STATIC_DRAW);
    }
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    // set up uvw buffer  
    glBindBuffer(GL_ARRAY_BUFFER, uv_buffer);
    if ((*p_obj).mesh.uvs.size() > 0) {
      glBufferData(GL_ARRAY_BUFFER,
                   p_obj->mesh.uvs.size() * sizeof(glm::vec3), //* 4 * sizeof(GLfloat),
                   &(p_obj->mesh.uvs[0]),
                   GL_STATIC_DRAW);
    }
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    // set up element buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 p_obj->mesh.elements.size() * sizeof(glm::tvec3<glm::u32>), // * 3 * sizeof(GLuint),
                 &(p_obj->mesh.elements[0]),
                 GL_STATIC_DRAW);
    glVertexAttribPointer(3, 3, GL_UNSIGNED_INT, GL_FALSE, 0, NULL);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    PipelineObject* p_pipeline_obj =  new PipelineObject(vao,
                                                         element_buffer,
                                                         p_obj->mesh.elements.size() * 3,
                                                         p_obj->transformation_matrix);
    this->ps_obj.push_back(p_pipeline_obj);
  }

  // setup shader
  this->shader = createVertexFragmentProgram(VERT_PATH, FRAG_PATH);

  glm::mat4 perspective_matrix = view.camera.getPerspectiveMatrix();
  GLint p_camera_to_clip = glGetUniformLocation(this->shader,
                                                "camera_to_clip");

  glUseProgram(this->shader);
  glUniformMatrix4fv(p_camera_to_clip,
                     1,
                     GL_FALSE,
                     glm::value_ptr(perspective_matrix));

  // setup light information
  // --------------------------------------------------------------------------
  //  Generate light data
  for (world::PointLight* p_light : this->world.p_lights) {
    PipelineLight data = { p_light->position,
                           p_light->intensity,
                           p_light->radius,
                           p_light->is_emitting };
    this->lights.push_back(data);
  }

  // ------------------------------------------------------------------------
  // generate UBO
  if (this->lights.size() > 0) {
    GLuint num_lights = this->lights.size();

    // Generate ubo buffer
    glGenBuffers(1, &this->light_ubo);
    glBindBuffer(GL_UNIFORM_BUFFER,
                 this->light_ubo);

    glBufferData(GL_UNIFORM_BUFFER,
                 sizeof((this->lights[0])) * num_lights,
                 this->lights.data(),
                 GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    // link UBO with the shader
    GLint p_lightBlock = glGetUniformBlockIndex(this->shader,
                                                "LightBlock");
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, this->light_ubo);
    glUniformBlockBinding(this->shader,
                          p_lightBlock,
                          0);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
  }

  glUseProgram(0);

}

Shader::~Shader() {
  for (PipelineObject* p_obj : this->ps_obj) {
    delete p_obj;
  }
}

void Shader::init() {

}

void Shader::render() {
  glUseProgram(this->shader);    
  glm::mat4 lookAt = this->view.camera.getLookAt();

  GLint p_modelToCamera = glGetUniformLocation(this->shader,
                                               "model_to_camera");

  for (PipelineObject* p_obj : this->ps_obj) {
    glm::mat4 model_to_camera = lookAt * (*p_obj).transformation_matrix;

    glUniformMatrix4fv(p_modelToCamera,
                       1,
                       GL_FALSE,
                       glm::value_ptr(model_to_camera));

    const GLuint vao = (*p_obj).vao;

    glBindVertexArray(vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
                 (*p_obj).element_buffer);
    glDrawElements(GL_TRIANGLES,
                   (*p_obj).n_elements,
                   GL_UNSIGNED_INT,
                   0);
  }

  glUseProgram(0);
}

}
}