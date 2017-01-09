#include "pipeline\light-management\hashed\light-octree\single-light-tree\octree-layer\FillElement.h"

namespace nTiled {
namespace pipeline {
namespace hashed {
namespace slt {

FillElement::FillElement(glm::uvec3 pos, FillDirection direction) :
  pos(pos),
  direction(direction) { }


FillElement::FillElement() : FillElement(glm::uvec3(0),
                                         FillDirection::LeftUpperClose) {
}


} // slt
} // hierarchical
} // pipeline
} // nTiled
