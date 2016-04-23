#pragma once

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <glad\glad.h>

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "state\StateView.h"
#include "world\World.h"

namespace nTiled {
namespace pipeline {

class ClusteredLightManager {
public:
  ClusteredLightManager(const state::View& view,
                        const world::World& world);
  ~ClusteredLightManager();

private:
  /*! World Reference of this ClusteredLightManager */
  const world::World& world;
  /*! View Reference of this ClusteredLightManager */
  const state::View& view;
  
  // --------------------------------------------------------------------------
  //  Compute shaders
  // --------------------------------------------------------------------------
};

} // pipeline
} // nTiled
