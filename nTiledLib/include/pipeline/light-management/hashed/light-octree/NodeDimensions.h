#pragma once

#include <glm/glm.hpp>

namespace nTiled {
namespace pipeline {
namespace hashed {

struct NodeDimensions {
public:
  /*! @brief Construct a new SLTNodeDimensions with the given origin and size
   */
  NodeDimensions(glm::vec3 origin,
                 double size);

  /*! @brief The origin of the corresponding node*/
  glm::vec3 origin;
  /*! @brief The size of the corresponding node*/
  double size;

  /*! @brief Get the index of the next SLTNode based on this SLTNodeDimensions
   *         and the specified point
   * 
   * @param point The point which is queried
   * 
   * @returns The index of the next node.
   */
  glm::bvec3 getNextIndex(glm::vec3 point) const;

  /*! @brief Get the SLTNodeDimensions of the next node based on the index 
   *         provided
   *
   * @param index The index of the next node
   * 
   * @returns The SLTNodeDimensions of the next node corresponding with index
   */
  NodeDimensions getNextDimensions(glm::bvec3 index) const;
};

}
}
}