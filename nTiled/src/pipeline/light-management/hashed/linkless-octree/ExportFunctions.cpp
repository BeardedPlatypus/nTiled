#include "pipeline\light-management\hashed\linkless-octree\ExportFunctions.h"

#include <rapidjson\writer.h>
#include <rapidjson\stringbuffer.h>
#include <fstream>

namespace nTiled {
namespace pipeline {
namespace hashed {

void exportInitialLayers(std::string path,
                         std::vector<std::pair<glm::uvec3, lo::Node*>> nodes) {
  rapidjson::StringBuffer s;
  rapidjson::Writer<rapidjson::StringBuffer> writer(s);

  writer.StartObject();

  writer.Key("nodes");
  writer.StartArray();

  for (std::pair<glm::uvec3, lo::Node*> n : nodes) {
    writer.StartObject();
    writer.Key("position");
    writer.StartObject();
    writer.Key("x");
    writer.Uint(n.first.x);
    writer.Key("y");
    writer.Uint(n.first.y);
    writer.Key("z");
    writer.Uint(n.first.z);
    writer.EndObject();

    writer.Key("data");
    n.second->exportToJson(writer);
    writer.EndObject();
  }

  writer.EndArray();

  writer.EndObject();

  std::ofstream output_stream;
  output_stream.open(path);
  output_stream << s.GetString();
  output_stream.close();
}

}
}
}
