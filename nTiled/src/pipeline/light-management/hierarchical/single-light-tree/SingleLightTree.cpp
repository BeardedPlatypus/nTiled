#include "pipeline\light-management\hierarchical\single-light-tree\SingleLightTree.h"

// ---------------------------------------------------------------------------
//  Libraries
// ---------------------------------------------------------------------------
#include <rapidjson\writer.h>
#include <rapidjson\stringbuffer.h>
#include <fstream>

namespace nTiled {
namespace pipeline {
namespace hierarchical {

SingleLightTree::SingleLightTree(const world::PointLight& light,
                                 const glm::ivec3 position,
                                 const slt::Node& root,
                                 const float root_size,
                                 slt::FullLightNode const * const p_full_light,
                                 slt::NoLightNode const * const p_no_light,
                                 const std::vector<slt::PartialLightNode const *> partial_light_nodes) :
    light(light),
    position(position),
    root(root),
    root_size(root_size),
    p_full_light(p_full_light),
    p_no_light(p_no_light),
    partial_light_nodes(partial_light_nodes) { }


SingleLightTree::~SingleLightTree() {
  delete p_full_light;
  delete p_no_light;

  for (slt::PartialLightNode const * const p_partial_node : this->partial_light_nodes) {
    delete p_partial_node;
  }
}


void SingleLightTree::exportToJson(const std::string& path) {
  rapidjson::StringBuffer s;
  rapidjson::Writer<rapidjson::StringBuffer> writer(s);

  writer.StartObject();
  // write nodes
  writer.Key("nodes");
  this->getRoot().exportToJson(writer);

  // write position in lattice coordinates
  glm::ivec3 position = this->getPosition();

  writer.Key("position");
  writer.StartObject();
    writer.Key("x");
    writer.Int(position.x);
    writer.Key("y");
    writer.Int(position.y);
    writer.Key("z");
    writer.Int(position.z);
  writer.EndObject();

  // write size
  writer.Key("size");
  writer.Double(this->getRootSize());

  // write light
  const world::PointLight& light = this->getLight();

  writer.Key("light");
  writer.StartObject();
    writer.Key("position");
    writer.StartObject();
      writer.Key("x");
      writer.Double(light.position.x / light.position.w);
      writer.Key("y");
      writer.Double(light.position.y / light.position.w);
      writer.Key("z");
      writer.Double(light.position.z / light.position.w);
    writer.EndObject();

    writer.Key("intensity");
    writer.StartObject();
      writer.Key("r");
      writer.Double(light.intensity.r);
      writer.Key("g");
      writer.Double(light.intensity.g);
      writer.Key("b");
      writer.Double(light.intensity.b);
    writer.EndObject();

    writer.Key("radius");
    writer.Double(light.radius);
  writer.EndObject();
  writer.EndObject();

  std::ofstream output_stream;
  output_stream.open(path);
  output_stream << s.GetString();
  output_stream.close();
}


} // hierarchical
} // pipeline
} // nTiled