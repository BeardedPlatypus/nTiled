#pragma once

// ----------------------------------------------------------------------------
//  nTiled Headers
// ----------------------------------------------------------------------------
#include "HashedLightManager.h"
#include "HashedLightManagerLogged.h"

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


class HashedLightManagerLoggedBuilder : public HashedLightManagerBuilder {
public:
  /*! @brief Construct a new HashedLightmanagerLoggedBuilder with the
   *         the specified logger object
   *
   * @param logger Reference to the ExecutionTimeLogger used in all 
   *               HashedLightManagerLogged created by this
   *               HashedLightmanagerLoggedBuilder.
   */
  HashedLightManagerLoggedBuilder(logged::ExecutionTimeLogger& logger);

  virtual HashedLightManager* constructNewHashedLightManager(
    const world::World& world,
    HashedConfig config) const;

private:
  /*! @brief Reference to the ExecutionTimeLogger object which logs
   *         the execution time of the constructed HashedLightManagers.
   */
  logged::ExecutionTimeLogger& logger;
};

}
}
}