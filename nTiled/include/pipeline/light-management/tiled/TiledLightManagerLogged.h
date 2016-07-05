#pragma once

#include "TiledLightManager.h"
#include "log\Logger.h"

namespace nTiled {
namespace pipeline {


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

  logged::ExecutionTimeLogger& logger;
};


class TiledLightManagerLoggedBuilder : public TiledLightManagerBuilder {
public:
  TiledLightManagerLoggedBuilder(logged::ExecutionTimeLogger& logger);

  virtual TiledLightManager* constructNewTiledLightManager(
    const world::World& world,
    const state::View& view,
    GLuint tile_width, GLuint tile_height,
    const LightProjector& projector) const override;

protected:
  logged::ExecutionTimeLogger& logger;
};

} // pipeline
} // nTiled
