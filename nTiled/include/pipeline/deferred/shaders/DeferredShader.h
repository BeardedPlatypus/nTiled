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

/*!
DeferredShader is responsible for rendering any objects assigned to it,
with the specified openGL shader code utilising a deferred pipeline.
*/
class DeferredShader {
public:
  // --------------------------------------------------------------------
  //  Constructor
  // --------------------------------------------------------------------
  /*!
  Construct a new DeferredShader

  Construct a new DeferredShader with the specified a DeferredShaderId
  and vertex and fragment shader paths to the geometry and light pass
  shaders.

  Args:
      shader_id (DeferredShaderId): the DeferredShaderId of this new
                                    DeferredShader
      path_geometry_pass_vertex_shader (const std::string&): path to the
           geometry pass vertex shader file.
      path_geometry_pass_fragment_shader (const std::string&): path to the
           geometry pass fragment shader file.
      path_light_pass_vertex_shader (const std::string&): path to the
           light pass vertex shader file.
      path_geometry_pass_fragment_shader (const std::string&): path to the
          light pass fragment shader file.

      world (World&): the world containing the objects and lights this 
                      Shader observes
      view (View&): the view and camera this shader uses to shade.

  Returns:
  A new DeferredShader of DeferredShaderId with the given shader
  files paths.
  */
  DeferredShader(DeferredShaderId shader_id,
                 const std::string& path_geometry_pass_vertex_shader,
                 const std::string& path_geometry_pass_fragment_shader,
                 const std::string& path_light_pass_vertex_shader,
                 const std::string& path_light_pass_fragment_shader,
                 const world::World& world,
                 const state::View& view);
 
  // --------------------------------------------------------------------
  //  Attribute Getters
  // --------------------------------------------------------------------
  /*!
  Get the DeferredShaderId of this DeferredShader

  Returns:
       The ForwardShaderId of this ForwardShader.
  */
  DeferredShaderId getId() const { return this->id; }

  // --------------------------------------------------------------------------
  //  Render methods
  // --------------------------------------------------------------------------
  /*!
  Render all objects of this DeferredShader
  */
  virtual void render();

protected:
  // --------------------------------------------------------------------------
  //  glsl management
  // --------------------------------------------------------------------------
  /*!
  Load the ShaderFiles specified in with the paths during construction
  */
  virtual void loadShaders(const std::string& path_geometry_vert_shader,
                           const std::string& path_geometry_frag_shader,
                           const std::string& path_light_vert_shader,
                           const std::string& path_light_frag_shader);

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
  /*!
  Render the geometry (first) pass of this DeferredShader

  Render all the attributes needed for the light (second) pass
  of this DeferredShader into the GBuffer of this DeferredShader.
  */
  virtual void renderGeometryPass();

  virtual void renderGeometryPassObjects();

  /*!
  Render the light (second) pass of this DeferredShader

  Render a quad and use the GBuffer to obtain the attributes of the
  scene to render the frame.
  */
  virtual void renderLightPass();

  virtual void renderLightPassObjects();

  /*!
  Render the different GBuffers to frame.
  */
  virtual void renderBuffers();

  // --------------------------------------------------------------------------
  //  Attributes
  // --------------------------------------------------------------------------

  // client side attributes
  // --------------------------------------------------------------------------
  /*! DeferredShaderId of this DeferredShader */
  const DeferredShaderId id;

  /*! GBuffer of this DeferredShader */
  GBuffer gBuffer;

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
  /*! GLuint pointer to the geometry pass ShaderProgam */
  GLuint geometry_pass_sp;
  /*! GLuint pointer to the light pass ShaderProgram */
  GLuint light_pass_sp;

  /*!
  GLuint pointer to the Uniform Buffer Object holding the internal
  light data.
  */
  GLuint light_ubo;

  /*! PipelineObject of the fullscreen quad used in the Light Pass */
  PipelineObject* fullscreen_quad;
};

} // pipeline 
} // nTiled