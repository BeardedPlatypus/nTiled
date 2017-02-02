/*! @file ForwardShaderId.h
 *  @brief ForwardShaderId.h contains the ForwardShaderId enum 
 *         specifying all ForwardShaders available in nTiled.
 */
#pragma once

namespace nTiled {
namespace pipeline {

/*! @brief Enum specifying all the possible forward shaders
*/
enum class ForwardShaderId {
  //  Regular Forward Shaders
  // --------------------------------------------------------------------
  ForwardAttenuated,
  ForwardTexturedAttenuated,

  // Tiled Forward Shaders
  // --------------------------------------------------------------------
  ForwardTiled,
  ForwardTexturedTiled,

  // Clustered Forward Shaders
  // --------------------------------------------------------------------
  ForwardClustered,

  // Hashed Forward Shader
  // --------------------------------------------------------------------
  ForwardHashed,
};

} // pipeline
} // nTiled