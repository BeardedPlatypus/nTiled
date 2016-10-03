/*! @file ClusteredLightManagerLogged.h 
 *  @brief ClusteredLightManagerLogged.h contains the definition of 
 *         the logged version of ClusteredLightManager.
 */
#pragma once

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "pipeline\light-management\clustered\ClusteredLightManager.h"
#include "log\Logger.h"


namespace nTiled {
namespace pipeline {

/*! @brief ClusteredLightManagerLogged acts exactly like the 
 *         ClusteredLightManager however it tracks the execution time 
 *         of each defined function.
 */
class ClusteredLightManagerLogged : public ClusteredLightManager {
public:
  ClusteredLightManagerLogged(const state::View& view,
                              const world::World& world,
                              glm::uvec2 tile_size,
                              GLuint depth_texture,
                              logged::ExecutionTimeLogger& logger);

protected:
  virtual void computeKeys() override;
  virtual void sortAndCompactKeys() override;
  virtual void clearClustering() override;
  virtual void buildClustering() override;
  virtual void finaliseClustering() override;

  /*! @brief Reference to the ExecutionTimeLogger object which logs
   *         the execution time of this TiledLightManager.
   */
  logged::ExecutionTimeLogger& logger;
};


/*! @brief ClusteredLightManagerLoggedBuilder constructs 
 *         ClusteredLightManagerLogged instead of TiledLightManager, in every 
 *         other regard it acts the the same as TiledLightManagerBuilder
 */
class ClusteredLightManagerLoggedBuilder : public ClusteredLightManagerBuilder {
public:
  /*! @brief Construct a new ClusteredLightmanagerLoggedBuilder with the 
   *         the specified logger object.
   *
   * @param logger Reference to the ExecutionTimeLogger used in all 
   *               ClusteredLightManagerLogged created by this
   *               ClusteredLightmanagerLoggedBuilder.
   */
  ClusteredLightManagerLoggedBuilder(logged::ExecutionTimeLogger& logger);

  ClusteredLightManager* constructNewClusteredLightManager(
    const state::View& view, const world::World& world,
    glm::uvec2 tile_size, GLuint depth_texture) const override;

protected:
  /*! @brief Reference to the ExecutionTimeLogger object which logs
   *         the execution time of the constructed ClusteredLightManagers.
   */
  logged::ExecutionTimeLogger& logger;

};

} // pipeline
} // nTiled