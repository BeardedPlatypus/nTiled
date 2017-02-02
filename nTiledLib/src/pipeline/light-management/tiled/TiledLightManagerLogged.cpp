#include "pipeline\light-management\tiled\TiledLightManagerLogged.h"


namespace nTiled {
namespace pipeline {


// ----------------------------------------------------------------------------
// TiledLightManagerLogged
// ----------------------------------------------------------------------------
TiledLightManagerLogged::TiledLightManagerLogged(const world::World& world,
                                                 const state::View& view,
                                                 GLuint tile_width, GLuint tile_height,
                                                 const LightProjector& projector,
                                                 logged::ExecutionTimeLogger& logger) :
  TiledLightManager(world, view, tile_width, tile_height, projector),
  logger(logger) {
}


void TiledLightManagerLogged::clearGrid() {
  this->logger.startLog(std::string("TiledLightManager::clearGrid"));
  TiledLightManager::clearGrid();
  this->logger.endLog();
}

void TiledLightManagerLogged::buildGrid() {
  this->logger.startLog(std::string("TiledLightManager::buildGrid"));
  TiledLightManager::buildGrid();
  this->logger.endLog();
}

void TiledLightManagerLogged::finaliseGrid() {
  this->logger.startLog(std::string("TiledLightManager::finaliseGrid"));
  TiledLightManager::finaliseGrid();
  this->logger.endLog();
}

// ----------------------------------------------------------------------------
// TiledLightManagerLoggedBuilder
// ----------------------------------------------------------------------------
TiledLightManagerLoggedBuilder::TiledLightManagerLoggedBuilder(
  logged::ExecutionTimeLogger& logger) : logger(logger) { 
}

TiledLightManager* TiledLightManagerLoggedBuilder::constructNewTiledLightManager(
  const world::World& world,
  const state::View& view,
  GLuint tile_width, GLuint tile_height,
  const LightProjector& projector) const {
  return new TiledLightManagerLogged(world,
                                     view,
                                     tile_width,
                                     tile_height,
                                     projector,
                                     this->logger);
}


} // pipeline
} // nTiled