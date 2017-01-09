#include "pipeline\light-management\hashed\light-octree\single-light-tree\SingleLightTree.h"

// ---------------------------------------------------------------------------
//  Libraries
// ---------------------------------------------------------------------------
#include <rapidjson\writer.h>
#include <rapidjson\stringbuffer.h>
#include <fstream>

namespace nTiled {
namespace pipeline {
namespace hashed {

SingleLightTree::SingleLightTree(const world::PointLight& light,
                                 const GLuint index,
                                 const glm::uvec3 origin_in_octree,
                                 const slt::Node& root,
                                 const unsigned int depth,
                                 slt::Full const * const p_full_light,
                                 slt::No const * const p_no_light,
                                 const std::vector<slt::Partial const *> partial_light_nodes) :
  light(light),
  index(index),
  origin_in_octree(origin_in_octree),
  root(root),
  depth(depth),
  p_full_light(p_full_light),
  p_no_light(p_no_light),
  partial_light_nodes(partial_light_nodes) {
}


SingleLightTree::~SingleLightTree() {
  delete p_full_light;
  delete p_no_light;

  for (slt::Partial const * const p_partial_node : this->partial_light_nodes) {
    delete p_partial_node;
  }
}


// TODO
void SingleLightTree::exportToJson(const std::string& path,
                                   unsigned int depth,
                                   glm::vec3 octree_origin,
                                   float minimum_node_size) {
  rapidjson::StringBuffer s;
  rapidjson::Writer<rapidjson::StringBuffer> writer(s);

  writer.StartObject();
  // write nodes
  writer.Key("nodes");
  this->getRoot().exportToJson(writer);

  // write octree origin offset
  writer.Key("octree_origin");
  writer.StartObject();
  writer.Key("x");
  writer.Double(octree_origin.x);
  writer.Key("y");
  writer.Double(octree_origin.y);
  writer.Key("z");
  writer.Double(octree_origin.z);
  writer.EndObject();

  // write depth of this SLT
  writer.Key("depth");
  writer.Uint(depth);

  // write position in octree coordinates
  glm::uvec3 position = this->getOriginInOctree();

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
  writer.Double(minimum_node_size);

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
