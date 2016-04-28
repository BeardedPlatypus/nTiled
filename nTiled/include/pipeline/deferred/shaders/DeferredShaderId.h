#pragma once

namespace nTiled {
namespace pipeline {

/*!
 * Enum specifying all the possible deferred shaders
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
  DeferredClustered
};

} // pipeline
} // nTiled
