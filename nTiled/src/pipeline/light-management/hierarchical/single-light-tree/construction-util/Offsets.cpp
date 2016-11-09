#include "pipeline\light-management\hierarchical\single-light-tree\construction-util\Offsets.h"

namespace nTiled {
namespace pipeline {
namespace hierarchical {
namespace slt {


// ----------------------------------------------------------------------------
OffsetEdge::OffsetEdge(glm::bvec3 direction,
                       glm::bvec3 offset_position,
                       glm::bvec2 offset_dimension) : 
  direction(direction), 
  offset_position(offset_position),
  offset_dimension(offset_dimension) { }

// ----------------------------------------------------------------------------
OffsetSide::OffsetSide(glm::bvec3 direction_i,
                       glm::bvec3 direction_j,
                       glm::bvec3 offset_position,
                       glm::bvec2 offset_i,
                       glm::bvec2 offset_j) :
  direction_i(direction_i), 
  direction_j(direction_j), 
  offset_position(offset_position),
  offset_i(offset_i), 
  offset_j(offset_j) { }

// ----------------------------------------------------------------------------
Offsets::Offsets(bool all_sides_have_offset) :
  x(glm::bvec2(all_sides_have_offset, all_sides_have_offset)),
  y(glm::bvec2(all_sides_have_offset, all_sides_have_offset)),
  z(glm::bvec2(all_sides_have_offset, all_sides_have_offset)),
  // 8 corners
  corners(std::vector<glm::bvec3>()),
  edges(std::vector<OffsetEdge>()),
  sides(std::vector<OffsetSide>()) {
  if (all_sides_have_offset) {
    // Corners
    corners.push_back(glm::uvec3(0, 0, 0));
    corners.push_back(glm::uvec3(1, 0, 0));
    corners.push_back(glm::uvec3(0, 1, 0));
    corners.push_back(glm::uvec3(1, 1, 0));
    corners.push_back(glm::uvec3(0, 0, 1));
    corners.push_back(glm::uvec3(1, 0, 1)); 
    corners.push_back(glm::uvec3(0, 1, 1));
    corners.push_back(glm::uvec3(1, 1, 1));

    // Edges
    // x direction
    edges.push_back(OffsetEdge(glm::bvec3(true,  false, false),
                               glm::bvec3(false, false, false),
                               glm::bvec2(true, true)));
    edges.push_back(OffsetEdge(glm::bvec3(true,  false, false),
                               glm::bvec3(false, true,  false),
                               glm::bvec2(true, true)));
    edges.push_back(OffsetEdge(glm::bvec3(true,  false, false),
                               glm::bvec3(false, false, true),
                               glm::bvec2(true, true)));
    edges.push_back(OffsetEdge(glm::bvec3(true,  false, false),
                               glm::bvec3(false, true,  true),
                               glm::bvec2(true, true)));

    // y direction
    edges.push_back(OffsetEdge(glm::bvec3(false, true, false),
                               glm::bvec3(false, false, false),
                               glm::bvec2(true, true)));
    edges.push_back(OffsetEdge(glm::bvec3(false, true, false),
                               glm::bvec3(true, false, false),
                               glm::bvec2(true, true)));
    edges.push_back(OffsetEdge(glm::bvec3(false, true, false),
                               glm::bvec3(false, false, true),
                               glm::bvec2(true, true)));
    edges.push_back(OffsetEdge(glm::bvec3(false, true, false),
                               glm::bvec3(true, false, true),
                               glm::bvec2(true, true)));
    // z direction
    edges.push_back(OffsetEdge(glm::bvec3(false, false, true),
                               glm::bvec3(false, false, false),
                               glm::bvec2(true, true)));
    edges.push_back(OffsetEdge(glm::bvec3(false, false, true),
                               glm::bvec3(true, false, false),
                               glm::bvec2(true, true)));
    edges.push_back(OffsetEdge(glm::bvec3(false, false, true),
                               glm::bvec3(false, true, false),
                               glm::bvec2(true, true)));
    edges.push_back(OffsetEdge(glm::bvec3(false, false, true),
                               glm::bvec3(true, true, false),
                               glm::bvec2(true, true)));

    /*
    // Edges
    // x direction
    edges.push_back(OffsetEdge(glm::bvec3(1, 0, 0),
                               glm::bvec3(0, 0, 0),
                               glm::bvec2(1, 1)));
    edges.push_back(OffsetEdge(glm::bvec3(1, 0, 0),
                               glm::bvec3(0, 1, 0),
                               glm::bvec2(1, 1)));
    edges.push_back(OffsetEdge(glm::bvec3(1, 0, 0),
                               glm::bvec3(0, 0, 1),
                               glm::bvec2(1, 1)));
    edges.push_back(OffsetEdge(glm::bvec3(1, 0, 0),
                               glm::bvec3(0, 1, 1),
                               glm::bvec2(1, 1)));
    // y direction
    edges.push_back(OffsetEdge(glm::bvec3(0, 1, 0),
                               glm::bvec3(0, 0, 0),
                               glm::bvec2(1, 1)));
    edges.push_back(OffsetEdge(glm::bvec3(0, 1, 0),
                               glm::bvec3(1, 0, 0),
                               glm::bvec2(1, 1)));
    edges.push_back(OffsetEdge(glm::bvec3(0, 1, 0),
                               glm::bvec3(0, 0, 1),
                               glm::bvec2(1, 1)));
    edges.push_back(OffsetEdge(glm::bvec3(0, 1, 0),
                               glm::bvec3(1, 0, 1),
                               glm::bvec2(1, 1)));
    // z direction
    edges.push_back(OffsetEdge(glm::bvec3(0, 0, 1),
                               glm::bvec3(0, 0, 0),
                               glm::bvec2(1, 1)));
    edges.push_back(OffsetEdge(glm::bvec3(0, 0, 1),
                               glm::bvec3(1, 0, 0),
                               glm::bvec2(1, 1)));
    edges.push_back(OffsetEdge(glm::bvec3(0, 0, 1),
                               glm::bvec3(0, 1, 0),
                               glm::bvec2(1, 1)));
    edges.push_back(OffsetEdge(glm::bvec3(0, 0, 1),
                               glm::bvec3(1, 1, 0),
                               glm::bvec2(1, 1)));
    */

    // sides
    // z
    sides.push_back(OffsetSide(glm::bvec3(true,  false, false),
                               glm::bvec3(false, true, false),
                               glm::bvec3(false, false, false),
                               glm::bvec2(true, true),
                               glm::bvec2(true, true)));
    sides.push_back(OffsetSide(glm::bvec3(true,  false, false),
                               glm::bvec3(false, true, false),
                               glm::bvec3(false, false, true),
                               glm::bvec2(true, true),
                               glm::bvec2(true, true)));
    // y
    sides.push_back(OffsetSide(glm::bvec3(true,  false, false),
                               glm::bvec3(false, false, true),
                               glm::bvec3(false, false, false),
                               glm::bvec2(true, true),
                               glm::bvec2(true, true)));
    sides.push_back(OffsetSide(glm::bvec3(true,  false, false),
                               glm::bvec3(false, false, true),
                               glm::bvec3(false, true, false),
                               glm::bvec2(true, true),
                               glm::bvec2(true, true)));
    // x
    sides.push_back(OffsetSide(glm::bvec3(false, true, false),
                               glm::bvec3(false, false, true),
                               glm::bvec3(false, false, false),
                               glm::bvec2(true, true),
                               glm::bvec2(true, true)));
    sides.push_back(OffsetSide(glm::bvec3(false, true, false),
                               glm::bvec3(false, false, true),
                               glm::bvec3(true, false, false),
                               glm::bvec2(true, true),
                               glm::bvec2(true, true)));

    /*
    // sides
    // x
    sides.push_back(OffsetSide(glm::bvec3(1, 0, 0),
                               glm::bvec3(0, 1, 0),
                               glm::bvec3(0, 0, 0),
                               glm::bvec2(1, 1),
                               glm::bvec2(1, 1)));
    sides.push_back(OffsetSide(glm::bvec3(1, 0, 0),
                               glm::bvec3(0, 1, 0),
                               glm::bvec3(0, 0, 1),
                               glm::bvec2(1, 1),
                               glm::bvec2(1, 1)));
    // y
    sides.push_back(OffsetSide(glm::bvec3(1, 0, 0),
                               glm::bvec3(0, 0, 1),
                               glm::bvec3(0, 0, 0),
                               glm::bvec2(1, 1),
                               glm::bvec2(1, 1)));
    sides.push_back(OffsetSide(glm::bvec3(1, 0, 0),
                               glm::bvec3(0, 0, 1),
                               glm::bvec3(0, 1, 0),
                               glm::bvec2(1, 1),
                               glm::bvec2(1, 1)));
    // x
    sides.push_back(OffsetSide(glm::bvec3(0, 1, 0),
                               glm::bvec3(0, 0, 1),
                               glm::bvec3(0, 0, 0),
                               glm::bvec2(1, 1),
                               glm::bvec2(1, 1)));
    sides.push_back(OffsetSide(glm::bvec3(0, 1, 0),
                               glm::bvec3(0, 0, 1),
                               glm::bvec3(1, 0, 0),
                               glm::bvec2(1, 1),
                               glm::bvec2(1, 1)));
                               */
  }
}


Offsets::Offsets(bool x, bool y, bool z) :
    x(glm::bvec2(!x, x)),
    y(glm::bvec2(!y, y)),
    z(glm::bvec2(!z, z)),
    corners(std::vector<glm::bvec3>()),
    edges(std::vector<OffsetEdge>()),
    sides(std::vector<OffsetSide>()) { 
  // compute single corner
  glm::uvec3 c= glm::uvec3(this->x.y,
                           this->y.y,
                           this->z.y);

  this->corners.push_back(c);

  // compute edges from corner
  this->edges.push_back(OffsetEdge(glm::uvec3(1, 0, 0),
                                   glm::uvec3(0, c.y, c.z),
                                   this->x));
  this->edges.push_back(OffsetEdge(glm::uvec3(0, 1, 0),
                                   glm::uvec3(c.x, 0, c.z),
                                   this->y));
  this->edges.push_back(OffsetEdge(glm::uvec3(0, 0, 1),
                                   glm::uvec3(c.x, c.y, 0),
                                   this->z));

  // compute sides from corner
  this->sides.push_back(OffsetSide(glm::uvec3(1, 0, 0),
                                   glm::uvec3(0, 1, 0),
                                   glm::uvec3(0, 0, c.z),
                                   this->x,
                                   this->y));
  this->sides.push_back(OffsetSide(glm::uvec3(1, 0, 0),
                                   glm::uvec3(0, 0, 1),
                                   glm::uvec3(0, c.y, 0),
                                   this->x,
                                   this->z));
  this->sides.push_back(OffsetSide(glm::uvec3(0, 1, 0),
                                   glm::uvec3(0, 0, 1),
                                   glm::uvec3(c.x, 0, 0),
                                   this->y,
                                   this->z));
}

}
}
}
}