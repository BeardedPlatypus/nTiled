#pragma once

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <glad\glad.h>

namespace nTiled {
namespace pipeline {

class DebugShader {
public:
  DebugShader(GLfloat z_value);

  virtual void render();
protected:
  GLfloat z_value;
};

} // pipeline
} // nTiled