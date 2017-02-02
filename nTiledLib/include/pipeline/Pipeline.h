/*! @file Pipeline.h
 *  @brief Pipeline.h contains the definition of the base class for all 
 *         pipelines in nTiled. 
 *         Any Pipeline which is to be used in nTiled needs to inherit from 
 *         this class.
 */
#pragma once


// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "state\State.h"


namespace nTiled {
namespace pipeline {

/*! @brief Pipeline is the base class of all Pipeline objects in nTiled.
 *         It contains the interface which nTiled uses to render with a 
 *         specific Pipeline.
 */
class Pipeline {
public:
  // --------------------------------------------------------------------------
  //  Constructor
  // --------------------------------------------------------------------------
  /*! @brief Construct a new Pipeline with the given state
   * 
   * @param state Reference to the state this Pipeline renders.
   */
  Pipeline(state::State& state);

  /*! @brief Destruct this Pipeline
   */
  virtual ~Pipeline() {}

  /*! @brief Initialise the shaders of this Pipeline.
   */
  virtual void initialiseShaders();

  // --------------------------------------------------------------------------
  //  Methods
  // --------------------------------------------------------------------------
  /*! @brief Render the state of this Pipeline into the framebuffer
   */
  virtual void render();

  /*! @brief Set the output buffer of this Pipeline to p_fbo
   *
   * @param p_fbo openGL Pointer to the new output buffer of this Pipeline.
   */
  virtual void setOutputBuffer(GLint p_fbo) { this->output_buffer = p_fbo; }

protected:
  /*! @brief Reference to the State of this Pipeline. */
  state::State& state;
  /*! @brief openGL Pointer to the output buffer of this Pipeline. */
  GLint output_buffer = 0;
};

} // pipeline
} // nTiled