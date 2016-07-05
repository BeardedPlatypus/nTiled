#include "pipeline\light-management\clustered\ClusteredLightManagerLogged.h"


namespace nTiled {
namespace pipeline {

// ----------------------------------------------------------------------------
//  ClusteredLightManagerLogged
// ----------------------------------------------------------------------------
ClusteredLightManagerLogged::ClusteredLightManagerLogged(
    const state::View& view,
    const world::World& world,
    glm::uvec2 tile_size,
    GLuint depth_texture,
    logged::ExecutionTimeLogger logger) :
  ClusteredLightManager(view, world, tile_size, depth_texture),
  logger(logger) {
}


void ClusteredLightManagerLogged::computeKeys() {
  this->logger.startLog(std::string("ClusteredLightManager::computeKeys"));
  ClusteredLightManager::computeKeys();
  this->logger.endLog();
}

void ClusteredLightManagerLogged::sortAndCompactKeys() {
  this->logger.startLog(std::string("ClusteredLightManager::sortAndCompactKeys"));
  ClusteredLightManager::sortAndCompactKeys();
  this->logger.endLog();
}

void ClusteredLightManagerLogged::clearClustering() {
  this->logger.startLog(std::string("ClusteredLightManager::clearClustering"));
  ClusteredLightManager::clearClustering();
  this->logger.endLog();
}

void ClusteredLightManagerLogged::buildClustering() {
  this->logger.startLog(std::string("ClusteredLightManager::buildClustering"));
  ClusteredLightManager::buildClustering();
  this->logger.endLog();
}

void ClusteredLightManagerLogged::finaliseClustering() {
  this->logger.startLog(std::string("ClusteredLightManager::finaliseClustering"));
  ClusteredLightManager::finaliseClustering();
  this->logger.endLog();
}


// ----------------------------------------------------------------------------
//  ClusteredLightManagerLoggedBuilder
// ----------------------------------------------------------------------------
ClusteredLightManagerLoggedBuilder::ClusteredLightManagerLoggedBuilder(
  logged::ExecutionTimeLogger& logger) : logger(logger) {
}

ClusteredLightManager* ClusteredLightManagerLoggedBuilder::constructNewClusteredLightManager(
  const state::View& view, const world::World& world,
  glm::uvec2 tile_size, GLuint depth_texture) const {
  return new ClusteredLightManagerLogged(
    view, world, tile_size, depth_texture, this->logger);
}


} // pipeline
} // nTiled