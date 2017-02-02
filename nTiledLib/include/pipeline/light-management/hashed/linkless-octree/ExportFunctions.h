#pragma once

#include <string>
#include <vector>

#include <glm\glm.hpp>

#include "pipeline\light-management\hashed\light-octree\nodes\Node.h"

namespace nTiled {
namespace pipeline {
namespace hashed {

void exportInitialLayers(std::string path,
                         std::vector<std::pair<glm::uvec3, lo::Node*>> nodes);


}
}
}