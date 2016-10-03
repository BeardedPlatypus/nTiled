/*! @file DeferredClusteredShader.h
 *  @brief DeferredClusteredShader.h contains the definition of the 
 *         DeferredClusteredShader which implements a Deferred rendering 
 *         approach in combination with Clustered shading.
 */
#pragma once

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "DeferredShader.h"
#include "pipeline\light-management\clustered\ClusteredLightManager.h"

namespace nTiled {
namespace pipeline {

/*! @brief DeferredClusteredShader implements the Clustered Shading algorithm 
 *         within a Deferred pipeline.
 *         It inherits from the DeferredShader.
 */
class DeferredClusteredShader : public DeferredShader {
public:
  /*! @brief Construct a new DeferredClusteredShader with the given parameters
   * 
   * @param shader_id The DeferredShaderId of this new DeferredShader
   * @param path_geometry_pass_vertex_shader Path to the geometry pass 
   *                                         vertex shader file.
   * @param path_geometry_pass_fragment_shader Path to the  geometry pass 
   *                                           fragment shader file.
   * @param path_light_pass_vertex_shader Path to the light pass vertex 
   *                                      shader file.
   * @param path_light_pass_fragment_shader Path to the light pass 
   *                                        fragment shader file.
   * @param world Reference to the world containing the objects and lights this 
   *              Shader observes
   * @param view Reference to the View and Camera this shader uses to shade.
   * @param p_output_buffer openGL pointer to the output buffer to which
   *                        this DeferredClusteredShader should render.
   * @param tile_size The tilesize in pixels the Tiled Shading algorithm
   *                  will use.
   * @param light_manager_builder Reference to the ClusteredLightManagerBuilder
   *                              this DeferredClusteredShader will use to 
   *                              obtain a ClusteredLightManager.
   */
  DeferredClusteredShader(DeferredShaderId shader_id,
                          const std::string& path_geometry_pass_vertex_shader,
                          const std::string& path_geometry_pass_fragment_shader,
                          const std::string& path_light_pass_vertex_shader,
                          const std::string& path_light_pass_fragment_shader,
                          const world::World& world,
                          const state::View& view,
                          GLint p_output_buffer,
                          glm::uvec2 tile_size,
                          const ClusteredLightManagerBuilder& light_manager_builder);

  virtual void renderGeometryPass() override;
  virtual void renderLightPass() override;

protected:
  // --------------------------------------------------------------------------
  // render sub-functions
  // --------------------------------------------------------------------------
  /*! @brief Load the light clustering. */
  virtual void loadLightClustering();

  // --------------------------------------------------------------------------
  //  Member functions
  // --------------------------------------------------------------------------
  /*! @brief ClusteredLightManager of this DeferredClusteredShader */
  ClusteredLightManager* p_clustered_light_manager;

  // --------------------------------------------------------------------------
  //  openGL LightManagement datastructures
  // --------------------------------------------------------------------------
  /*! @brief GLuint pointer to the summed indices buffer */
  GLuint summed_indices_buffer;

  /*! @brief GLuint pointer to the k_index map computed by the
   *         ClusteredLightManager*/
  GLuint k_index_map;

  /*! @brief GLuint pointer to the Shader Storage Buffer Object storing
   *         the light clustering of this DeferredClusteredShader
   */
  GLuint light_cluster_buffer;

  /*! @brief GLuint pointer to the Shader Storage Buffer Object storing
   *         the light index of this DeferredClusteredShader
   */
  GLuint light_index_buffer;
};

}
}
