#include "pipeline\pipeline-util\ConstructQuad.h"

namespace nTiled {
namespace pipeline {

PipelineObject* constructQuad() {
  // Position data
  GLfloat quad_vertices[] = {
    -1.0f, -1.0f, 0.0f, 1.0f,
    1.0f, -1.0f, 0.0f, 1.0f,
    1.0f,  1.0f, 0.0f, 1.0f,
    -1.0f,  1.0f, 0.0f, 1.0f
  };

  // Element data
  GLushort elements[] = {
    0, 1, 2,
    0, 2, 3
  };

  // setup buffers
  GLuint vbo_handles[2];
  glGenBuffers(2, vbo_handles);
  
  GLuint position_buffer = vbo_handles[0];
  GLuint element_buffer = vbo_handles[1];

  GLuint vao;
  // setup vertex array object
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  // set up position buffer
  glBindBuffer(GL_ARRAY_BUFFER, position_buffer);
  glBufferData(GL_ARRAY_BUFFER,
               16 * sizeof(GLfloat),
               quad_vertices,
               GL_STATIC_DRAW);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);

  // set up element buffer
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               6 * sizeof(GLushort),
               elements,
               GL_STATIC_DRAW);
  glVertexAttribPointer(1, 3, GL_UNSIGNED_SHORT, GL_FALSE, 0, NULL);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  return new PipelineObject(vao,
                            element_buffer,
                            6,
                            glm::mat4(1.0f));
}

}
}