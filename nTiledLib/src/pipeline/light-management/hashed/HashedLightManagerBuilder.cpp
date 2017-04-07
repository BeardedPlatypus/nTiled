#include "pipeline\light-management\hashed\HashedLightManagerBuilder.h"

namespace nTiled {
namespace pipeline {
namespace hashed {

HashedLightManagerBuilder::HashedLightManagerBuilder() { }


HashedLightManager* HashedLightManagerBuilder::constructNewHashedLightManager(
    const world::World& world,
    HashedConfig config) const {
  return new HashedLightManager(world, config);
}


HashedLightManagerLoggedBuilder::HashedLightManagerLoggedBuilder(
  logged::ExecutionTimeLogger& logger) : logger(logger) { }


HashedLightManager* HashedLightManagerLoggedBuilder::constructNewHashedLightManager(
    const world::World& world,
    HashedConfig config) const {
  return new HashedLightManagerLogged(world, config, this->logger);
}


}
}
}