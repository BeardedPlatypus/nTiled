#include "pipeline\deferred\DeferredPipelineLogged.h"

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "pipeline\deferred\shaders\logged\DeferredAttenuatedShaderLogged.h"
#include "pipeline\deferred\shaders\logged\DeferredTiledShaderLogged.h"
#include "pipeline\deferred\shaders\logged\DeferredClusteredShaderLogged.h"
#include "pipeline\deferred\shaders\logged\DeferredHashedShaderLogged.h"

#include "pipeline\light-management\tiled\TiledLightManagerLogged.h"
#include "pipeline\light-management\clustered\ClusteredLightManagerLogged.h"
#include "pipeline\light-management\hashed\HashedLightManagerBuilder.h"

// Path defines
#define VERT_PATH_GEO std::string("../nTiledLib/src/pipeline/deferred/shaders-glsl/lambert_gbuffer.vert")
#define FRAG_PATH_GEO std::string("../nTiledLib/src/pipeline/deferred/shaders-glsl/lambert_gbuffer.frag")

#define VERT_PATH_LIGHT std::string("../nTiledLib/src/pipeline/deferred/shaders-glsl/lambert_light.vert")

#define FRAG_PATH_LIGHT_ATTENUATED std::string("../nTiledLib/src/pipeline/deferred/shaders-glsl/lambert_light_attenuated.frag")
#define FRAG_PATH_LIGHT_TILED std::string("../nTiledLib/src/pipeline/deferred/shaders-glsl/lambert_light_tiled.frag")
#define FRAG_PATH_LIGHT_CLUSTERED std::string("../nTiledLib/src/pipeline/deferred/shaders-glsl/lambert_light_clustered.frag")
#define FRAG_PATH_LIGHT_HASHED std::string("../nTiledLib/src/pipeline/deferred/shaders-glsl/lambert_light_hashed.frag")

namespace nTiled {
namespace pipeline {

DeferredPipelineLogged::DeferredPipelineLogged(state::State& state,
                                               logged::ExecutionTimeLogger& logger) :
  DeferredPipeline(state),
  logger(logger) {
}

void DeferredPipelineLogged::constructShader() {
  DeferredShaderId id = this->state.shading.deferred_shader_id;
  if (id == DeferredShaderId::DeferredAttenuated) {
    this->p_deferred_shader = new DeferredAttenuatedShaderLogged(
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
    this->p_deferred_shader = new DeferredTiledShaderLogged(
      DeferredShaderId::DeferredTiled,
      VERT_PATH_GEO,
      FRAG_PATH_GEO,
      VERT_PATH_LIGHT,
      FRAG_PATH_LIGHT_TILED,
      *(this->state.p_world),
      this->state.view,
      this->output_buffer,
      this->state.shading.tile_size,
      TiledLightManagerLoggedBuilder(this->logger),
      this->logger);
  } else if (id == DeferredShaderId::DeferredClustered) {
    this->p_deferred_shader = new DeferredClusteredShaderLogged(
      DeferredShaderId::DeferredClustered,
      VERT_PATH_GEO,
      FRAG_PATH_GEO,
      VERT_PATH_LIGHT,
      FRAG_PATH_LIGHT_CLUSTERED,
      *(this->state.p_world),
      this->state.view,
      this->output_buffer,
      this->state.shading.tile_size,
      ClusteredLightManagerLoggedBuilder(this->logger),
      this->logger);
  } else if (id == DeferredShaderId::DeferredHashed) {
    this->p_deferred_shader = new DeferredHashedShaderLogged(
      DeferredShaderId::DeferredHashed,
      VERT_PATH_GEO,
      FRAG_PATH_GEO,
      VERT_PATH_LIGHT,
      FRAG_PATH_LIGHT_HASHED,
      *(this->state.p_world),
      this->state.view,
      this->output_buffer,
      hashed::HashedLightManagerLoggedBuilder(this->logger),
      this->state.shading.hashed_config,
      this->logger);
  } else {
    throw std::runtime_error(std::string("Unsupported shader"));
  }
}

}
}
