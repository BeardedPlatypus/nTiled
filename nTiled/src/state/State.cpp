#include "state\State.h"

// ----------------------------------------------------------------------------
//  System Libraries
// ----------------------------------------------------------------------------
// File handling
#include <fstream>
#include <string>
#include <vector>

// Json include
#include <rapidjson\document.h>

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------

// TODO add graceful error handling
namespace nTiled {
namespace state {

State::State(camera::Camera camera,                  // view
             camera::CameraControl* camera_control,
             glm::uvec2 viewport,
             world::World* p_world,                     // world
             std::map<std::string, std::string> texture_file_map,
             pipeline::PipelineType pipeline_type) :
    view(View(camera,
              camera_control,
              viewport)),
    p_world(p_world),
    texture_catalog(TextureCatalog(texture_file_map)) { }

State::~State() {
  delete this->p_world;
}

State constructStateFromJson(const std::string& path) {
  // Parse scene.json
  // -------------------------------------------------------------------------
  // stream file
  std::ifstream ifs(path);
  std::string config_file((std::istreambuf_iterator<char>(ifs)),
                          (std::istreambuf_iterator<char>()));
  // parse json
  rapidjson::Document config;
  config.Parse(config_file.c_str());

  // Build Shading Component 
  // ------------------------------------------------------------------------
  // Pipeline type
  pipeline::PipelineType pipeline_type;
  std::string pipeline_type_str = config["pipeline"].GetString();
  if (pipeline_type_str.compare("DEFERRED") == 0) {
    pipeline_type = pipeline::PipelineType::Deferred;
  } else if (pipeline_type_str.compare("FORWARD") == 0) {
    pipeline_type = pipeline::PipelineType::Forward;
  } else if (pipeline_type_str.compare("FORWARD-DEBUG") == 0) {
    pipeline_type = pipeline::PipelineType::ForwardDebug;
  } else {
    pipeline_type = pipeline::PipelineType::Forward;
  }

  // Build View Component
  // ------------------------------------------------------------------------
  // camera control
  std::string camera_control_string = config["camera"]["control"].GetString();

  camera::CameraControl* camera_control = nullptr;
  if (camera_control_string.compare("TURNTABLE") == 0) {
    camera_control = new camera::TurnTableCameraControl();
  } else {
    camera_control = new camera::TurnTableCameraControl();
  }

  // viewport
  auto& viewport_json = config["viewport"];

  glm::uvec2 viewport = glm::uvec2(viewport_json["width"].GetUint(),
                                   viewport_json["height"].GetUint());

  // camera construction
  auto& eye_json = config["camera"]["eye"];
  glm::vec3 camera_eye = glm::vec3(eye_json["x"].GetFloat(),
                                   eye_json["y"].GetFloat(),
                                   eye_json["z"].GetFloat());

  auto& center_json = config["camera"]["center"];
  glm::vec3 camera_center = glm::vec3(center_json["x"].GetFloat(),
                                      center_json["y"].GetFloat(),
                                      center_json["z"].GetFloat());


  auto& up_json = config["camera"]["up"];
  glm::vec3 camera_up = glm::vec3(up_json["x"].GetFloat(),
                                  up_json["y"].GetFloat(),
                                  up_json["z"].GetFloat());

  float fovy = config["camera"]["fovy"].GetFloat();
  float aspect = config["camera"]["aspect"].GetFloat();

  float z_near = config["camera"]["clip"]["near"].GetFloat();
  float z_far = config["camera"]["clip"]["far"].GetFloat();

  camera::Camera camera = camera::Camera(camera_control,
                                         camera_eye,
                                         camera_center,
                                         camera_up,
                                         fovy,
                                         aspect,
                                         z_near,
                                         z_far);

  // Build World and Texture Component
  // ------------------------------------------------------------------------
  // construct world
  world::World* p_world = new world::World();
  std::map<std::string, std::string> texture_file_map =
    std::map<std::string, std::string>();

  // load geometry
  auto& geometry_array_json = config["geometry"];
  for (rapidjson::Value::ConstValueIterator itr = geometry_array_json.Begin();
       itr != geometry_array_json.End();
       ++itr) {

    std::string geometry_path = (*itr)["path"].GetString();
    parseGeometry(geometry_path, *p_world, texture_file_map);
  }

  return State(camera,
               camera_control,
               viewport,
               p_world,
               texture_file_map,
               pipeline_type);
}

} // state
} // pipeline




