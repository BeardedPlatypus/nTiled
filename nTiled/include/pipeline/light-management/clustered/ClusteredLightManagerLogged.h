#pragma once

#include "pipeline\light-management\clustered\ClusteredLightManager.h"
#include "log\Logger.h"

namespace nTiled {
namespace pipeline {


class ClusteredLightManagerLogged : public ClusteredLightManager {
public:
  ClusteredLightManagerLogged(const state::View& view,
                              const world::World& world,
                              glm::uvec2 tile_size,
                              GLuint depth_texture,
                              logged::ExecutionTimeLogger logger);

protected:
  virtual void computeKeys() override;
  virtual void sortAndCompactKeys() override;
  virtual void clearClustering() override;
  virtual void buildClustering() override;
  virtual void finaliseClustering() override;

  logged::ExecutionTimeLogger& logger;
};


class ClusteredLightManagerLoggedBuilder : public ClusteredLightManagerBuilder {
public:
  ClusteredLightManagerLoggedBuilder(logged::ExecutionTimeLogger& logger);

  ClusteredLightManager* constructNewClusteredLightManager(
    const state::View& view, const world::World& world,
    glm::uvec2 tile_size, GLuint depth_texture) const override;

protected:
  logged::ExecutionTimeLogger& logger;

};

} // pipeline
} // nTiled