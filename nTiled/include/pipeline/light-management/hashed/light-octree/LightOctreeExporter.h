#pragma once

#include <string>

#include "LightOctree.h"

namespace nTiled {
namespace pipeline {
namespace hashed {

void exportLights(const std::string& path,
                  const std::vector<world::PointLight*>& p_lights);
void exportLightOctree(const std::string& path,
                       const LightOctree& octree);

}
}
}
