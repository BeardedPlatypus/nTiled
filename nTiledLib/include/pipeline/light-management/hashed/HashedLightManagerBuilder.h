#pragma once

// ----------------------------------------------------------------------------
//  nTiled Headers
// ----------------------------------------------------------------------------
#include "HashedLightManager.h"

namespace nTiled {
namespace pipeline {
namespace hashed {

class HashedLightManagerBuilder {
public:
  /*! @brief Construct a new HashedLightManagerBuilder.
   */
  HashedLightManagerBuilder();

  /*! @brief Construct a new HashedLightManager with this 
   *         HashedLightManagerBuilder with the given world and config
   * 
   * @param world A reference to the world which the new HashedLightManager
   *              observes.
   * @param config The parameters required to construct a new HashedLightManager.
   */
  virtual HashedLightManager* constructNewHashedLightManager(
    const world::World& world,
    HashedConfig config) const;
};

}
}
}