#pragma once

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <string>

#include "state\StateView.h"
#include "state\StateTexture.h"

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "world\World.h"

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
        world::World* p_world,                     // world
        std::map<std::string, std::string> texture_file_map,
        pipeline::PipelineType pipeline_type);

  ~State();

  // Member attributes
  View view;
  world::World* p_world;
  TextureCatalog texture_catalog;
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
 Parse the geometry form a specified geometry.json file

 Parse the geometry from a specified geometry.json file, 
 these objects will be added to the referenced world.

 Args:
     path (const std::string&): Path to the geometry.json file
     world (nTiled::world::World&): world to which this geometry 
                                    should be added
 */
void parseGeometry(const std::string& path,
                   world::World& world,
                   std::map<std::string, std::string>& texture_map_file);
} // state
} // nTiled

