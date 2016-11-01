/*! @file FilleElement.h 
 *  @brief FillElement.h contains the definition of both the FillElement and
 *         FillDirection used during the fill step in the first phase of 
 *         SLT construction.
 */
#pragma once

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <glm\glm.hpp>

namespace nTiled {
namespace pipeline {
namespace hierarchical {
namespace slt {

/*! @brief FillDirection describes the 8 possible corners from which the
 *         fill algorithm of SLT construction can be started.
 */
enum class FillDirection {
  LeftLowerClose,
  RightLowerClose,
  LeftUpperClose,
  RightUpperClose,
  LeftLowerFar,
  RightLowerFar,
  LeftUpperFar,
  RightUpperFar
};


/*! @brief FillElement describes a single element when building the first
 *         step of an SLT
 */
struct FillElement {
  /*! @brief Construct a new FillElment with the given position within
   *         the lattice, and the given FillDirection.
   *
   *  @param pos The position within the lattice of this FillElement.
   *  @param direction The FillDirection of this FillElement.
   */
  FillElement(glm::uvec3 pos,
              FillDirection direction);

  /*! @brief Default constructor. */
  FillElement();

  /*! @brief The position within the lattice of this FillElement. */
  glm::uvec3 pos;
  /*! @brief The FillDirection of this FillElement. */
  FillDirection direction;
};


} // slt
} // hierarchical
} // pipeline
} // nTiled