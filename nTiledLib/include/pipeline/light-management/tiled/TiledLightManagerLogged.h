/*! @file TiledLightManagerLogged.h 
 *  @brief TiledLightManagerLogged.h contains the definition of 
 *         the logged version TiledLightManager.
 */
#pragma once

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "TiledLightManager.h"
#include "log\Logger.h"


namespace nTiled {
namespace pipeline {

/*! @brief TiledLightManagerLogged acts exactly like the TiledLightManager
 *         however it tracks the execution time of each defined function.
 */
class TiledLightManagerLogged : public TiledLightManager {
public:
  TiledLightManagerLogged(const world::World& world,
                          const state::View& view,
                          GLuint tile_width, GLuint tile_height,
                          const LightProjector& projector,
                          logged::ExecutionTimeLogger& logger);
protected:
  virtual void clearGrid() override;
  virtual void buildGrid() override;
  virtual void finaliseGrid() override;

  /*! @brief Reference to the ExecutionTimeLogger object which logs
   *         the execution time of this TiledLightManager.
   */
  logged::ExecutionTimeLogger& logger;
};


/*! @brief TiledLightManagerLoggedBuilder constructs TiledLightManagerLogged
 *         instead of TiledLightManager, in every other regard it acts the 
 *         the same as TiledLightManagerBuilder
 */
class TiledLightManagerLoggedBuilder : public TiledLightManagerBuilder {
public:
  /*! @brief Construct a new TiledLightmanagerLoggedBuilder with the 
   *         the specified logger object.
   *
   * @param logger Reference to the ExecutionTimeLogger used in all 
   *               TiledLightManagerLogged created by this
   *               TiledLightmanagerLoggedBuilder.
   */
  TiledLightManagerLoggedBuilder(logged::ExecutionTimeLogger& logger);

  virtual TiledLightManager* constructNewTiledLightManager(
    const world::World& world,
    const state::View& view,
    GLuint tile_width, GLuint tile_height,
    const LightProjector& projector) const override;

protected:
  /*! @brief Reference to the ExecutionTimeLogger object which logs
   *         the execution time of the constructed TiledLightManagers.
   */
  logged::ExecutionTimeLogger& logger;
};

} // pipeline
} // nTiled
