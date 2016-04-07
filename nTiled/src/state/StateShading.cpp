#include "state\StateShading.h"

namespace nTiled {
namespace state {

Shading::Shading(std::map<pipeline::ForwardShaderId,
                          pipeline::ForwardShader*> forward_shader_catalog,
                 std::map<pipeline::DeferredShaderId,
                          pipeline::DeferredShader*> deferred_shader_catalog,
                 pipeline::PipelineType pipeline_type) :
  forward_shader_catalog(forward_shader_catalog),
  deferred_shader_catalog(deferred_shader_catalog),
  pipeline_type(pipeline_type) {
}


Shading::~Shading() {
  // Delete stored forward shaders
  for (const auto& shader_pair : this->forward_shader_catalog) {
    pipeline::ForwardShader* s = shader_pair.second;
    delete s;
  }

  // Delete stored deferred shaders
  for (const auto& shader_pair : this->deferred_shader_catalog) {
    pipeline::DeferredShader* s = shader_pair.second;
    delete s;
  }
}

} // state
} // nTiled