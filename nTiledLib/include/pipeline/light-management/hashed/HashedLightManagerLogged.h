#pragma once


// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "HashedLightManager.h"
#include "log\Logger.h"


namespace nTiled {
namespace pipeline {
namespace hashed {

class HashedLightManagerLogged : public HashedLightManager {
public:
  /*! @brief Constructa new HashedLightmanagerLogged, acting as a 
   *         HashedLightmanager but tracking execution time
   * 
   * @param world The world this HashedLightManagerLogged is observing
   * @param hashed_config The configuration values for this 
   *                      HashedLightManagerLogged
   * @param logger The ExecutionTimeLogger responsible for logging the time
   */
  HashedLightManagerLogged(const world::World& world,
                           HashedConfig hashed_config,
                           logged::ExecutionTimeLogger& logger);

  
  virtual void addConstructedSLTs() override;
  virtual void constructEmptyLightOctree() override;
  virtual void constructSLTs() override;
  virtual void constructLinklessOctree() override;

private:
  logged::ExecutionTimeLogger& logger;
};

}
}
}
