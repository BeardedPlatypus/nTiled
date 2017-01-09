#pragma once

#include <glm\glm.hpp>

#include "Node.h"

namespace nTiled {
namespace pipeline {
namespace hashed {
namespace slt {


class Partial: public Node {
public:
  Partial(const Node& child_000,
          const Node& child_100,
          const Node& child_010,
          const Node& child_110,
          const Node& child_001,
          const Node& child_101,
          const Node& child_011,
          const Node& child_111);

  Partial(Node const * const * children);


  virtual ~Partial() override;

  virtual void addToLightOctreeNode(lo::Node* node, 
                                    const SingleLightTree& slt) const override;

  virtual void exportToJson(rapidjson::Writer<rapidjson::StringBuffer>& writer) const override;
  virtual void exportToJson(rapidjson::Writer<rapidjson::StringBuffer>& writer, glm::uvec3 position) const;

  const Node& child_000;
  const Node& child_100;
  const Node& child_010;
  const Node& child_110;
  const Node& child_001;
  const Node& child_101;
  const Node& child_011;
  const Node& child_111;
};

}
}
}
}
