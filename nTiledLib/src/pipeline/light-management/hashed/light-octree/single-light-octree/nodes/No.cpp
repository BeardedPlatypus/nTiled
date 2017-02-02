#include "pipeline\light-management\hashed\light-octree\single-light-tree\nodes\No.h"

#include "pipeline\light-management\hashed\light-octree\nodes\Node.h"

namespace nTiled {
namespace pipeline {
namespace hashed {
namespace slt {

No::No() {}
No::~No() {}

void No::addToLightOctreeNode(lo::Node* node, const SingleLightTree& slt) const {}

void No::exportToJson(rapidjson::Writer<rapidjson::StringBuffer>& writer) const {
  writer.StartObject();
  writer.Key("type");
  writer.String("no");
  writer.EndObject();
}

void No::exportToJson(rapidjson::Writer<rapidjson::StringBuffer>& writer,
                        glm::uvec3 position) const {
  writer.StartObject();
  writer.Key("type");
  writer.String("no");

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