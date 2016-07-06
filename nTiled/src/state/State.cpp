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

#include <iostream>
// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "world\light-constructor\PointLightConstructor.h"
#include <glm/gtc/matrix_transform.hpp>

// TODO add graceful error handling
namespace nTiled {
namespace state {

State::State(camera::Camera camera,                  // view
             camera::CameraControl* camera_control,
             glm::uvec2 viewport,
             ViewOutput* output,
             world::World* p_world,                     // world
             std::map<std::string, std::string> texture_file_map,
             std::vector<pipeline::ForwardShaderId> forward_shader_ids,
             bool is_debug,
             bool is_logging_data,
             std::string log_output_path) :
    view(View(camera,
              camera_control,
              viewport,
              output)),
    p_world(p_world),
    texture_catalog(TextureCatalog(texture_file_map)),
    shading(Shading(forward_shader_ids, 
                    is_debug)),
    log(is_logging_data,
        log_output_path) { }

State::State(camera::Camera camera,                  // view
             camera::CameraControl* camera_control,
             glm::uvec2 viewport,
             ViewOutput* output,
             world::World* p_world,                     // world
             std::map<std::string, std::string> texture_file_map,
             pipeline::DeferredShaderId deferred_shader_id,
             bool is_debug,
             bool is_logging_data,
             std::string log_output_path) :
    view(View(camera,
              camera_control,
              viewport,
              output)),
    p_world(p_world),
    texture_catalog(TextureCatalog(texture_file_map)),
    shading(Shading(deferred_shader_id, 
                    is_debug)),
    log(is_logging_data,
        log_output_path) { }

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
  } else {
    throw std::runtime_error(std::string("Unspecified pipeline type: ") + pipeline_type_str);
  }

  // is debug
  rapidjson::Value::ConstMemberIterator itr = config.FindMember("is_debug");
  bool is_debug = ((itr != config.MemberEnd()) && itr->value.GetBool());

  // is logging data
  bool is_logging_data = false;
  std::string log_output_path = "";

  rapidjson::Value::ConstMemberIterator log_itr = config.FindMember("log");
  if (log_itr != config.MemberEnd()) {
    auto& log_json = log_itr->value;

    is_logging_data = log_json["is_logging"].GetBool();
    log_output_path = log_json["output_path"].GetString();
  }

  // Build View Component
  // ------------------------------------------------------------------------
  // camera control
  std::string camera_control_string = config["camera"]["control"].GetString();

  camera::CameraControl* camera_control = nullptr;
  if (camera_control_string.compare("TURNTABLE") == 0) {
    camera_control = new camera::TurnTableCameraControl();
  } else if (camera_control_string.compare("PATH") == 0) {
    // Read the camera path json
    std::cout << "PATH" << std::endl;
    auto& itr = config["camera"].FindMember("frames_path");
    if (itr != config["camera"].MemberEnd()) {
      std::string frames_path = itr->value.GetString();
      std::vector<glm::mat4> frames = readCameraFrames(frames_path);

      camera_control = new camera::PathCameraControl(frames);
    } else {
      throw std::runtime_error(std::string("No path specified"));
    }
  } else {
    std::cout << "Default" << std::endl;
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

  camera::CameraConstructionData construction_data = 
    camera::CameraConstructionData(camera_eye,
                                   camera_center,
                                   camera_up,
                                   fovy,
                                   aspect,
                                   z_near,
                                   z_far);
  camera::Camera camera = camera::Camera(camera_control,
                                         construction_data);

  // output
  ViewOutput* output;

  rapidjson::Value::ConstMemberIterator output_itr = 
    config["camera"].FindMember("output");
  if (output_itr != config["camera"].MemberEnd()) {
    auto& output_json = output_itr->value;
    std::string output_type_str = output_json["type"].GetString();

    if (output_type_str.compare("DISPLAY") == 0) {
      output = new ViewOutput();
    } else if (output_type_str.compare("MEMORY") == 0) {
      unsigned int frame_start = output_json["frame_start"].GetUint();
      unsigned int frame_end = output_json["frame_end"].GetUint();
      std::string base_path = output_json["image_base_path"].GetString();
      
      output = new ViewOutput(base_path,
                              frame_start,
                              frame_end);
    } else {
      output = new ViewOutput();
    }
  } else {
    output = new ViewOutput();
  }


  // Build World and Texture Component
  // ------------------------------------------------------------------------
  // construct world
  world::World* p_world = new world::World();

  std::map<std::string, std::string> texture_file_map =
    std::map<std::string, std::string>();

  std::vector<pipeline::ForwardShaderId> forward_shader_ids;
  pipeline::DeferredShaderId deferred_shader_id;

  // load geometry
  auto& geometry_array_json = config["geometry"];
  for (rapidjson::Value::ConstValueIterator itr = geometry_array_json.Begin();
       itr != geometry_array_json.End();
       ++itr) {

    std::string geometry_path = (*itr)["path"].GetString();
    parseGeometry(geometry_path,
                  *p_world,
                  forward_shader_ids,
                  deferred_shader_id,
                  texture_file_map);
  }

  // Load lights
  world::PointLightConstructor light_constructor =
    world::PointLightConstructor(*p_world);

  auto& lights_array_json = config["lights"];
  for (rapidjson::Value::ConstValueIterator itr = lights_array_json.Begin();
  itr != lights_array_json.End();
    ++itr) {
    std::string lights_path = (*itr)["path"].GetString();
    parseLights(lights_path, light_constructor);
  }

  if (pipeline_type == pipeline::PipelineType::Forward) {
    return State(camera,
                 camera_control,
                 viewport,
                 output,
                 p_world,
                 texture_file_map,
                 forward_shader_ids,
                 is_debug,
                 is_logging_data,
                 log_output_path);
  } else {
    return State(camera,
                 camera_control,
                 viewport,
                 output,
                 p_world,
                 texture_file_map,
                 deferred_shader_id,
                 is_debug,
                 is_logging_data,
                 log_output_path);
  }
}


std::vector<glm::mat4> readCameraFrames(const std::string& path) {
  // Parse scene.json
  // -------------------------------------------------------------------------
  // stream file
  std::ifstream ifs(path);
  std::string config_file((std::istreambuf_iterator<char>(ifs)),
                          (std::istreambuf_iterator<char>()));
  // parse json
  rapidjson::Document config;
  config.Parse(config_file.c_str());

  // Parse frames
  // --------------------------------------------------------------------------
  std::vector<glm::mat4> frames = std::vector<glm::mat4>();

  auto& itr = config.FindMember("frames");

  if (itr != config.MemberEnd()) {
    auto& frames_json = itr->value;
    for (rapidjson::Value::ConstValueIterator itr = frames_json.Begin();
         itr != frames_json.End();
         ++itr) {
      auto& up_json     = (*itr)["up"];
      auto& eye_json    = (*itr)["eye"];
      auto& center_json = (*itr)["center"];

      glm::vec3 eye = glm::vec3(eye_json["x"].GetFloat(),
                                eye_json["y"].GetFloat(),
                                eye_json["z"].GetFloat());

      glm::vec3 center = glm::vec3(center_json["x"].GetFloat(),
                                   center_json["y"].GetFloat(),
                                   center_json["z"].GetFloat());

      glm::vec3 up = glm::vec3(up_json["x"].GetFloat(),
                               up_json["y"].GetFloat(),
                               up_json["z"].GetFloat());

      frames.push_back(glm::lookAt(eye, center, up));
    }

    return frames;
  } else {
    throw std::runtime_error(std::string("No frames found in specified path"));
  }
}

} // state
} // pipeline




