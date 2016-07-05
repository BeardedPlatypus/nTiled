#include "pipeline\deferred\DeferredPipeline.h"

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "pipeline\deferred\shaders\DeferredAttenuatedShader.h"
#include "pipeline\deferred\shaders\DeferredTiledShader.h"
#include "pipeline\deferred\shaders\DeferredClusteredShader.h"

// Path defines
#define VERT_PATH_GEO std::string("C:/Users/Monthy/Documents/projects/thesis/implementation_new/nTiled/nTiled/src/pipeline/deferred/shaders-glsl/lambert_gbuffer.vert")
#define FRAG_PATH_GEO std::string("C:/Users/Monthy/Documents/projects/thesis/implementation_new/nTiled/nTiled/src/pipeline/deferred/shaders-glsl/lambert_gbuffer.frag")

#define VERT_PATH_LIGHT std::string("C:/Users/Monthy/Documents/projects/thesis/implementation_new/nTiled/nTiled/src/pipeline/deferred/shaders-glsl/lambert_light.vert")

#define FRAG_PATH_LIGHT_ATTENUATED std::string("C:/Users/Monthy/Documents/projects/thesis/implementation_new/nTiled/nTiled/src/pipeline/deferred/shaders-glsl/lambert_light_attenuated.frag")
#define FRAG_PATH_LIGHT_TILED std::string("C:/Users/Monthy/Documents/projects/thesis/implementation_new/nTiled/nTiled/src/pipeline/deferred/shaders-glsl/lambert_light_tiled.frag")
#define FRAG_PATH_LIGHT_CLUSTERED std::string("C:/Users/Monthy/Documents/projects/thesis/implementation_new/nTiled/nTiled/src/pipeline/deferred/shaders-glsl/lambert_light_clustered.frag")

namespace nTiled {
namespace pipeline {

DeferredPipeline::DeferredPipeline(state::State& state) : Pipeline(state) {
  DeferredShaderId id = this->state.shading.deferred_shader_id;
  if (id == DeferredShaderId::DeferredAttenuated) {
    this->p_deferred_shader = new DeferredAttenuatedShader(
      DeferredShaderId::DeferredAttenuated,
      VERT_PATH_GEO,
      FRAG_PATH_GEO,
      VERT_PATH_LIGHT,
      FRAG_PATH_LIGHT_ATTENUATED,
      *(this->state.p_world),
      this->state.view,
      this->output_buffer);
  } else if (id == DeferredShaderId::DeferredTiled) {
    this->p_deferred_shader = new DeferredTiledShader(
      DeferredShaderId::DeferredTiled,
      VERT_PATH_GEO,
      FRAG_PATH_GEO,
      VERT_PATH_LIGHT,
      FRAG_PATH_LIGHT_TILED,
      *(this->state.p_world),
      this->state.view,
      this->output_buffer,
      glm::uvec2(32, 32),
      TiledLightManagerBuilder());
  } else if (id == DeferredShaderId::DeferredClustered) {
    this->p_deferred_shader = new DeferredClusteredShader(
      DeferredShaderId::DeferredClustered,
      VERT_PATH_GEO,
      FRAG_PATH_GEO,
      VERT_PATH_LIGHT,
      FRAG_PATH_LIGHT_CLUSTERED,
      *(this->state.p_world),
      this->state.view,
      this->output_buffer,
      glm::uvec2(32, 32),
      ClusteredLightManagerBuilder());
  } else {
      throw std::runtime_error(std::string("Unsupported shader"));
    }
}

DeferredPipeline::~DeferredPipeline() {
  delete this->p_deferred_shader;
}

void DeferredPipeline::render() {
  this->p_deferred_shader->render();
}

void DeferredPipeline::setOutputBuffer(GLint p_output_buffer) {
  Pipeline::setOutputBuffer(p_output_buffer);
  this->p_deferred_shader->setOutputBuffer(p_output_buffer);
}

} // pipeline
} // nTiled