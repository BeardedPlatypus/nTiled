#pragma once

#include <glad\glad.h>
#include "world\World.h"
#include "pipeline\PipelineObject.h"
#include "state\StateView.h"

namespace nTiled {
namespace pipeline {

class Shader {
public:
  Shader(world::World& world,
         state::View& view);
  ~Shader();

  void init();
  void render();

private:
  world::World& world;
  state::View& view;

  PipelineObject* p_obj;

  GLuint shader;
};


}
}
