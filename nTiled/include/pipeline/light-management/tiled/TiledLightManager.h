/*! @file TiledLightManager.h
 *  @brief TiledLightManager.h contains the definition of the 
 *         TiledLightManager class as well as the TiledLightmanagerBuilder
 *         class usedd to construct TiledLightManagers.
 */

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
/*! @brief The TiledLightManager is responsible for managing the LightGrid 
 *         and obtaining the LightData and projections in order to
 *         construct LightGrid frames.
 */
class TiledLightManager {
 public:
  /*! @brief Construct a new TiledLightManager
   *
   * Construct a new TiledLightManager with the given World and View,
   * tile dimensions and light projector
   *
   * @param world Reference to the world which this TiledLightManager 
   *              observes.
   * @param view Reference to the view which this TiledLightManager 
   *             observes.
   * @param tile_width Width of a tile of this TiledLightManager.
   * @param tile_height Height of a tile of this TiledLightManager.
   * @param projector Reference to the LightProjector this 
   *                  new TiledLightmanager uses to project lights on 
   *                  clipspace coordinates.
   */
  TiledLightManager(const world::World& world,
                    const state::View& view,
                    GLuint tile_width, GLuint tile_height,
                    const LightProjector& projector);

  // ------------------------------------------------------------------------
  /*! @brief Construct the light grid frame based on the current View and 
    *        World.
   */
  void constructGridFrame();

  /*! @brief LightGrid datastructure to which this lightmanager writes. */
  LightGrid light_grid;

protected:
  // -------------------------------------------------------------------------
  //  constructGridFrame() sub-functions
  // -------------------------------------------------------------------------
  /*! @brief Clear the current light_grid of this TiledLightManager
   */
  virtual void clearGrid();

  /*! @brief Build the light_grid of this TiledLightManager
   */
  virtual void buildGrid();

  /*! @brief Finalise the light_grid of this TiledLightManager
   */
  virtual void finaliseGrid();


  // -------------------------------------------------------------------------
  //  Member variables
  // -------------------------------------------------------------------------
  /*! @brief LightProjector to be used to calculate the affected lights */
  const LightProjector& projector;

  /*! @brief World Reference of this TiledLightManager */
  const world::World& world;
  /*! @brief View Reference of this TiledLightManager */
  const state::View& view;
};


/*! @brief The TiledLightManagerBuilder is responsible for constructing 
 *         TiledLightManagers. 
 */
class TiledLightManagerBuilder {
public:
  /*! @brief Construct a new TiledLightManagerBuilder
   */
  TiledLightManagerBuilder();

  /*! @brief Construct a new TiledLightManager with the given parameters and return 
   *         a pointer to it.
   *
   * @param world Reference to the world which the new TiledLightManager 
   *              observes.
   * @param view Reference to the view which the new TiledLightManager 
   *             observes.
   * @param tile_width Width of a tile of this TiledLightManager.
   * @param tile_height Height of a tile of the new TiledLightManager.
   * @param projector Reference to the LightProjector the
   *                  new TiledLightmanager uses to project lights on 
   *                  clipspace coordinates.
   *
   * @Return A pointer to the new TiledLightManager containing a new LightGrid, 
   *         observing the specified World and View.
   */
  virtual TiledLightManager* constructNewTiledLightManager(const world::World& world,
                                                           const state::View& view,
                                                           GLuint tile_width, GLuint tile_height,
                                                           const LightProjector& projector) const;
};


} // pipeline
} // nTiled
