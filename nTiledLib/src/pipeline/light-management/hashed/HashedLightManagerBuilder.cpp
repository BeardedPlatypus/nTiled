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

}
}
}