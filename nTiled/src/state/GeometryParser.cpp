#include "state\State.h"

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
// File handling
#include <fstream>
#include <string>
#include <map>

// Json include
#include <rapidjson\document.h>

// glm
#include <glm\gtc\matrix_transform.hpp>

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "world\object-constructor\AssImpConstructor.h"

namespace nTiled {

void state::parseGeometry(const std::string& path,
                          world::World& world,
                          std::map<std::string, std::string>& texture_file_map) {
  // Read file to string
  // --------------------------------------------------------------------------
  std::ifstream ifs(path);
  std::string geometry_json((std::istreambuf_iterator<char>(ifs)),
                            (std::istreambuf_iterator<char>()));

  // Parse json
  // --------------------------------------------------------------------------
  rapidjson::Document geometry;
  geometry.Parse(geometry_json.c_str());

  // Parse meshes
  // --------------------------------------------------------------------------
  std::map<std::string, std::string> mesh_map = std::map<std::string, std::string>();
  auto& meshes_json = geometry["meshes"];

  for (rapidjson::Value::ConstValueIterator itr = meshes_json.Begin();
       itr != meshes_json.End();
       ++itr) {
    std::string mesh_id = (*itr)["id"].GetString();
    std::string mesh_path = (*itr)["path"].GetString();

    mesh_map.insert(std::pair<std::string, std::string>(mesh_id, mesh_path));
  }

  // Parse textures
  // --------------------------------------------------------------------------
  rapidjson::Value::ConstMemberIterator itr = geometry.FindMember("textures");
  if (itr != geometry.MemberEnd()) {
    auto& textures_json = itr->value;

    for (rapidjson::Value::ConstValueIterator itr = textures_json.Begin();
         itr != textures_json.End();
         ++itr) {
      std::string texture_id = (*itr)["id"].GetString();
      std::string texture_path = (*itr)["path"].GetString();

      texture_file_map.insert(std::pair<std::string, std::string>(texture_id,
                                                                  texture_path));
    }
  }

  // --------------------------------------------------------------------------
  std::map<std::string, pipeline::ShaderKey> strToId =
  {
    { "forward_attenuated",
      pipeline::ShaderKey(pipeline::ForwardShaderId::ForwardAttenuated) },
    { "forward_textured_attenuated",
      pipeline::ShaderKey(pipeline::ForwardShaderId::ForwardTexturedAttenuated) },
    { "forward_tiled",
      pipeline::ShaderKey(pipeline::ForwardShaderId::ForwardTiled) },
    { "forward_textured_tiled",
      pipeline::ShaderKey(pipeline::ForwardShaderId::ForwardTexturedTiled) },
    { "deferred_basic",
      pipeline::ShaderKey(pipeline::DeferredShaderId::DeferredBasic) },
    { "deferred_attenuated",
      pipeline::ShaderKey(pipeline::DeferredShaderId::DeferredAttenuated) },
    { "deferred_tiled",
      pipeline::ShaderKey(pipeline::DeferredShaderId::DeferredTiled) },
  };

  // Object constructor catalog
  std::map<std::string, world::AssImpConstructor*> obj_constructor_catalog =
    std::map<std::string, world::AssImpConstructor*>();

  //  Build Objects
  // --------------------------------------------------------------------------
  auto& objects_json = geometry["objects"];
  for (rapidjson::Value::ConstValueIterator itr = objects_json.Begin();
       itr != objects_json.End();
       ++itr) {

    // get object constructor
    // ------------------------------------------------------------------------
    world::AssImpConstructor* obj_constructor_p;

    // check if mesh constructor already exists
    std::string mesh_id = (*itr)["mesh_id"].GetString();
    std::map<std::string, world::AssImpConstructor*>::iterator it =
      obj_constructor_catalog.find(mesh_id);

    if (it != obj_constructor_catalog.end()) {
      obj_constructor_p = it->second;
    } else {
      obj_constructor_p = new world::AssImpConstructor(mesh_map[mesh_id],
                                                       world);
      obj_constructor_catalog.insert(std::pair<std::string, 
                                     world::AssImpConstructor*>(
                                       mesh_id, 
                                       obj_constructor_p));
    }

    // get attributes of the object
    // name
    std::string name = (*itr)["name"].GetString();
    pipeline::ShaderKey shader_key = strToId[(*itr)["shader_id"].GetString()];

    // translation
    // ------------------------------------------------------------------------
    auto& translation_json = (*itr)["translation"];
    glm::vec3 translation = glm::vec3(translation_json["x"].GetFloat(),
                                      translation_json["y"].GetFloat(),
                                      translation_json["z"].GetFloat());

    glm::mat4 transformation_matrix = glm::translate(glm::mat4(1.0), 
                                                     translation);

    // textures 
    std::map<std::string, std::string> texture_map_object =
      std::map < std::string, std::string>();

    rapidjson::Value::ConstMemberIterator textures_map_itr =
      (*itr).FindMember("textures");

    if (textures_map_itr != (*itr).MemberEnd()) {
      // there exists textures which need to be loaded.
      auto& texture_map_json = textures_map_itr->value;


      for (rapidjson::Value::ConstValueIterator itr = texture_map_json.Begin();
           itr != texture_map_json.End();
           ++itr) {
        std::string tex_key = (*itr)["key"].GetString();
        std::string tex_id = (*itr)["id"].GetString();

        texture_map_object.insert(std::pair<std::string, std::string>(tex_key,
                                                                      tex_id));
      }
    }

    obj_constructor_p->add(name,
                           transformation_matrix,
                           shader_key,
                           texture_map_object,
                           true);
  }
       
  // Remove object constructors
  for (auto const constructor_pair : obj_constructor_catalog) {
    delete constructor_pair.second;
  }
}

} // nTiled