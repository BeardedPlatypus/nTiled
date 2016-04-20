#pragma once

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "state\State.h"

namespace nTiled {
namespace pipeline {

class Pipeline {
public:
  // --------------------------------------------------------------------------
  //  Constructor
  // --------------------------------------------------------------------------
  /*!
   Construct a new Pipeline with the given state
   
   Param:
       state (State&): reference to the state this Pipeline renders
   */
  Pipeline(state::State& state);
  /*!
   Destruct this Pipeline
   */
  virtual ~Pipeline() {}

  // --------------------------------------------------------------------------
  //  Methods
  // --------------------------------------------------------------------------
  /*!
   Render the state of this Pipeline into the framebuffer
   */
  virtual void render();

  /*!
   Set the output buffer of this Pipeline to p_fbo

   Param:
       p_fbo (GLint): the new output buffer
   */
  virtual void setOutputBuffer(GLint p_fbo) { this->output_buffer = p_fbo; }

protected:
  state::State& state;
  GLint output_buffer = 0;
};


} // pipeline
} // nTiled