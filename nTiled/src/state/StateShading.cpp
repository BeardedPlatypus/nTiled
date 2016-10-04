#include "state\StateShading.h"

namespace nTiled {
namespace state {

Shading::Shading(std::vector<pipeline::ForwardShaderId> forward_shader_ids,
                 glm::uvec2 tile_size,
                 bool is_debug) : 
    forward_shader_ids(forward_shader_ids), 
    pipeline_type(pipeline::PipelineType::Forward),
    tile_size(tile_size),
    is_debug(is_debug) { }

Shading::Shading(pipeline::DeferredShaderId deferred_shader_id,
                 glm::uvec2 tile_size,
                 bool is_debug) :
    deferred_shader_id(deferred_shader_id),
    pipeline_type(pipeline::PipelineType::Deferred),
    tile_size(tile_size),
    is_debug(is_debug) { }

}
}