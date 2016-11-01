#include "pipeline\light-management\hierarchical\single-light-tree\construction-util\FillElement.h"

namespace nTiled {
namespace pipeline {
namespace hierarchical {
namespace slt {

FillElement::FillElement(glm::uvec3 pos, FillDirection direction) :
  pos(pos),
  direction(direction) { }


FillElement::FillElement() : FillElement(glm::uvec3(0),
                                         FillDirection::LeftUpperClose) { }


} // slt
} // hierarchical
} // pipeline
} // nTiled
