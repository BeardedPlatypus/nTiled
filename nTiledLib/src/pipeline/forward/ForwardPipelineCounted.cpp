#include "pipeline\forward\ForwardPipelineCounted.h"


// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "pipeline\forward\shaders\counted\ForwardAttenuatedShaderCounted.h"
#include "pipeline\forward\shaders\counted\ForwardTiledShaderCounted.h"
#include "pipeline\forward\shaders\counted\ForwardClusteredShaderCounted.h"
#include "pipeline\forward\shaders\counted\ForwardHashedShaderCounted.h"


// TODO: move this to an external file
// Path defines
#define VERT_PATH_BASIC std::string("../nTiledLib/src/pipeline/forward/shaders-glsl/lambert_basic.vert")
#define VERT_PATH_HASHED std::string("../nTiledLib/src/pipeline/forward/shaders-glsl/lambert_hashed.vert")

#define FRAG_PATH_BASIC_ATTENUATED std::string("../nTiledLib/src/pipeline/forward/shaders-glsl/counted/lambert_attenuated_counted.frag")
#define FRAG_PATH_BASIC_TILED std::string("../nTiledLib/src/pipeline/forward/shaders-glsl/counted/lambert_tiled_counted.frag")
#define FRAG_PATH_BASIC_CLUSTERED std::string("../nTiledLib/src/pipeline/forward/shaders-glsl/counted/lambert_clustered_counted.frag")
#define FRAG_PATH_BASIC_HASHED std::string("../nTiledLib/src/pipeline/forward/shaders-glsl/counted/lambert_hashed_counted.frag")

namespace nTiled {
namespace pipeline {

ForwardPipelineCounted::ForwardPipelineCounted(state::State& state,
                                               logged::LightCalculationsLogger& logger) :
  ForwardPipeline(state),
  logger(logger) {
}

void ForwardPipelineCounted::constructShaderCatalog() {
  for (ForwardShaderId id : this->state.shading.forward_shader_ids) {
    ForwardShader* p_shader;

    if (id == ForwardShaderId::ForwardAttenuated) {
      p_shader = new ForwardAttenuatedShaderCounted(id,
                                                    VERT_PATH_BASIC,
                                                    FRAG_PATH_BASIC_ATTENUATED,
                                                    *(this->state.p_world),
                                                    this->state.view,
                                                    this->output_buffer,
                                                    this->logger);
    } else if (id == ForwardShaderId::ForwardTiled) {
      p_shader = new ForwardTiledShaderCounted(id,
                                               VERT_PATH_BASIC,
                                               FRAG_PATH_BASIC_TILED,
                                               *(this->state.p_world),
                                               this->state.view,
                                               this->output_buffer,
                                               this->state.shading.tile_size,
                                               TiledLightManagerBuilder(),
                                               this->logger);
    } else if (id == ForwardShaderId::ForwardClustered) {
      p_shader = new ForwardClusteredShaderCounted(id,
                                                   VERT_PATH_BASIC,
                                                   FRAG_PATH_BASIC_CLUSTERED,
                                                   *(this->state.p_world),
                                                   this->state.view,
                                                   this->output_buffer,
                                                   this->state.shading.tile_size,
                                                   ClusteredLightManagerBuilder(),
                                                   this->logger);
    } else if (id == ForwardShaderId::ForwardHashed) {
      p_shader = new ForwardHashedShaderCounted(id, 
                                                VERT_PATH_HASHED,
                                                FRAG_PATH_BASIC_HASHED,
                                                *(this->state.p_world),
                                                this->state.view,
                                                this->output_buffer,
                                                hashed::HashedLightManagerBuilder(),
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
