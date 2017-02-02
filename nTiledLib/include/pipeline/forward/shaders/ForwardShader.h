/*! @file ForwardShader.h
 *  @brief ForwardShader.h contains the defition of the ForwardShader used
 *         in the Forward shading pipeline.
 */
#pragma once

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "pipeline\forward\shaders\ForwardShaderId.h"

#include "world\World.h"
#include "state\StateView.h"

#include "pipeline\PipelineObject.h"
#include "pipeline\PipelineLight.h"

namespace nTiled {
namespace pipeline {

/*! @brief ForwardShader is responsible for rendering any objects assigned
 *         to it, with the specified openGL shader code utilising a forward 
 *         pipeline.
 */
class ForwardShader {
public:
  // -------------------------------------------------------------------------
  //  Constructor
  // -------------------------------------------------------------------------
  /*! @brief Construct a new ForwardShader
   *
   * Construct a new ForwardShader with the specified ForwardShaderId
   * and vertex and fragment shader paths
   *
   * @param shader_id The ForwardShaderId of this new ForwardShader
   * @param path_vertex_shader Path to the vertex shader file.
   * @param path_fragment_shader Path to the fragment shader file.
   * @param world Reference to the world containing the objects and lights 
   *              this new ForwardShader observes
   * @param view Reference to the View and Camera this ForwardShader uses 
   *             to shade.
   * @param p_output_buffer openGL pointer to the output buffer to which
   *                        this ForwardShader should render.
  */
  ForwardShader(ForwardShaderId shader_id,
                const std::string& path_vertex_shader,
                const std::string& path_fragment_shader,
                const world::World& world,
                const state::View& view,
                GLint p_output_buffer);


  ForwardShader(ForwardShaderId shader_id,
                const world::World& world,
                const state::View& view,
                GLint p_output_buffer);
  // --------------------------------------------------------------------
  //  Attribute Getters
  // --------------------------------------------------------------------
  /*! @brief Get the ForwardShaderId of this ForwardShader
   *
   * @return The ForwardShaderId of this ForwardShader.
  */
  ForwardShaderId getId() const { return this->id; }

  // --------------------------------------------------------------------
  //  Render methods
  // --------------------------------------------------------------------
  /*! @brief Render all objects in the world this Shader observes that 
   *         indicate they are rendered with this shader.
   */
  virtual void render();

  /*! @brief  Set the output buffer of this ForwardShader to p_output_buffer
   * 
   * @param p_output_buffer openGL pointer to the new output buffer of this 
   *                        ForwardShader
   */
  virtual void setOutputBuffer(GLint p_output_buffer) { 
    this->p_output_buffer = p_output_buffer; 
  }

protected:
  // --------------------------------------------------------------------------
  //  glsl management
  // --------------------------------------------------------------------------
  /*! @brief Load the ShaderFiles specified in with the paths during 
   *         construction.
   * 
   * @param path_vert_shader Path to the vertex shader of this ForwardShader
   * @param path_frag_shader Path tot he fragment shader of this FarwardShader
   */
  virtual void loadShaders(const std::string& path_vert_shader,
                           const std::string& path_frag_shader);

  /*! @brief Load all objects in the world
   */
  virtual void loadObjects();

  /*! @brief Construct a PipelineObject from the specified Object and add 
   *         to this shader
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

  /*! @brief Construct a PipelineLight from the specified Light and add to 
   *         this Shader
   * 
   * @param light Reference to the PointLight of which a new PipelineLight
   *              should be created.
   */
  virtual void constructPipelineLight(const world::PointLight& light);

  // --------------------------------------------------------------------------
  //  Render subfunctions
  // --------------------------------------------------------------------------
  /*! @brief render all objects in this ForwardShader. */
  virtual void renderObjects();

  // --------------------------------------------------------------------------
  //  Attributes
  // --------------------------------------------------------------------------

  // client side attributes
  // --------------------------------------------------------------------------
  /*! @brief ForwardShaderId of this ForwardShader */
  const ForwardShaderId id;

  /*! @brief The world this shader observes */
  const world::World& world;
  /*! @brief The view this shader uses */
  const state::View& view;

  // pipeline objects
  // --------------------------------------------------------------------------
  /*! Vector containing pointers to all PipelineObjects of this ForwardShader 
   */
  std::vector<PipelineObject*> ps_obj;

  /*! Vector containing all PipelineLights of this ForwardShader. */
  std::vector<PipelineLight> lights;

  // glsl attributes
  // --------------------------------------------------------------------------
  /*! @brief GLuint pointer to the ShaderProgram constructed with LoadShaders */
  GLuint shader;
  /*! @brief GLuint pointer to the Uniform Buffer Object holding the internal
   *         light data.
   */
  GLuint light_ubo;

  /*! @brief output buffer that should be restored upon changing the 
   *         framebuffer, and to which the final result should be written 
   */
  GLint p_output_buffer;
};


} // pipeline
} // nTiled
