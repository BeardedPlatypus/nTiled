#pragma once

#include <string>
#include "SingleLightTree.h"

namespace nTiled {
namespace pipeline {
namespace hierarchical {

void exportSLT(const std::string& path,
               const SingleLightTree& tree);

}
}
}
