#include "pipeline\light-management\Tiled\TiledLightManager.h"

namespace nTiled {
namespace pipeline {

// ============================================================================
// TiledLightManager
// ----------------------------------------------------------------------------
//  Constructor 
// ----------------------------------------------------------------------------
TiledLightManager::TiledLightManager(const world::World& world,
                                     const state::View& view,
                                     GLuint tile_width, GLuint tile_height,
                                     const LightProjector& projector) :
  world(world),
  view(view),
  light_grid(LightGrid(view.viewport.x, view.viewport.y,
                       tile_width, tile_height)),
  projector(projector) {
}


// ----------------------------------------------------------------------------
//  Construct Grid
// ----------------------------------------------------------------------------
void TiledLightManager::constructGridFrame() {
  this->clearGrid();
  this->buildGrid();
  this->finaliseGrid();
}

// ----------------------------------------------------------------------------
void TiledLightManager::clearGrid() {
  this->light_grid.clearGrid();
}

void TiledLightManager::buildGrid() {
  // Build new grid with projector
  int index = 0;
  glm::uvec4 affected_tiles;
  for (const world::PointLight* light : this->world.p_lights) {
    if (this->projector.computeProjection(*light,
                                          this->view.camera,
                                          this->view.viewport,
                                          glm::uvec2(this->light_grid.tile_width,
                                                     this->light_grid.tile_height),
                                          affected_tiles)) {
      this->light_grid.incrementTiles(affected_tiles, index);
    }
    index++;
  }
}

void TiledLightManager::finaliseGrid() {
  this->light_grid.finaliseGrid();
}


// ============================================================================
// TiledLightManagerBuilder
// ----------------------------------------------------------------------------
//  Constructor 
// ----------------------------------------------------------------------------
TiledLightManagerBuilder::TiledLightManagerBuilder() { }

TiledLightManager* TiledLightManagerBuilder::constructNewTiledLightManager(
    const world::World& world,
    const state::View& view,
    GLuint tile_width,
    GLuint tile_height,
    const LightProjector& projector) const {
  return new TiledLightManager(world, view, 
                               tile_width, tile_height, 
                               projector);
}


} // pipeline
} // nTiled