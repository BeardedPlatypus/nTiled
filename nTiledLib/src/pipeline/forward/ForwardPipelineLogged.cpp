#include "pipeline\forward\ForwardPipelineLogged.h"


// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "pipeline\forward\shaders\logged\ForwardAttenuatedShaderLogged.h"
#include "pipeline\forward\shaders\logged\ForwardTiledShaderLogged.h"
#include "pipeline\forward\shaders\logged\ForwardClusteredShaderLogged.h"
#include "pipeline\forward\shaders\logged\ForwardHashedShaderLogged.h"

#include "pipeline\light-management\tiled\TiledLightManagerLogged.h"
#include "pipeline\light-management\clustered\ClusteredLightManagerLogged.h"

// TODO: move this to an external file
// Path defines
#define VERT_PATH_BASIC std::string("../nTiledLib/src/pipeline/forward/shaders-glsl/lambert_basic.vert")
#define VERT_PATH_HASHED std::string("../nTiledLib/src/pipeline/forward/shaders-glsl/lambert_hashed.vert")

#define FRAG_PATH_BASIC_ATTENUATED std::string("../nTiledLib/src/pipeline/forward/shaders-glsl/lambert_basic_attenuated.frag")
#define FRAG_PATH_BASIC_TILED std::string("../nTiledLib/src/pipeline/forward/shaders-glsl/lambert_basic_tiled.frag")
#define FRAG_PATH_BASIC_CLUSTERED std::string("../nTiledLib/src/pipeline/forward/shaders-glsl/lambert_clustered.frag")
#define FRAG_PATH_BASIC_HASHED std::string("../nTiledLib/src/pipeline/forward/shaders-glsl/lambert_hashed.frag")

namespace nTiled {
namespace pipeline {

ForwardPipelineLogged::ForwardPipelineLogged(state::State& state,
                                             logged::ExecutionTimeLogger& logger) :
  ForwardPipeline(state),
  logger(logger) {
}

void ForwardPipelineLogged::constructShaderCatalog() {
  for (ForwardShaderId id : this->state.shading.forward_shader_ids) {
    ForwardShader* p_shader;

    if (id == ForwardShaderId::ForwardAttenuated) {
      p_shader = new ForwardAttenuatedShaderLogged(id,
                                                   VERT_PATH_BASIC,
                                                   FRAG_PATH_BASIC_ATTENUATED,
                                                   *(this->state.p_world),
                                                   this->state.view,
                                                   this->output_buffer,
                                                   this->logger);
    } else if (id == ForwardShaderId::ForwardTiled) {
      p_shader = new ForwardTiledShaderLogged(id,
                                              VERT_PATH_BASIC,
                                              FRAG_PATH_BASIC_TILED,
                                              *(this->state.p_world),
                                              this->state.view,
                                              this->output_buffer,
                                              this->state.shading.tile_size,
                                              TiledLightManagerLoggedBuilder(this->logger),
                                              this->logger);
    } else if (id == ForwardShaderId::ForwardClustered) {
      p_shader = new ForwardClusteredShaderLogged(id,
                                                  VERT_PATH_BASIC,
                                                  FRAG_PATH_BASIC_CLUSTERED,
                                                  *(this->state.p_world),
                                                  this->state.view,
                                                  this->output_buffer,
                                                  this->state.shading.tile_size,
                                                  ClusteredLightManagerLoggedBuilder(this->logger),
                                                  this->logger);
    } else if (id == ForwardShaderId::ForwardHashed) {
      p_shader = new ForwardHashedShaderLogged(id, 
                                         VERT_PATH_HASHED,
                                         FRAG_PATH_BASIC_HASHED,
                                         *(this->state.p_world),
                                         this->state.view,
                                         this->output_buffer,
                                         hashed::HashedLightManagerLoggedBuilder(this->logger),
                                         this->state.shading.hashed_config,
                                         this->logger);
    } else {
      throw std::runtime_error(std::string("Unsupported shader"));
    }
    this->shader_catalog.push_back(p_shader);
  }
}

}
}