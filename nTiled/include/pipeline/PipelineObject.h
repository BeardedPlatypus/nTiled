#pragma once

#include <glad\glad.h>
#include <glm\glm.hpp>

namespace nTiled {
namespace pipeline {

// ---------------------------------------------------------------------------
// Pipeline data structs
struct PipelineObject {
public:
  // constructor
  PipelineObject(const GLuint vao,
                 const GLuint element_buffer,
                 const GLuint n_elements,
                 glm::mat4 transformation_matrix);

  // member variables
  const GLuint vao;
  const GLuint element_buffer;
  const GLuint n_elements;
  glm::mat4 transformation_matrix;
};

} // pipeline
} // nTiled