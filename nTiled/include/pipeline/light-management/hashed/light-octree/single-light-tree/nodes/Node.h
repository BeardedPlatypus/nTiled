#pragma once

// ---------------------------------------------------------------------------
//  Libraries
// ---------------------------------------------------------------------------
#include <glm\glm.hpp>
#include <rapidjson\writer.h>
#include <rapidjson\stringbuffer.h>


namespace nTiled {
namespace pipeline {
namespace hashed {

class SingleLightTree;

namespace lo {
class Node;
}

namespace slt {

class Node {
public:
  virtual ~Node() { }

  virtual void addToLightOctreeNode(lo::Node* node, 
                                    const SingleLightTree& slt) const = 0;

  virtual void exportToJson(rapidjson::Writer<rapidjson::StringBuffer>& writer) const = 0;
  virtual void exportToJson(rapidjson::Writer<rapidjson::StringBuffer>& writer, glm::uvec3 position) const = 0;
};

}
}
}
}