/*! @file State.h
 *  @brief State.h contains the definition of the State struct as well as 
 *         the methods to construct the State and parse lights and geometry.
 */
#pragma once

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <string>

// ----------------------------------------------------------------------------
//  State headers
// ----------------------------------------------------------------------------
#include "state\StateView.h"
#include "state\StateShading.h"
#include "state\StateTexture.h"
#include "state\StateLog.h"

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "world\World.h"
#include "world\light-constructor\LightConstructor.h"
#include "pipeline\light-management\hashed\HashedConfig.h"

namespace nTiled {
namespace state {

/*! @brief State holds all attributes of relating to the state of this nTiled 
 *         renderer
 */
struct State {
public:
  // --------------------------------------------------------------------------
  // Constructor || Destructor
  // --------------------------------------------------------------------------
  /*! @brief Construct a new State with a ForwardPipeline given the provided 
   *         parameters.
   * 
   * @param camera The camera of this new State.
   * @param camera_control Pointer to the CameraControl to be used in this new
   *                       new State
   * @param view_port The dimensions of the viewport of this new State.
   * @param output The ViewOutput state of this new State
   * @param p_world A pointer to the World of this new State
   * @param texture_file_map A map containing a mapping of texture names 
   *                         to texture files.
   * @param forward_shader_ids The set of ForwardShaderId used in this run of
   *                           nTiled.
   * @param tile_size The tile size in pixels to be used in Tiled and Clustered
   *                  shading.
   * @param is_debug Whether this run is in DebugMode
   * @param is_logging_data Whether this run is logging data.
   * @param frame_start The frame at which logging should be started (ignored if
   *                    !is_logging_data)
   * @param frame_end The frame at which logging should be ended (ignored if 
   *                  !is_logging_data)
   * @param exit_after_done Whether the application should stop after logging
   *                        the data.
   */
  State(camera::Camera camera,                  // view
        camera::CameraControl* camera_control,
        glm::uvec2 viewport,
        ViewOutput* output,
        world::World* p_world,                     // world
        std::map<std::string, std::string> texture_file_map,
        std::vector<pipeline::ForwardShaderId> forward_shader_ids,
        glm::uvec2 tile_size,
        bool is_debug,
        bool is_logging_data,
        bool is_counting_calculations,
        std::string log_output_path,
        std::string log_output_path_calculations,
        unsigned int frame_start,
        unsigned int frame_end,
        bool exit_after_done,
        unsigned int exit_frame,
        bool display_light_calculations,
        pipeline::hashed::HashedConfig hashed_config);

  /*! @brief Construct a new State with a DeferredPipeline given the provided 
   *         parameters.
   * 
   * @param camera The camera of this new State.
   * @param camera_control Pointer to the CameraControl to be used in this new
   *                       new State
   * @param view_port The dimensions of the viewport of this new State.
   * @param output The ViewOutput state of this new State
   * @param p_world A pointer to the World of this new State
   * @param texture_file_map A map containing a mapping of texture names 
   *                         to texture files.
   * @param deferred_shader_id The DeferredShaderId used in this run of
   *                           nTiled.
   * @param tile_size The tile size in pixels to be used in Tiled and Clustered
   *                  shading.
   * @param is_debug Whether this run is in DebugMode
   * @param is_logging_data Whether this run is logging data.
   * @param frame_start The frame at which logging should be started (ignored if
   *                    !is_logging_data)
   * @param frame_end The frame at which logging should be ended (ignored if 
   *                  !is_logging_data)
   * @param exit_after_done Whether the application should stop after logging
   *                        the data.
   */
  State(camera::Camera camera,                  // view
        camera::CameraControl* camera_control,
        glm::uvec2 viewport,
        ViewOutput* output,
        world::World* p_world,                     // world
        std::map<std::string, std::string> texture_file_map,
        pipeline::DeferredShaderId deferred_shader_id,
        glm::uvec2 tile_size,
        bool is_debug,
        bool is_logging_data,
        bool is_counting_calculations,
        std::string log_output_path,
        std::string log_output_path_calculations,
        unsigned int frame_start,
        unsigned int frame_end,
        bool exit_after_done,
        unsigned int exit_frame,
        bool display_light_calculations,
        pipeline::hashed::HashedConfig hashed_config);

  /*! @brief virtual destructor */
  ~State();

  // --------------------------------------------------------------------------
  // Member attributes
  // --------------------------------------------------------------------------
  /*! @brief The View state of this State. */
  View view;
  /*! @brief A pointer to the World of this State. */
  world::World* p_world;
  /*! @brief The TextureCatalog of this State. */
  TextureCatalog texture_catalog;
  /*! @brief The Shading state of this State. */
  Shading shading;
  /*! @brief The Log state of this State */
  Log log;
};

/*! @brief Construct a State from the specified scene.json file
 *
 * @param path Path to the scene.json file
 * @Return A state corresponding with the provided scene.json file
 */
State* constructStateFromJson(const std::string& path);

// Parsing functions
/*! @brief Parse the lights from a specified lights.json file
 *
 * Parse the lights from a specified lights.json file,
 * these will be added to the world with the provided
 * LightConstructor object

 * @param path Path to the lights.json file
 * @param constructor LightConstructor used to add the specified lights to
 *                    the world the LightConstructor belongs to
*/
void parseLights(const std::string& path,
                 world::LightConstructor& constructor);

/*! @brief Parse the geometry from a specified geometry.json file
 * 
 * Parse the geometry from a specified geometry.json file, 
 * these objects will be added to the referenced world.
 * 
 * @param path Path to the geometry.json file
 * @param world world to which this geometry should be added
 * @param forward_shader_ids reference to the all forward_shader_ids
 * @param deferred_shader_id reference to the deferred_shader_id
 * @param texture_map_file reference to the map of texture files
 */
void parseGeometry(const std::string& path,
                   world::World& world,
                   std::vector<pipeline::ForwardShaderId>& forward_shader_ids,
                   pipeline::DeferredShaderId& deferred_shader_id,
                   std::map<std::string, std::string>& texture_map_file);


/*! @brief Parse the camera lookAt matrix per frame for the PathCamera 
 *
 * @param path  Path to the camera_path.json file
 */
std::vector<glm::mat4> readCameraFrames(const std::string& path);

} // state
} // nTiled

