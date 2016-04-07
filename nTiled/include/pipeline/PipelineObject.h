#pragma once

#include <glad\glad.h>

namespace nTiled {
namespace pipeline {

// ---------------------------------------------------------------------------
// Pipeline data structs
struct PipelineObject {
public:
  // constructor
  PipelineObject(const GLuint vao,
                 const GLuint element_buffer,
                 const GLuint n_elements);

  // member variables
  const GLuint vao;
  const GLuint element_buffer;
  const GLuint n_elements;
};

} // pipeline
} // nTiled