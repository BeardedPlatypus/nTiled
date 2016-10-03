/*! @file ShaderKey.h
 *  @brief ShaderKey.h contains the definition of the ShaderKey struct, 
 *         which in turn is responsible for defining which shader is to be 
 *         used when rendering an object
 */
#pragma once


// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <string>


// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "pipeline\forward\shaders\ForwardShaderId.h"
#include "pipeline\deferred\shaders\DeferredShaderId.h"
#include "pipeline\PipelineType.h"


namespace nTiled {
namespace pipeline {

/*! @brief ShaderKey specifies a ShaderId and corresponding PipelineType
 * 
 * The ShaderKey is responsible for determining which shader is to be used
 * when rendering an object.
 */
struct  ShaderKey {
  // --------------------------------------------------------------------------
  // Constructors
  // --------------------------------------------------------------------------
  /*! @brief Default ShaderKey constructor
   */
  ShaderKey();

  /*! @brief Construct a new ShaderKey with a Forward type and the specified
   *         ForwardShaderId
   *
   * @param id The ForwardShaderId of this new ShaderKey
   */
  ShaderKey(ForwardShaderId id);

  /*! @brief Construct a new ShaderKey with a Deferred type and the specified
   *         DeferredShaderId
   * 
   * @param id The DeferredShaderId of this new ShaderKey
   */
  ShaderKey(DeferredShaderId id);

  // --------------------------------------------------------------------------
  // MemberTypes
  // --------------------------------------------------------------------------
  /*! @brief The PipelineType of this ShaderKey. */
  PipelineType type;
  /*! @brief The ForwardShaderId of this ShaderKey if type==Forward. */
  ForwardShaderId forward_id;
  /*! @brief The DeferredShaderId of this ShaderKey if type==Deferred. */
  DeferredShaderId deferred_id;
};

} // pipeline
} // nTiled