/*! @file ForwardClusteredShader.h
 *  @brief ForwardClusteredShader.h contains the definition of the 
 *         ForwardClusteredShader which implements a Forward rendering 
 *          approach in combination with Clustered shading.
 */
#pragma once

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "ForwardShader.h"
#include "pipeline\light-management\clustered\ClusteredLightManager.h"
#include "pipeline\forward\DepthBuffer.h"


namespace nTiled {
namespace pipeline {

/*! @brief ForwardClusteredShader implements the Clustered Shading algorithm 
 *         within a Forward pipeline.
 *         It inherits from the ForwardShader.
 */
class ForwardClusteredShader : public ForwardShader {
public:
  /*! @brief Construct a new ForwardClusteredShader with the given parameters
   * 
   * @param shader_id The ForwardShaderId of this new ForwardClusteredShader
   * @param path_vertex_shader Path to the vertex shader of this 
   *                           ForwardClusteredShader
   * @param path_fragment_shader Path to the fragment shader of this
   *                             ForwardClusteredShader
   * @param world Reference to the World this ForwardTiledShader observes
   * @param view Reference to the View this ForwardTiledShader observes
   * @param p_output_buffer openGL reference to the output buffer this
   *                        ForwardClusteredShader should render to.
   * @param tile_size The tilesize in pixels the Clustered Shading algorithm
   *                  will use.
   * @param light_manager_builder Reference to the 
   *                              ClusteredLightManagerBuilder this 
   *                              ForwardClusteredShader will use to 
   *                              obtain a ClusteredLightManager.
   */
  ForwardClusteredShader(ForwardShaderId shader_id,
                         const std::string& path_vertex_shader,
                         const std::string& path_fragment_shader,
                         const world::World& world,
                         const state::View& view,
                         GLint p_output_buffer,
                         glm::uvec2 tile_size,
                         const ClusteredLightManagerBuilder& light_manager_builder);

  virtual void render() override;

protected:
  // --------------------------------------------------------------------------
  // render sub-functions
  // --------------------------------------------------------------------------
  /*! @brief Render the depth pass. */
  virtual void depthPass();
  /*! @brief Load the light clustering. */
  virtual void loadLightClustering();
  /*! @brief Copy the result to the output buffer. */
  virtual void copyResult();

  // --------------------------------------------------------------------------
  //  Member functions
  // --------------------------------------------------------------------------
  /*! @brief DepthBuffer of this ForwardClusteredShader*/
  DepthBuffer depth_buffer;

  /*! @brief Depth pass program of this ForwardClusteredShader */
  GLuint depth_pass_shader;

  /*! @brief Depth copy program of this ForwardClusteredShader */
  GLuint depth_copy_shader;

  /*! @brief ClusteredLightManager of this ForwardClusteredShader */
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
   *         the light clustering of this ForwardClusteredShader
  */
  GLuint light_cluster_buffer;
  /*! @brief GLuint pointer to the Shader Storage Buffer Object storing
   *         the light index of this ForwardClusteredShader
   */
  GLuint light_index_buffer;

  /*! @brief Pointer to a fullscreen quad PipelineObject. */
  PipelineObject* fullscreen_quad;
};

}
}