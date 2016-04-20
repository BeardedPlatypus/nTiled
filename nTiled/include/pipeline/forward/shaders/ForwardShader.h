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

/*!
ForwardShader is responsible for rendering any objects assigned to it,
with the specified openGL shader code utilising a forward pipeline.
*/
class ForwardShader {
public:
  // --------------------------------------------------------------------
  //  Constructor
  // --------------------------------------------------------------------
  /*!
  Construct a new ForwardShader

  Construct a new ForwardShader with the specified ForwardShaderId
  and vertex and fragment shader paths

  Args:
  shader_id (ForwardShaderId): the ForwardShaderId of this new
                               ForwardShader
  path_vertex_shader (std::string): path to the vertex shader
                                    file.
  path_fragment_shader (std::string): path to the fragment shader
                                      file.
  world (World&): the world containing the objects and lights this 
                  Shader observes
  view (View&): the view and camera this shader uses to shade.

  Returns:
  A new ForwardShader of ForwardShaderId with the given shader
  files paths.
  */
  ForwardShader(ForwardShaderId shader_id,
                const std::string& path_vertex_shader,
                const std::string& path_fragment_shader,
                const world::World& world,
                const state::View& view,
                GLint p_output_buffer);

  // --------------------------------------------------------------------
  //  Attribute Getters
  // --------------------------------------------------------------------
  /*!
  Get the ForwardShaderId of this ForwardShader

  Returns:
  The ForwardShaderId of this ForwardShader.
  */
  ForwardShaderId getId() const { return this->id; }

  // --------------------------------------------------------------------
  //  Render methods
  // --------------------------------------------------------------------
  /*!
  Render all objects in the world this Shader observes that indicate 
  They are rendered with this shader.
  */
  virtual void render();

  /*!
   Set the output buffer of this ForwardShader to p_output_buffer

   Param:
       p_output_buffer (GLint): the new output buffer of this shader
   */
  virtual void setOutputBuffer(GLint p_output_buffer) { 
    this->p_output_buffer = p_output_buffer; 
  }

protected:
  // --------------------------------------------------------------------------
  //  glsl management
  // --------------------------------------------------------------------------
  /*!
  Load the ShaderFiles specified in with the paths during construction
  */
  virtual void loadShaders(const std::string& path_vert_shader,
                           const std::string& path_frag_shader);

  /*!
   Load all objects in the world
   */
  virtual void loadObjects();

  /*!
   Construct a PipelineObject from the specified Object and add to this shader
   */
  virtual void constructPipelineObject(const world::Object& obj,
                                       GLuint vao,
                                       GLuint element_buffer);

  /*!
   Load all lights in the world
   */
  virtual void loadLights();

  /*!
   Construct a PipelineLight from the specified Light and add to this Shader
   */
  virtual void constructPipelineLight(const world::PointLight& light);

  // --------------------------------------------------------------------------
  //  Render subfunctions
  // --------------------------------------------------------------------------
  virtual void renderObjects();

  // --------------------------------------------------------------------------
  //  Attributes
  // --------------------------------------------------------------------------

  // client side attributes
  // --------------------------------------------------------------------------
  /*! ForwardShaderId of this ForwardShader */
  const ForwardShaderId id;

  /*! The world this shader observes */
  const world::World& world;
  /*! The view this shader uses */
  const state::View& view;

  // pipeline objects
  // --------------------------------------------------------------------------
  std::vector<PipelineObject*> ps_obj;
  std::vector<PipelineLight> lights;

  // glsl attributes
  // --------------------------------------------------------------------------
  /*! GLuint pointer to the ShaderProgram constructed with LoadShaders */
  GLuint shader;
  /*!
  GLuint pointer to the Uniform Buffer Object holding the internal
  light data.
  */
  GLuint light_ubo;

  /*! output buffer that should be restored upon changing the framebuffer, and 
      to which the final result should be written */
  GLint p_output_buffer;
};


} // pipeline
} // nTiled
