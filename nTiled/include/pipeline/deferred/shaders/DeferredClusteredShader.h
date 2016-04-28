#pragma once

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "DeferredShader.h"
#include "pipeline\light-management\clustered\ClusteredLightManager.h"

namespace nTiled {
namespace pipeline {

class DeferredClusteredShader : public DeferredShader {
public:
  DeferredClusteredShader(DeferredShaderId shader_id,
                          const std::string& path_geometry_pass_vertex_shader,
                          const std::string& path_geometry_pass_fragment_shader,
                          const std::string& path_light_pass_vertex_shader,
                          const std::string& path_light_pass_fragment_shader,
                          const world::World& world,
                          const state::View& view,
                          GLint p_output_buffer,
                          glm::uvec2 tile_size);
  ~DeferredClusteredShader();

  virtual void renderGeometryPass() override;
  virtual void renderLightPass() override;

protected:
  /*! ClusteredLightManager of this DeferredClusteredShader */
  ClusteredLightManager clustered_light_manager;

  // --------------------------------------------------------------------------
  //  openGL LightManagement datastructures
  // --------------------------------------------------------------------------
  /*! GLuint pointer to the summed indices buffer */
  GLuint summed_indices_buffer;
  /*! GLuint pointer to the k_index map computed by the ClusteredLightManager*/
  GLuint k_index_map;
  /*!
  GLuint pointer to the Shader Storage Buffer Object storing
  the light clustering of this DeferredClusteredShader
  */
  GLuint light_cluster_buffer;
  /*!
  GLuint pointer to the Shader Storage Buffer Object storing
  the light index of this DeferredClusteredShader
  */
  GLuint light_index_buffer;

  
};

}
}
