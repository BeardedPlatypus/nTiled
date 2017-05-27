#pragma once

// ----------------------------------------------------------------------------
//  nTiled Headers
// ----------------------------------------------------------------------------
#include "log\Logger.h"
#include "pipeline\light-management\hashed\HashedLightManagerLogged.h"

namespace nTiled {
namespace main {

class DataController {
public:
  DataController(const std::string& config_path);
  ~DataController();

  void init();
  void execute();
  void exportData();

private:
  bool has_init;

  const std::string config_path;
  std::string execution_time_path;
  std::string memory_data_path;

  Clock clock;

  logged::ExecutionTimeLogger logger;
  pipeline::hashed::HashedLightManagerLogged* p_logged_manager;
  world::World* p_world;
};

}
}
