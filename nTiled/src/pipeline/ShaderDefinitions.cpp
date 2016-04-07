#include "pipeline\ShaderDefinitions.h"

namespace nTiled {
namespace pipeline {

ShaderKey::ShaderKey() {}

ShaderKey::ShaderKey(ForwardShaderId id) :
  type(PipelineType::Forward),
  forward_id(id) {
}

ShaderKey::ShaderKey(DeferredShaderId id) :
  type(PipelineType::Deferred),
  deferred_id(id) {
}

} // pipeline
} // nTiled