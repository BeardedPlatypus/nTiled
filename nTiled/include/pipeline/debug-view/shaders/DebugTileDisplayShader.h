#pragma once

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <vector>

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "pipeline\debug-view\shaders\DebugShader.h"
#include "pipeline\PipelineObject.h"
#include "pipeline\light-management\tiled\TiledLightManager.h"
#include "pipeline\light-management\tiled\BoxProjector.h"

namespace nTiled {
namespace pipeline {

class DebugTileDisplayShader : public DebugShader {
public:
  DebugTileDisplayShader(GLfloat z_value,
                         const state::View& view,
                         const world::World& world,
                         glm::uvec2 tile_size);
  ~DebugTileDisplayShader();

  virtual void render() override;

private:
  void drawGrid();
  void drawTiles();

  const state::View& view;
  const world::World& world;

  const BoxProjector projector;
  TiledLightManager tiled_light_manager;

  // --------------------------------------------------------------------------
  //  OpenGL related attributes
  // --------------------------------------------------------------------------
  GLuint tile_shader_program;
  GLuint grid_shader_program;

  std::vector<PipelineObject*> tiles;
  PipelineObject* p_grid;
};

} // pipeline
} // nTiled