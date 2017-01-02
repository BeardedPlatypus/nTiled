#include "pipeline\light-management\hierarchical\LightOctreeExporter.h"

#include <rapidjson\writer.h>
#include <rapidjson\stringbuffer.h>
#include <fstream>

namespace nTiled {
namespace pipeline {
namespace hierarchical {

void fExportToJson(const std::string& path_lights,
                  const std::string& path_octree,
                  const std::vector<world::PointLight*>& p_lights,
                  const LightOctree& octree) {
  exportLights(path_lights, p_lights);
  exportLightOctree(path_octree, octree);
}


void exportLights(const std::string& path,
                  const std::vector<world::PointLight*>& p_lights) {
  rapidjson::StringBuffer s;
  rapidjson::Writer<rapidjson::StringBuffer> writer(s);

  writer.StartObject(); 

  // --------------------------------------------
  //  Lights
  writer.Key("lights");
  writer.StartArray();

  for (world::PointLight* p : p_lights) {
    writer.StartObject();
      writer.Key("position");
      writer.StartObject();
        writer.Key("x");
        writer.Double(p->position.x / p->position.w);
        writer.Key("y");
        writer.Double(p->position.y / p->position.w);
        writer.Key("z");
        writer.Double(p->position.z / p->position.w);
      writer.EndObject();

      writer.Key("intensity");
      writer.StartObject();
        writer.Key("r");
        writer.Double(p->intensity.r);
        writer.Key("g");
        writer.Double(p->intensity.g);
        writer.Key("b");
        writer.Double(p->intensity.b);
      writer.EndObject();

      writer.Key("radius");
      writer.Double(p->radius);
    writer.EndObject();
  }

  writer.EndArray();
  writer.EndObject();

  std::ofstream output_stream;
  output_stream.open(path);
  output_stream << s.GetString();
  output_stream.close();
}


void exportLightOctree(const std::string&path,
                       const LightOctree& octree) {
  rapidjson::StringBuffer s;
  rapidjson::Writer<rapidjson::StringBuffer> writer(s);

  writer.StartObject(); 
      writer.Key("node_size");
      writer.Double(octree.getMinimumLeafNodeSize());

      writer.Key("depth");
      writer.Uint(octree.getDepth());

      writer.Key("origin");
      writer.StartObject();
        glm::vec4 position = octree.getOrigin();
        writer.Key("x");
        writer.Double(position.x / position.w);
        writer.Key("y");
        writer.Double(position.y / position.w);
        writer.Key("z");
        writer.Double(position.z / position.w);
      writer.EndObject();

      writer.Key("nodes");
      octree.getRoot().exportToJson(writer);
  writer.EndObject();

  std::ofstream output_stream;
  output_stream.open(path);
  output_stream << s.GetString();
  output_stream.close();
}


}
}
}
