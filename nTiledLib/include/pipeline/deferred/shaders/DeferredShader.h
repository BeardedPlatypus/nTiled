/*! @file DeferredShader.h
 *  @brief DeferredShader.h contains the defition of the DeferredShader used
 *         in the Deferred shading pipeline.
 */
#pragma once

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <string>

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "pipeline\deferred\shaders\DeferredShaderId.h"

#include "world\World.h"
#include "state\StateView.h"

#include "pipeline\PipelineObject.h"
#include "pipeline\PipelineLight.h"

#include "pipeline\deferred\GBuffer.h"

namespace nTiled {
namespace pipeline {

/*! @brief DeferredShader is responsible for rendering any objects assigned 
 *         to it, with the specified openGL shader code utilising a deferred 
 *         pipeline.
*/
class DeferredShader {
public:
  // --------------------------------------------------------------------------
  //  Constructor
  // --------------------------------------------------------------------------
  /*! @brief Construct a new DeferredShader with the specified parameters

   * Construct a new DeferredShader with the specified a DeferredShaderId
   * and vertex and fragment shader paths to the geometry and light pass
   * shaders.
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
   *                        this DeferredShader should render.
   * 
   * @return A new DeferredShader of DeferredShaderId with the given shader 
   *         files paths.
   */
  DeferredShader(DeferredShaderId shader_id,
                 const std::string& path_geometry_pass_vertex_shader,
                 const std::string& path_geometry_pass_fragment_shader,
                 const std::string& path_light_pass_vertex_shader,
                 const std::string& path_light_pass_fragment_shader,
                 const world::World& world,
                 const state::View& view,
                 GLint p_output_buffer);
 
  // --------------------------------------------------------------------
  //  Attribute Getters
  // --------------------------------------------------------------------
  /*! @brief Get the DeferredShaderId of this DeferredShader
   *
   * @return The ForwardShaderId of this ForwardShader.
   */
  DeferredShaderId getId() const { return this->id; }

  // --------------------------------------------------------------------------
  //  Render methods
  // --------------------------------------------------------------------------
  /*! @brief Render all objects of this DeferredShader
   */
  virtual void render();

  /*! @brief  Set the output buffer of this DeferredShader to p_output_buffer
   * 
   * @param p_output_buffer the new output buffer of this shader
   */
  virtual void setOutputBuffer(GLint p_output_buffer) { 
    this->p_output_buffer = p_output_buffer; 
  }

protected:
  // --------------------------------------------------------------------------
  //  glsl management
  // --------------------------------------------------------------------------
  /*! @brief Load the ShaderFiles specified in with the paths during 
   *         construction
   *
   * @param path_geometry_pass_vertex_shader Path to the geometry pass 
   *                                         vertex shader file.
   * @param path_geometry_pass_fragment_shader Path to the  geometry pass 
   *                                           fragment shader file.
   * @param path_light_pass_vertex_shader Path to the light pass vertex 
   *                                      shader file.
   * @param path_light_pass_fragment_shader Path to the light pass 
   *                                        fragment shader file.
   */
  virtual void loadShaders(const std::string& path_geometry_vert_shader,
                           const std::string& path_geometry_frag_shader,
                           const std::string& path_light_vert_shader,
                           const std::string& path_light_frag_shader);

  /*! @brief Load all objects in the world
   */
  virtual void loadObjects();

  /*! @brief Construct a PipelineObject from the specified Object and 
   *         add to this shader
   * 
   * @param obj Reference to the object of which a PipelineObject should
   *            be constructed.
   * @param vao openGL pointer to Vertex Array Object (vao) of this new 
   *            PipelineObject
   * @param element_buffer openGL pointer to the element buffer of this
   *                       PipelineObject
   */
  virtual void constructPipelineObject(const world::Object& obj,
                                       GLuint vao,
                                       GLuint element_buffer);

  /*! @brief Load all lights in the world
   */
  virtual void loadLights();

  /*! @brief Construct a PipelineLight from the specified Light and 
   *         add to this Shader
   *
   * @param light Reference to the PointLight of which a new PipelineLight
   *              should be created.
   */
  virtual void constructPipelineLight(const world::PointLight& light);

  // --------------------------------------------------------------------------
  //  Render subfunctions
  // --------------------------------------------------------------------------
  /*! @brief Render the geometry (first) pass of this DeferredShader
   *
   * Render all the attributes needed for the light (second) pass
   * of this DeferredShader into the GBuffer of this DeferredShader.
   */
  virtual void renderGeometryPass();

  /*! @brief Render all the objects for the Geometry pass of this 
   *         DeferredShader.
   */
  virtual void renderGeometryPassObjects();

  /*! @brief  Render the light (second) pass of this DeferredShader
   *
   * Render a quad and use the GBuffer to obtain the attributes of the
   * scene to render the frame.
   */
  virtual void renderLightPass();

  /*! @brief Render all objects for the Light pass of this 
   *         DeferredShader
   */
  virtual void renderLightPassObjects();

  /*! @brief Render the different GBuffers to frame.
   */
  virtual void renderBuffers();

  // --------------------------------------------------------------------------
  //  Attributes
  // --------------------------------------------------------------------------

  // client side attributes
  // --------------------------------------------------------------------------
  /*! @brief DeferredShaderId of this DeferredShader */
  const DeferredShaderId id;

  /*! @brief GBuffer of this DeferredShader */
  GBuffer gBuffer;

  /*! @brief The world this shader observes */
  const world::World& world;
  /*! @brief The view this shader uses */
  const state::View& view;

  // pipeline objects
  // --------------------------------------------------------------------------
  /*! Vector containing pointers to all PipelineObjects of this DeferredShader 
   */
  std::vector<PipelineObject*> ps_obj;

  /*! Vector containing all PipelineLights of this DeferredShader. */
  std::vector<PipelineLight> lights;

  // glsl attributes
  // --------------------------------------------------------------------------
  /*! @brief GLuint pointer to the geometry pass ShaderProgam */
  GLuint geometry_pass_sp;
  /*! @brief GLuint pointer to the light pass ShaderProgram */
  GLuint light_pass_sp;

  /*! @brief GLuint pointer to the Uniform Buffer Object holding the internal
   *         light data.
  */
  GLuint light_ubo;

  /*! @brief PipelineObject of the fullscreen quad used in the Light Pass */
  PipelineObject* fullscreen_quad;

  /*! @brief output buffer that should be restored upon changing the 
   *         framebuffer, and to which the final result should be written 
   */
  GLint p_output_buffer;
};

} // pipeline 
} // nTiled