#include "pipeline\light-management\hashed\HashedLightManagerLogged.h"


namespace nTiled {
namespace pipeline {
namespace hashed {

// ----------------------------------------------------------------------------
//  Constructor
// ----------------------------------------------------------------------------
HashedLightManagerLogged::HashedLightManagerLogged(const world::World& world,
                                                   HashedConfig hashed_config,
                                                   logged::ExecutionTimeLogger& logger) :
  HashedLightManager(world, hashed_config),
  logger(logger) {
}


// ----------------------------------------------------------------------------
//  Time tracked functions
// ----------------------------------------------------------------------------
void HashedLightManagerLogged::constructEmptyLightOctree() {
  this->logger.startLog(std::string("HashedLightManager::constructEmptyLightOctree"));
  HashedLightManager::constructEmptyLightOctree();
  this->logger.endLog();
}


void HashedLightManagerLogged::constructSLTs() {
  this->logger.startLog(std::string("HashedLightManager::constructSLTs"));
  HashedLightManager::constructSLTs();
  this->logger.endLog();
}


void HashedLightManagerLogged::addConstructedSLTs() {
  this->logger.startLog(std::string("HashedLightManager::addConstructedSLTs"));
  HashedLightManager::addConstructedSLTs();
  this->logger.endLog();
}


void HashedLightManagerLogged::constructLinklessOctree() {
  this->logger.startLog(std::string("HashedLightManager::constructLinklessOctree"));
  HashedLightManager::constructLinklessOctree();
  this->logger.endLog();
}


}
}
}