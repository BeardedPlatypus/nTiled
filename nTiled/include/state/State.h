#pragma once

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <string>

#include "state\StateView.h"
#include "state\StateShading.h"
#include "state\StateTexture.h"
#include "state\StateLog.h"

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "world\World.h"
#include "world\light-constructor\LightConstructor.h"

namespace nTiled {
namespace state {

/*!
 State holds all attributes of this nTiled renderer
 */
struct State {
 public:
  // Constructor
  State(camera::Camera camera,                  // view
        camera::CameraControl* camera_control,
        glm::uvec2 viewport,
        ViewOutput* output,
        world::World* p_world,                     // world
        std::map<std::string, std::string> texture_file_map,
        std::vector<pipeline::ForwardShaderId> forward_shader_ids,
        bool is_debug,
        bool is_logging_data,
        std::string log_output_path,
        unsigned int frame_start,
        unsigned int frame_end);
  State(camera::Camera camera,                  // view
        camera::CameraControl* camera_control,
        glm::uvec2 viewport,
        ViewOutput* output,
        world::World* p_world,                     // world
        std::map<std::string, std::string> texture_file_map,
        pipeline::DeferredShaderId deferred_shader_id,
        bool is_debug,
        bool is_logging_data,
        std::string log_output_path,
        unsigned int frame_start,
        unsigned int frame_end);
  ~State();

  // Member attributes
  View view;
  world::World* p_world;
  TextureCatalog texture_catalog;
  Shading shading;
  Log log;


};

/*!
 Construct a State from the specified scene.json file

 Args:
     path (const std::string&): Path to the scene.json file
 Returns:
     A state corresponding with the provided scene.json file
 */
State constructStateFromJson(const std::string& path);

// Parsing functions
/*!
Parse the lights from a specified lights.json file

Parse the lights from a specified lights.json file,
these will be added to the world with the provided
LightConstructor object

Args:
path (const std::string& ): Path to the lights.json file
constructor (nTiled::world::LightConstructor&):
Light constructor used to add the specified lights to
the world the LightConstructor belongs to
*/
void parseLights(const std::string& path,
                 world::LightConstructor& constructor);

/*!
 Parse the geometry from a specified geometry.json file

 Parse the geometry from a specified geometry.json file, 
 these objects will be added to the referenced world.

 Args:
     path (const std::string&): Path to the geometry.json file
     world (nTiled::world::World&): world to which this geometry 
                                    should be added
 */
void parseGeometry(const std::string& path,
                   world::World& world,
                   std::vector<pipeline::ForwardShaderId>& forward_shader_ids,
                   pipeline::DeferredShaderId& deferred_shader_id,
                   std::map<std::string, std::string>& texture_map_file);


/*!
 Parse the camera lookAt matrix per frame for the PathCamera 

 Args:
     path (const std::string&): Path to the camera_path.json file
 */
std::vector<glm::mat4> readCameraFrames(const std::string& path);
} // state
} // nTiled

