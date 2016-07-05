#pragma once

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "DeferredShader.h"
#include "pipeline\light-management\tiled\TiledLightManager.h"
#include "pipeline\light-management\tiled\BoxProjector.h"

namespace nTiled {
namespace pipeline {

class DeferredTiledShader : public DeferredShader {
public:
  DeferredTiledShader(DeferredShaderId shader_id,
                      const std::string& path_geometry_pass_vertex_shader,
                      const std::string& path_geometry_pass_fragment_shader,
                      const std::string& path_light_pass_vertex_shader,
                      const std::string& path_light_pass_fragment_shader,
                      const world::World& world,
                      const state::View& view,
                      GLint p_output_buffer,
                      glm::uvec2 tile_size,
                      const TiledLightManagerBuilder& light_manager_builder);

  virtual void renderGeometryPass() override;
  virtual void renderLightPass() override;

protected:
  // --------------------------------------------------------------------------
  // render sub-functions
  // --------------------------------------------------------------------------
  virtual void loadLightGrid();

  // --------------------------------------------------------------------------
  // LightManagement Functions
  // --------------------------------------------------------------------------
  /*! Sphere projector of this DeferredTiledShader */
  const BoxProjector projector;
  /*! TiledLightManager of this DeferredTiledShader */
  TiledLightManager* p_light_manager;

  // --------------------------------------------------------------------------
  //  openGL LightManagement datastructures
  // --------------------------------------------------------------------------
  /*!
  GLuint pointer to the Shader Storage Buffer Object storing
  the light grid of this DeferredTiledShader
  */
  GLuint light_grid_buffer;
  /*!
  GLuint pointer to the Shader Storage Buffer Object storing
  the light index of this DeferredTiledShader
  */
  GLuint light_index_buffer;
};

}
}
