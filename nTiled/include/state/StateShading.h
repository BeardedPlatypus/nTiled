/*! @file StateShading.h
 *  @brief StateShading.h contains the definition of the Shading struct, 
 *         which holds all information related to the shading of an 
 *         execution of nTiled.
 */
#pragma once

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <vector>
#include <glm\glm.hpp>


// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "pipeline\forward\shaders\ForwardShaderId.h"
#include "pipeline\deferred\shaders\DeferredShaderId.h"
#include "pipeline\debug-view\shaders\DebugShaderId.h"

#include "pipeline\PipelineType.h"


namespace nTiled {
namespace state {

/*! @brief Shading contains all attributes related to the shading of the State
 */
struct Shading {
  // --------------------------------------------------------------------------
  // Constructor
  // --------------------------------------------------------------------------
  /*! @brief Construct a new Shading object with a ForwardShader pipeline
   * 
   * @param forward_shader_ids The set of ForwardShaderId that are active in 
   *                           this run of nTiled.
   * @param tile_size The size of the tiles in pixels in Tiled and Clustered
   *                  shading.
   * @param is_debug Whether this run is in debug mode or not.
   */
  Shading(std::vector<pipeline::ForwardShaderId> forward_shader_ids,
          glm::uvec2 tile_size,
          bool is_debug);

  /*! @brief Construct a new Shading object with a DeferredShader pipeline
   * 
   * @param deferred_shader_id The DeferredShaderId that is active in 
   *                           this run of nTiled.
   * @param tile_size The size of the tiles in pixels in Tiled and Clustered
   *                  shading.
   * @param is_debug Whether this run is in debug mode or not.
   */
  Shading(pipeline::DeferredShaderId deferred_shader_id,
          glm::uvec2 tile_size,
          bool is_debug);

  // Shaders
  /*! @brief The set of ForwardShaderIds that are active in this run of 
   *         nTiled if pipeline_type = Forward
   */
  std::vector<pipeline::ForwardShaderId> forward_shader_ids;

  /*! @brief The DeferredShaderId that is active in this run 
   *         if pipeline_type = Deferred
   */
  pipeline::DeferredShaderId deferred_shader_id;
  std::vector<pipeline::DebugShaderId> debug_shader_ids;

  // Pipeline type
  /*! @brief The PipelineType of this nTiled execution. */
  pipeline::PipelineType pipeline_type;

  /*! @brief Whether this run is executed in DebugMode. */
  bool is_debug;

  /*! @brief The tile_size in pixels used in Tiled and Clustered shading. */
  const glm::uvec2 tile_size;
};

} // state
} // nTiled
