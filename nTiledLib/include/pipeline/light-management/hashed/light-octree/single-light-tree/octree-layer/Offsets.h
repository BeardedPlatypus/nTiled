#pragma once

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <glm\glm.hpp>
#include <vector>

namespace nTiled {
namespace pipeline {
namespace hashed {
namespace slt {

struct OffsetEdge {
  OffsetEdge(glm::bvec3 direction,
             glm::bvec3 offset_position,
             glm::bvec2 offset_dimensions);

  glm::bvec3 direction;
  glm::bvec3 offset_position;
  glm::bvec2 offset_dimension;
};


struct OffsetSide {
  OffsetSide(glm::bvec3 direction_i,
             glm::bvec3 direction_j,
             glm::bvec3 offset_position,
             glm::bvec2 offset_i,
             glm::bvec2 offset_j);

  glm::bvec3 direction_i;
  glm::bvec3 direction_j;
  glm::bvec3 offset_position;

  glm::bvec2 offset_i;
  glm::bvec2 offset_j;
};


struct Offsets {
public:
  /*! @brief Construct a new SLTOffsets which either has no offsets or all
  *         offsets
  *
  * @param all_sides_have_offset Whether all sides have an offset or not
  */
  Offsets(bool all_sides_have_offset);

  /*! @brief Construct a new SLTOffsets in which either there is an
  *         offset on the lower or higher bound of an axis.
  *
  * @param x Whether there is an offset left (false) or right (true)
  * @param y Whether there is an offset bottom (false) or up (true)
  * @param z Whether there is an offset close (false) or far (true)
  */
  Offsets(bool x, bool y, bool z);

  std::vector<glm::bvec3> corners;
  std::vector<OffsetEdge> edges;
  std::vector<OffsetSide> sides;

  /*! @brief Offset of the x-axis. */
  glm::bvec2 x;
  /*! @brief Offset of the y-axis */
  glm::bvec2 y;
  /*! @brief Offset of the z-axis */
  glm::bvec2 z;
};

} // slt
} // hierarchical
} // pipeline
} // nTiled
