#include "main/DataController.h"


// ----------------------------------------------------------------------------
//  nTiled Headers
// ----------------------------------------------------------------------------
#include "state\State.h"
#include "world\light-constructor\PointLightConstructor.h"

// ----------------------------------------------------------------------------
//  System Libraries
// ----------------------------------------------------------------------------
// File handling
#include <fstream>
#include <string>
#include <vector>

// Json include
#include <rapidjson\document.h>

#include <iostream>


namespace nTiled {
namespace main {

DataController::DataController(const std::string& config_path) :
    config_path(config_path),
    clock(Clock()),
    logger(logged::ExecutionTimeLogger(this->clock, 1, 2)),
    has_init(false) {
}


DataController::~DataController() {
  if (this->has_init) {
    delete this->p_logged_manager;
    delete this->p_world;
  }
}


void DataController::init() {
  this->has_init = true;

  // obtain relevant data from config file
  std::ifstream ifs(this->config_path);
  std::string config_file((std::istreambuf_iterator<char>(ifs)),
                          (std::istreambuf_iterator<char>()));
  // parse json
  rapidjson::Document config;
  config.Parse(config_file.c_str());

  // Load hashed_config data
  pipeline::hashed::HashedConfig hashed_config = pipeline::hashed::HashedConfig();
  rapidjson::Value::ConstMemberIterator hashed_config_itr = config.FindMember("hashed_config");
  if (hashed_config_itr != config.MemberEnd()) {
    auto& hashed_config_json = hashed_config_itr->value;

    unsigned int hashed_seed = 22;
    rapidjson::Value::ConstMemberIterator hashed_seed_itr = hashed_config_json.FindMember("seed");
    if (hashed_seed_itr != hashed_config_json.MemberEnd()) {
      hashed_seed = hashed_config_json["seed"].GetUint();
    }

    hashed_config = pipeline::hashed::HashedConfig(hashed_config_json["node_size"].GetFloat(),
                                                   hashed_config_json["starting_depth"].GetUint(),
                                                   hashed_config_json["r_increase_ratio"].GetFloat(),
                                                   hashed_config_json["max_attempts"].GetUint(),
                                                   hashed_seed);
  } else {
    throw std::runtime_error(std::string("No hash config specified"));
  }

  // Load light data
  bool is_logging_data = false;
  unsigned int logged_start_frame = 0;
  unsigned int logged_end_frame = 0;
  std::string log_output_path = "";
  std::string log_memory_path = "";

  rapidjson::Value::ConstMemberIterator log_itr = config.FindMember("log");
  if (log_itr != config.MemberEnd()) {
    auto& log_json = log_itr->value;

    is_logging_data = log_json["is_logging"].GetBool();
    log_output_path = log_json["output_path"].GetString();
    log_memory_path = log_json["memory_path"].GetString();
  }

  this->execution_time_path = log_output_path;
  this->memory_data_path = log_memory_path;

  if (!is_logging_data) {
    throw std::runtime_error(std::string("Logging disabled in config"));
  }

  // Construct relevant data
  this->p_world = new world::World();
  
  // Load lights
  world::PointLightConstructor light_constructor =
    world::PointLightConstructor(*p_world);

  auto& lights_array_json = config["lights"];
  for (rapidjson::Value::ConstValueIterator itr = lights_array_json.Begin();
  itr != lights_array_json.End();
    ++itr) {
    std::string lights_path = (*itr)["path"].GetString();
    state::parseLights(lights_path, light_constructor);
  }

  this->p_logged_manager = 
    new pipeline::hashed::HashedLightManagerLogged(*this->p_world,
                                                   hashed_config,
                                                   logger);
}


void DataController::execute() {
  this->logger.activate();
  this->p_logged_manager->init();
  this->logger.deactivate();
}


void DataController::exportData() {
  this->logger.exportLog(this->execution_time_path);
  this->p_logged_manager->exportMemoryUsageData(this->memory_data_path);
}

}
}