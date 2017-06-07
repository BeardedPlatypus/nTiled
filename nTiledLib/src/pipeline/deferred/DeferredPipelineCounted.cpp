#include "pipeline\deferred\DeferredPipelineCounted.h"


// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "pipeline\deferred\shaders\counted\DeferredAttenuatedShaderCounted.h"
#include "pipeline\deferred\shaders\counted\DeferredTiledShaderCounted.h"
#include "pipeline\deferred\shaders\counted\DeferredClusteredShaderCounted.h"
#include "pipeline\deferred\shaders\counted\DeferredHashedShaderCounted.h"


// Path defines
#define VERT_PATH_GEO std::string("../nTiledLib/src/pipeline/deferred/shaders-glsl/lambert_gbuffer.vert")
#define FRAG_PATH_GEO std::string("../nTiledLib/src/pipeline/deferred/shaders-glsl/lambert_gbuffer.frag")
#define VERT_PATH_LIGHT std::string("../nTiledLib/src/pipeline/deferred/shaders-glsl/lambert_light.vert")

#define FRAG_PATH_LIGHT_ATTENUATED std::string("../nTiledLib/src/pipeline/deferred/shaders-glsl/counted/lambert_light_attenuated_counted.frag")
#define FRAG_PATH_LIGHT_TILED std::string("../nTiledLib/src/pipeline/deferred/shaders-glsl/counted/lambert_light_tiled_counted.frag")
#define FRAG_PATH_LIGHT_CLUSTERED std::string("../nTiledLib/src/pipeline/deferred/shaders-glsl/counted/lambert_light_clustered_counted.frag")
#define FRAG_PATH_LIGHT_HASHED std::string("../nTiledLib/src/pipeline/deferred/shaders-glsl/counted/lambert_light_hashed_counted.frag")

namespace nTiled {
namespace pipeline {

DeferredPipelineCounted::DeferredPipelineCounted(state::State& state,
                                                 logged::LightCalculationsLogger& logger) :
  DeferredPipeline(state),
  logger(logger) {
}

void DeferredPipelineCounted::constructShader() {
  DeferredShaderId id = this->state.shading.deferred_shader_id;
  if (id == DeferredShaderId::DeferredAttenuated) {
    this->p_deferred_shader = new DeferredAttenuatedShaderCounted(
      DeferredShaderId::DeferredAttenuated,
      VERT_PATH_GEO,
      FRAG_PATH_GEO,
      VERT_PATH_LIGHT,
      FRAG_PATH_LIGHT_ATTENUATED,
      *(this->state.p_world),
      this->state.view,
      this->output_buffer,
      this->logger);
  } else if (id == DeferredShaderId::DeferredTiled) {
    this->p_deferred_shader = new DeferredTiledShaderCounted(
      DeferredShaderId::DeferredTiled,
      VERT_PATH_GEO,
      FRAG_PATH_GEO,
      VERT_PATH_LIGHT,
      FRAG_PATH_LIGHT_TILED,
      *(this->state.p_world),
      this->state.view,
      this->output_buffer,
      this->state.shading.tile_size,
      TiledLightManagerBuilder(),
      this->logger);
  } else if (id == DeferredShaderId::DeferredClustered) {
    this->p_deferred_shader = new DeferredClusteredShaderCounted(
      DeferredShaderId::DeferredClustered,
      VERT_PATH_GEO,
      FRAG_PATH_GEO,
      VERT_PATH_LIGHT,
      FRAG_PATH_LIGHT_CLUSTERED,
      *(this->state.p_world),
      this->state.view,
      this->output_buffer,
      this->state.shading.tile_size,
      ClusteredLightManagerBuilder(),
      this->logger);
  } else if (id == DeferredShaderId::DeferredHashed) {
    this->p_deferred_shader = new DeferredHashedShaderCounted(
      DeferredShaderId::DeferredHashed,
      VERT_PATH_GEO,
      FRAG_PATH_GEO,
      VERT_PATH_LIGHT,
      FRAG_PATH_LIGHT_HASHED,
      *(this->state.p_world),
      this->state.view,
      this->output_buffer,
      hashed::HashedLightManagerBuilder(),
      this->state.shading.hashed_config,
      this->logger);
  } else {
    throw std::runtime_error(std::string("Unsupported shader"));
  }
}

}
}
