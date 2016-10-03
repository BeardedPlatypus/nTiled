/*! @file Shader.h
 *  @brief Shader.h contains the definition of the Shader class.
 */
#pragma once

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <glad\glad.h>


// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "world\World.h"
#include "pipeline\PipelineObject.h"
#include "pipeline\PipelineLight.h"
#include "state\StateView.h"

namespace nTiled {
namespace pipeline {

class Shader {
public:
  /*! @brief Construct a new Shader with a reference to the World and View
   *         of this nTiled execution
   *
   * @param world A reference to the World of this nTiled execution
   * @param view A reference to the View of this nTiled execution
   */
  Shader(world::World& world,
         state::View& view);

  /*! @brief Default destructor*/
  ~Shader();

  /*! @brief Initialise this Shader such that subsequently the render
   *         function can be called.
   */
  void init();

  /*! @brief Render all objects specified to be rendered with this Shader
   *         with all lights that effect this Shader.
   */
  void render();

private:
  /*! @brief Reference to the World of this nTiled execution. */
  world::World& world;

  /*! @brief Reference to the View of this nTiled execution. */
  state::View& view;

  /*! @brief A vector containing pointers to each PipelineObject rendered
   *         by this Shader.
   */
  std::vector<PipelineObject*> ps_obj;

  /*! @brief A vector containing all the PipelineLights used in rendering
   *         by this Shader.
   */
  std::vector<PipelineLight> lights;

  /*! @brief openGL pointer to the shader in video memory. */
  GLuint shader;
  /*! @brief openGL pointer to the light ubo in video memory. */
  GLuint light_ubo;
};


} // pipeline
} // nTiled
