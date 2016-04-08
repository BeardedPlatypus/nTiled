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
   * Construct a new Pipeline with the given state
   * 
   * Param:
   *     state (State&): reference to the state this Pipeline renders
   */
  Pipeline(state::State& state);

  /*!
   * Destruct this Pipeline
   */
  virtual ~Pipeline() {}

  // --------------------------------------------------------------------------
  //  Methods
  // --------------------------------------------------------------------------
  /*!
   * Render the state of this Pipeline into the framebuffer
   */
  virtual void render() = 0;

protected:
  state::State& state;
};


} // pipeline
} // nTiled