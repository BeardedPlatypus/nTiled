#include "pipeline\Shader.h"

#include <string>
#include <glm/gtc/type_ptr.hpp>

#include "pipeline\shader-util\LoadShaders.h"

#define VERT_PATH std::string("C:/Users/Monthy/Documents/projects/thesis/implementation_new/nTiled/nTiled/src/pipeline/shader-glsl/solid.vert")
#define FRAG_PATH std::string("C:/Users/Monthy/Documents/projects/thesis/implementation_new/nTiled/nTiled/src/pipeline/shader-glsl/solid.frag")

#define WIDTH 1200
#define HEIGHT 1200

namespace nTiled {
namespace pipeline {

Shader::Shader(world::World& world,
               state::View& view) : world(world),
                                    view(view) {
  /*
  // Position data
  std::vector<glm::vec4> vertices =  {
    glm::vec4(-1.0f, -1.0f, 0.0f, 1.0f),
    glm::vec4(1.0f, -1.0f, 0.0f, 1.0f),
    glm::vec4(1.0f,  1.0f, 0.0f, 1.0f),
    glm::vec4(-1.0f,  1.0f, 0.0f, 1.0f),
  };

  // Element data
  std::vector<glm::tvec3<glm::u32>> elements = {
    glm::tvec3<glm::u32>(0, 1, 2),
    glm::tvec3<glm::u32>(0, 2, 3)
  };

  world::Mesh* p_mesh = world.constructMesh(vertices,
                                            std::vector<glm::vec3>(),
                                            std::vector<glm::vec3>(),
                                            elements);

  world::Object* p_obj = world.constructObject("test",
                                               p_mesh,
                                               glm::mat4(1.0f),
                                               ShaderKey(),
                                               std::map<std::string, std::string>(),
                                               true);
                                               */
  /*
  GLfloat vertices[] =  {
    -1.0f, -1.0f, 0.0f, 1.0f,
    1.0f, -1.0f, 0.0f, 1.0f,
    1.0f,  1.0f, 0.0f, 1.0f,
    -1.0f,  1.0f, 0.0f, 1.0f,
  };

  GLuint elements[] = {
    0, 1, 2,
    0, 2, 3
  };
  */

  // setup buffers
  GLuint vbo_handles[2];
  glGenBuffers(2, vbo_handles);
  GLuint position_buffer = vbo_handles[0];
  GLuint element_buffer = vbo_handles[1];
  GLuint vao;


  // setup vertex array object
  glGenVertexArrays(1, &(vao));
  glBindVertexArray(vao);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  // set up position buffer
  glBindBuffer(GL_ARRAY_BUFFER, position_buffer);
  glBufferData(GL_ARRAY_BUFFER,
               (*(world.p_objects[0])).mesh.vertices.size() * sizeof(glm::vec4), //* 4 * sizeof(GLfloat),
               &((*(world.p_objects[0])).mesh.vertices[0]),
               GL_STATIC_DRAW);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);

  // set up element buffer
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               (*(world.p_objects[0])).mesh.elements.size() * sizeof(glm::tvec3<glm::u32>), // * 3 * sizeof(GLuint),
               &((*(world.p_objects[0])).mesh.elements[0]),
               GL_STATIC_DRAW);
  glVertexAttribPointer(1, 3, GL_UNSIGNED_INT, GL_FALSE, 0, NULL);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  this->p_obj = new PipelineObject(vao, 
                                   element_buffer,
                                   6,
                                   (*(world.p_objects[0])).transformation_matrix);
  // setup shader
  this->shader = createVertexFragmentProgram(VERT_PATH, FRAG_PATH);

  // setup shader
  glm::mat4 perspective_matrix = view.camera.getPerspectiveMatrix();
  GLint p_camera_to_clip = glGetUniformLocation(this->shader,
                                                "camera_to_clip");

  glUseProgram(this->shader);
  glUniformMatrix4fv(p_camera_to_clip,
                     1,
                     GL_FALSE,
                     glm::value_ptr(perspective_matrix));

  glUseProgram(0);

}

Shader::~Shader() {
  delete this->p_obj;
}

void Shader::init() {

}

void Shader::render() {
  glUseProgram(this->shader);    
  glm::mat4 lookAt = this->view.camera.getLookAt();

  GLint p_modelToCamera = glGetUniformLocation(this->shader,
                                               "model_to_camera");

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
  glUseProgram(0);
}

}
}