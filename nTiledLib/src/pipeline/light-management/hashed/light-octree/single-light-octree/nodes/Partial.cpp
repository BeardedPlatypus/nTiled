#include "pipeline\light-management\hashed\light-octree\single-light-tree\nodes\Partial.h"

#include "pipeline\light-management\hashed\light-octree\nodes\Node.h"

namespace nTiled {
namespace pipeline {
namespace hashed {
namespace slt {

Partial::Partial(const Node& child_000,
                 const Node& child_100,
                 const Node& child_010,
                 const Node& child_110,
                 const Node& child_001,
                 const Node& child_101,
                 const Node& child_011,
                 const Node& child_111) :
  child_000(child_000),
  child_001(child_001),
  child_010(child_010),
  child_011(child_011),
  child_100(child_100),
  child_101(child_101),
  child_110(child_110),
  child_111(child_111) {
}


Partial::~Partial() {}


Partial::Partial(Node const * const * children) :
  Partial(*(children[0]),
          *(children[1]),
          *(children[2]),
          *(children[3]),
          *(children[4]),
          *(children[5]),
          *(children[6]),
          *(children[7])) {
}


void Partial::addToLightOctreeNode(lo::Node* node, const SingleLightTree& slt) const {
  node->addPartialNode(slt, *this);
}

void Partial::exportToJson(rapidjson::Writer<rapidjson::StringBuffer>& writer) const {
  writer.StartObject();
  writer.Key("type");
  writer.String("partial");

  // sub nodes.
  writer.Key("n000");
  this->child_000.exportToJson(writer);
  writer.Key("n100");
  this->child_100.exportToJson(writer);
  writer.Key("n010");
  this->child_010.exportToJson(writer);
  writer.Key("n110");
  this->child_110.exportToJson(writer);
  writer.Key("n001");
  this->child_001.exportToJson(writer);
  writer.Key("n101");
  this->child_101.exportToJson(writer);
  writer.Key("n011");
  this->child_011.exportToJson(writer);
  writer.Key("n111");
  this->child_111.exportToJson(writer);
  writer.EndObject();
}

void Partial::exportToJson(rapidjson::Writer<rapidjson::StringBuffer>& writer,
                        glm::uvec3 position) const {
  writer.StartObject();
  writer.Key("type");
  writer.String("partial");

  // sub nodes.
  writer.Key("n000");
  this->child_000.exportToJson(writer);
  writer.Key("n100");
  this->child_100.exportToJson(writer);
  writer.Key("n010");
  this->child_010.exportToJson(writer);
  writer.Key("n110");
  this->child_110.exportToJson(writer);
  writer.Key("n001");
  this->child_001.exportToJson(writer);
  writer.Key("n101");
  this->child_101.exportToJson(writer);
  writer.Key("n011");
  this->child_011.exportToJson(writer);
  writer.Key("n111");
  this->child_111.exportToJson(writer);

  writer.Key("position");
  writer.StartObject();
  writer.Key("x");
  writer.Uint(position.x);
  writer.Key("y");
  writer.Uint(position.y);
  writer.Key("z");
  writer.Uint(position.z);
  writer.EndObject();

  writer.EndObject();
}

}
}
}
}