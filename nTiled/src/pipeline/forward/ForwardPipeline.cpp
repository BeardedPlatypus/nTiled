#include "pipeline\forward\ForwardPipeline.h"
#include "pipeline\forward\shaders\ForwardAttenuatedShader.h"
#include "pipeline\forward\shaders\ForwardTiledShader.h"

#define VERT_PATH std::string("C:/Users/Monthy/Documents/projects/thesis/implementation_new/nTiled/nTiled/src/pipeline/shader-glsl/lambert_basic.vert")
//#define FRAG_PATH std::string("C:/Users/Monthy/Documents/projects/thesis/implementation_new/nTiled/nTiled/src/pipeline/shader-glsl/lambert_basic_attenuated.frag")
#define FRAG_PATH std::string("C:/Users/Monthy/Documents/projects/thesis/implementation_new/nTiled/nTiled/src/pipeline/shader-glsl/lambert_basic_tiled.frag")

namespace nTiled {
namespace pipeline {

ForwardPipeline::ForwardPipeline(state::State& state) : Pipeline(state) { 
  for (ForwardShaderId id : this->state.shading.forward_shader_ids) {
    // construct shader
    ForwardShader* p_shader = new ForwardTiledShader(id,
                                                     VERT_PATH,
                                                     FRAG_PATH,
                                                     *(this->state.p_world),
                                                     this->state.view,
                                                     glm::uvec2(32, 32));
    /*
    ForwardShader* p_shader = new ForwardAttenuatedShader(id,
                                                          VERT_PATH,
                                                          FRAG_PATH,
                                                          *(this->state.p_world),
                                                          this->state.view);
                                                          */
                                                          
    this->shader_catalog.push_back(p_shader);
  }
}

ForwardPipeline::~ForwardPipeline() {
  for (ForwardShader* p_shader : this->shader_catalog) {
    delete p_shader;
  }
}

void ForwardPipeline::render() {
  for (ForwardShader* p_shader : this->shader_catalog) {
    p_shader->render();
  }
}

}
}