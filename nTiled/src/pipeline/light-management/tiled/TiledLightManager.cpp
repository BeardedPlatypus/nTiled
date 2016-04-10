#include "pipeline\light-management\Tiled\TiledLightManager.h"

// ----------------------------------------------------------------------------
//  System Libraries
// ----------------------------------------------------------------------------
#include <iostream>

// ----------------------------------------------------------------------------
namespace nTiled {
namespace pipeline {

// ----------------------------------------------------------------------------
//  TiledLightManager class
// ----------------------------------------------------------------------------
// constructor 
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


void TiledLightManager::constructGridFrame() {
  // Clear Grid
  this->light_grid.clearGrid();

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
      if (affected_tiles.x < 0) {
        std::cout << "x: " << affected_tiles.x << std::endl;
      }
      if (affected_tiles.y < 0) {
        std::cout << "y: " << affected_tiles.y << std::endl;
      }
      if (affected_tiles.z >= this->light_grid.n_x) {
        std::cout << "z: " << affected_tiles.z << std::endl;
      }
      if (affected_tiles.w >= this->light_grid.n_y) {
        std::cout << "w: " << affected_tiles.w << std::endl;
      }

      this->light_grid.incrementTiles(affected_tiles, index);
    }
    //affected_tiles = glm::uvec4(0, 0, this->state.viewport.x, this->state.viewport.y);
    //this->light_grid.incrementTiles(affected_tiles, index);
    index++;
  }

  // Finalise grid
  this->light_grid.finaliseGrid();
}

} // pipeline
} // nTiled