#pragma once

namespace nTiled {
namespace pipeline {

/*!
* Enum specifying all the possible forward shaders
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
};

} // pipeline
} // nTiled