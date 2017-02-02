#pragma once

#include <glad\glad.h>
#include "pipeline\light-management\clustered\compute-client\ComputeShader.h"

namespace nTiled {
namespace pipeline {
namespace clustered {

class TestComputeShader : public ComputeShader {
public: 
  TestComputeShader();

  virtual void execute() override;

protected:
  void loadComputeShaders();
  void createFBO();

  GLuint compute_shader;
  GLuint p_fbo;
  GLuint p_k_tex;
};

}
}
}
