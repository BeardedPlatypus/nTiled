#include "pipeline\forward\ForwardPipeline.h"

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "pipeline\forward\shaders\ForwardAttenuatedShader.h"
#include "pipeline\forward\shaders\ForwardTiledShader.h"
#include "pipeline\forward\shaders\ForwardClusteredShader.h"

// Path defines
#define VERT_PATH_BASIC std::string("C:/Users/Monthy/Documents/projects/thesis/implementation_new/nTiled/nTiled/src/pipeline/shader-glsl/lambert_basic.vert")
#define FRAG_PATH_BASIC_ATTENUATED std::string("C:/Users/Monthy/Documents/projects/thesis/implementation_new/nTiled/nTiled/src/pipeline/shader-glsl/lambert_basic_attenuated.frag")
#define FRAG_PATH_BASIC_TILED std::string("C:/Users/Monthy/Documents/projects/thesis/implementation_new/nTiled/nTiled/src/pipeline/shader-glsl/lambert_basic_tiled.frag")
#define FRAG_PATH_BASIC_CLUSTERED std::string("C:/Users/Monthy/Documents/projects/thesis/implementation_new/nTiled/nTiled/src/pipeline/forward/shaders-glsl/lambert_clustered.frag")

namespace nTiled {
namespace pipeline {

ForwardPipeline::ForwardPipeline(state::State& state) : Pipeline(state) { 
  for (ForwardShaderId id : this->state.shading.forward_shader_ids) {
    ForwardShader* p_shader;

    if (id == ForwardShaderId::ForwardAttenuated) {
      p_shader = new ForwardAttenuatedShader(id,
                                             VERT_PATH_BASIC,
                                             FRAG_PATH_BASIC_ATTENUATED,
                                             *(this->state.p_world),
                                             this->state.view,
                                             this->output_buffer);
    } else if (id == ForwardShaderId::ForwardTiled) {
      p_shader = new ForwardTiledShader(id,
                                        VERT_PATH_BASIC,
                                        FRAG_PATH_BASIC_TILED,
                                        *(this->state.p_world),
                                        this->state.view,
                                        this->output_buffer,
                                        glm::uvec2(32, 32));
    } else if (id == ForwardShaderId::ForwardClustered) {
      p_shader = new ForwardClusteredShader(id,
                                            VERT_PATH_BASIC,
                                            FRAG_PATH_BASIC_CLUSTERED,
                                            *(this->state.p_world),
                                            this->state.view,
                                            this->output_buffer,
                                            glm::uvec2(32, 32));
    } else {
      throw std::runtime_error(std::string("Unsupported shader"));
    }
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

void ForwardPipeline::setOutputBuffer(GLint p_output_buffer) {
  Pipeline::setOutputBuffer(p_output_buffer);

  for (ForwardShader* p_shader : this->shader_catalog) {
    p_shader->setOutputBuffer(this->output_buffer);
  }
}

}
}