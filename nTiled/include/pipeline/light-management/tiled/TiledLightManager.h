#pragma once

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "pipeline\light-management\Tiled\LightGrid.h"
#include "pipeline\light-management\Tiled\LightProjector.h"

#include "state\StateView.h"
#include "world\World.h"

namespace nTiled {
namespace pipeline {

// ------------------------------------------------------------------------
//  Light Manager class
// ------------------------------------------------------------------------
/*!
 The TiledLightManager is responsible for managing the LightGrid and
 obtaining the LightData and projections in order to construct LightGrid
 frames.
 */
class TiledLightManager {
 public:
  /*!
   Construct a new TiledLightManager

   Construct a new TiledLightManager with the given World and View,
   tile dimensions and light projector

   Args:
    world (nTiled_world::World&): Reference to the world which this
                    TiledLightManager observes.
    view (nTiled_state::View&): Reference to the view which this
                                TiledLightManager observes.
    tile_width (GLuint): width of a tile of this TiledLightManager.
    tile_height (GLuint): height of a tile of this TiledLightManager.
    projector (LightProjector&): Reference to the LightProjector this
                                 TiledLightmanager uses to project
                   lights on clipspace coordinates.

  Returns:
    A new TiledLightManager containing a new LightGrid, observing the
    specified World and View.
   */
  TiledLightManager(const world::World& world,
                    const state::View& view,
                    GLuint tile_width, GLuint tile_height,
                    const LightProjector& projector);

  /*!
   Construct the light grid frame based on the current View and World
   */
  void constructGridFrame();

  /*! LightGrid datastructure to which this lightmanager writes. */
  LightGrid light_grid;

protected:
  // -------------------------------------------------------------------------
  //  constructGridFrame() sub-functions
  // -------------------------------------------------------------------------
  /*! 
   Clear the current light_grid of this TiledLightManager
   */
  virtual void clearGrid();

  /*!
   Build the light_grid of this TiledLightManager
   */
  virtual void buildGrid();

  /*!
   Finalise the light_grid of this TiledLightManager
   */
  virtual void finaliseGrid();


  // -------------------------------------------------------------------------
  //  Member variables
  // -------------------------------------------------------------------------
  /*! LightProjector to be used to calculate the affected lights */
  const LightProjector& projector;

  /*! World Reference of this TiledLightManager */
  const world::World& world;
  /*! View Reference of this TiledLightManager */
  const state::View& view;

};


/*!
 The TiledLightManagerBuilder is responsible for constructing 
 TiledLightManagers. 
 */
class TiledLightManagerBuilder {
public:
  /*! 
   Construct a new TiledLightManagerBuilder
   */
  TiledLightManagerBuilder();

  /*! 
   Construct a new TiledLightManager with the given parameters and return 
   a pointer to it.
   */
  virtual TiledLightManager* constructNewTiledLightManager(const world::World& world,
                                                           const state::View& view,
                                                           GLuint tile_width, GLuint tile_height,
                                                           const LightProjector& projector) const;
};


} // pipeline
} // nTiled
