/*! @file DeferredShaderId.h
 *  @brief DeferredShaderId.h contains the DeferredShaderId enum
 *         specifying all the DeferredShaders available in nTiled.
 */
#pragma once

namespace nTiled {
namespace pipeline {

/*! @brief Enum specifying all the possible deferred shaders
 */
enum class DeferredShaderId {
  //  Regular Deferred Shaders
  // --------------------------------------------------------------------
  DeferredAttenuated,

  // Tiled Deferred Shaders
  // --------------------------------------------------------------------
  DeferredTiled, 

  // Clustered Deferred Shaders
  // --------------------------------------------------------------------
  DeferredClustered,

  // Hashed Deferred Shaders
  // --------------------------------------------------------------------
  DeferredHashed,
};

} // pipeline
} // nTiled
