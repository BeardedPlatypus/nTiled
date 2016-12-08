#pragma once

#include <string>

#include "LightOctree.h"

namespace nTiled {
namespace pipeline {
namespace hierarchical {

void exportToJson(const std::string& path_lights,
                  const std::string& path_octree,
                  const std::vector<world::PointLight*>& p_lights,
                  const LightOctree& octree);
void exportLights(const std::string& path,
                  const std::vector<world::PointLight*>& p_lights);
void exportLightOctree(const std::string& path,
                       const LightOctree& octree);

}
}
}