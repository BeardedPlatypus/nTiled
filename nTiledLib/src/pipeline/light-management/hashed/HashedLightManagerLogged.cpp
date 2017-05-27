#include "pipeline\light-management\hashed\HashedLightManagerLogged.h"

// ----------------------------------------------------------------------------
//  System Libraries
// ----------------------------------------------------------------------------
// File handling
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

// Json include
#include <rapidjson\document.h>
#include <rapidjson\writer.h>
#include <rapidjson\stringbuffer.h>


namespace nTiled {
namespace pipeline {
namespace hashed {

// ----------------------------------------------------------------------------
//  Constructor
// ----------------------------------------------------------------------------
HashedLightManagerLogged::HashedLightManagerLogged(const world::World& world,
                                                   HashedConfig hashed_config,
                                                   logged::ExecutionTimeLogger& logger) :
  HashedLightManager(world, hashed_config),
  logger(logger) {
}


// ----------------------------------------------------------------------------
//  Time tracked functions
// ----------------------------------------------------------------------------
void HashedLightManagerLogged::constructEmptyLightOctree() {
  this->logger.startLog(std::string("HashedLightManager::constructEmptyLightOctree"));
  HashedLightManager::constructEmptyLightOctree();
  this->logger.endLog();
}


void HashedLightManagerLogged::constructSLTs() {
  this->logger.startLog(std::string("HashedLightManager::constructSLTs"));
  HashedLightManager::constructSLTs();
  this->logger.endLog();
}


void HashedLightManagerLogged::addConstructedSLTs() {
  this->logger.startLog(std::string("HashedLightManager::addConstructedSLTs"));
  HashedLightManager::addConstructedSLTs();
  this->logger.endLog();
}


void HashedLightManagerLogged::constructLinklessOctree() {
  this->logger.startLog(std::string("HashedLightManager::constructLinklessOctree"));
  HashedLightManager::constructLinklessOctree();
  this->logger.endLog();
}


void HashedLightManagerLogged::exportMemoryUsageData(const std::string& path) {
  /* JSON layout:
     { "light_indices" : { "length": i }
     , "light_octree" : { "origin": { "x": x,
                                    , "y": y
                                    , "z": z 
                                    }
                        , "depth": depth
                        }
     , "linkless_octree" : { "depth" : depth
                           , "origin" : { "x" : x
                                        , "y" : y
                                        , "z" : z
                                        }
                           , "n_levels" : n_levels
                           , "n_octree_tables" : octree_tables
                           , "n_hash_tables" : hash_tables
                           , "tables" : [ { "octree_table" : { "m" : m
                                                             , "r" : r
                                                             }
                                          , "data_table" : { "exists": exists
                                                           , "m" : m
                                                           , "r" : r
                                                           }
                                          }
                                        ]
     }
   */
  pipeline::hashed::LinklessOctree* p_linkless = this->getLinklessOctree();
  pipeline::hashed::LightOctree* p_light = this->getLightOctree();

  rapidjson::StringBuffer s;
  rapidjson::Writer<rapidjson::StringBuffer> writer(s);

  writer.StartObject(); // base
  // ---------------------------
  writer.Key("light_indices");
  writer.StartObject();
    writer.Key("length");
    writer.Int(p_linkless->getLightIndices()->size());
  writer.EndObject();
  // ---------------------------
  writer.Key("light_octree");
  writer.StartObject();
    writer.Key("origin");
    writer.StartObject();
      glm::vec3 pos_light = p_light->getOrigin();
      writer.Key("x");
      writer.Double(pos_light.x);
      writer.Key("y");
      writer.Double(pos_light.y);
      writer.Key("z");
      writer.Double(pos_light.z);
    writer.EndObject();
    writer.Key("depth");
    writer.Uint(p_light->getDepth());
  writer.EndObject();
  // ---------------------------
  writer.Key("linkless_octree");
  writer.StartObject();
    writer.Key("origin");
    writer.StartObject();
      glm::vec3 pos_linkless = p_linkless->getOrigin();
      writer.Key("x");
      writer.Double(pos_linkless.x);
      writer.Key("y");
      writer.Double(pos_linkless.y);
      writer.Key("z");
      writer.Double(pos_linkless.z);
    writer.EndObject();
    writer.Key("depth");
    writer.Uint(p_linkless->getDepth());
    writer.Key("n_levels");
    writer.Uint(p_linkless->getNLevels());
    writer.Key("tables");
    writer.StartArray();
    
    std::vector<SpatialHashFunction<glm::u8vec2>*>* octree_tables =
      p_linkless->getOctreeHashMaps();

    std::vector<SpatialHashFunction<glm::uvec2>*>* data_tables =
      p_linkless->getDataHashMaps();

    std::vector<bool>* exists = p_linkless->getDataHashMapsExists();
    for (unsigned int i = 0; i < octree_tables->size(); ++i) {
      writer.StartObject();
        writer.Key("octree_table");
        writer.StartObject();
          writer.Key("m");
          writer.Uint(octree_tables->at(i)->getM());
          writer.Key("r");
          writer.Uint(octree_tables->at(i)->getR());
        writer.EndObject();
        writer.Key("data_table");
        writer.StartObject();
          writer.Key("exists");
          writer.Bool(exists->at(i));

          if (exists->at(i)) {
            writer.Key("m");
            writer.Uint(data_tables->at(i)->getM());
            writer.Key("r");
            writer.Uint(data_tables->at(i)->getR());
          }
        writer.EndObject();
      writer.EndObject();
    }
    
    writer.EndArray();
  writer.EndObject();
  writer.EndObject();

  std::ofstream output_stream;
  output_stream.open(path);
  output_stream << s.GetString();
  output_stream.close();
}

}
}
}