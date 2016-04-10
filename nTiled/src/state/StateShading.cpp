#include "state\StateShading.h"

namespace nTiled {
namespace state {

Shading::Shading(std::vector<pipeline::ForwardShaderId> forward_shader_ids,
                 pipeline::PipelineType pipeline_type) : 
    forward_shader_ids(forward_shader_ids), 
    pipeline_type(pipeline_type) { }

}
}