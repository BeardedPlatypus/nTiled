#include "pipeline\light-management\hashed\light-octree\nodes\LONodeContainer.h"

namespace nTiled {
namespace pipeline {
namespace hashed {

LONodeContainer::LONodeContainer(LONode* p_node,
                                 LOParent* p_parent) :
  p_node(p_node),
  p_parent(p_parent) {
}

}
}
}

