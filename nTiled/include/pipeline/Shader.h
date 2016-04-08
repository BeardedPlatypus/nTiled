#pragma once

#include <glad\glad.h>
#include "world\World.h"
#include "pipeline\PipelineObject.h"
#include "pipeline\PipelineLight.h"
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

  std::vector<PipelineObject*> ps_obj;
  std::vector<PipelineLight> lights;

  GLuint shader;
  GLuint light_ubo;
};


}
}
