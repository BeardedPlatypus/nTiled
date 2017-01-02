#include "pipeline\light-management\hierarchical\LightOctreeBuilder.h"

#include <cmath>
#include "math\util.h"

#include "pipeline\light-management\hierarchical\single-light-tree\SLTBuilder.h"

namespace nTiled {
namespace pipeline {
namespace hierarchical {


LightOctreeBuilder::LightOctreeBuilder() {}
LightOctreeBuilder::~LightOctreeBuilder() {}

LightOctree* LightOctreeBuilder::buildLightOctree(const std::vector<world::PointLight*>& p_lights,
                                                  float minimum_leaf_node_size) {

  if (p_lights.empty()) {
    return nullptr;
  }

  // Determine octree origin and size
  // --------------------------------------------------------------------------
  world::PointLight* p_light = p_lights[0];
  float x_min = p_light->position.x - p_light->radius;
  float y_min = p_light->position.y - p_light->radius;
  float z_min = p_light->position.z - p_light->radius;

  float x_max = p_light->position.x + p_light->radius;
  float y_max = p_light->position.y + p_light->radius;
  float z_max = p_light->position.z + p_light->radius;

  float radius_temp;
  glm::vec4 position_temp;

  for (unsigned int i = 1; i < p_lights.size(); i++) {
    p_light = p_lights[i];

    radius_temp = p_light->radius;
    position_temp = p_light->position;

    // x
    if (x_min > position_temp.x - radius_temp) x_min = position_temp.x - radius_temp;
    if (x_max < position_temp.x + radius_temp) x_max = position_temp.x + radius_temp;
    // y
    if (y_min > position_temp.y - radius_temp) y_min = position_temp.y - radius_temp;
    if (y_max < position_temp.y + radius_temp) y_max = position_temp.y + radius_temp;
    // z
    if (z_min > position_temp.z - radius_temp) z_min = position_temp.z - radius_temp;
    if (z_max < position_temp.z + radius_temp) z_max = position_temp.z + radius_temp;
  }

  glm::vec4 octree_origin = glm::vec4((x_min - 0.1 * minimum_leaf_node_size),
                                      (y_min - 0.1 * minimum_leaf_node_size), 
                                      (z_min - 0.1 * minimum_leaf_node_size), 
                                      1.0);

  float size = (x_max - x_min);

  if (size < (y_max - y_min)) size = y_max - y_min;
  if (size < (z_max - z_min)) size = z_max - z_min;

  size += float(0.2 * minimum_leaf_node_size);

  unsigned int k = math::getNextPow2(int(ceil(size / minimum_leaf_node_size)));
  unsigned int octree_depth = (unsigned int)(log2(k));

  // Calculate Single Light Trees and construct single octree
  // --------------------------------------------------------------------------
  std::vector<SingleLightTree*> p_slts = std::vector<SingleLightTree*>();
  SLTBuilder builder = SLTBuilder(minimum_leaf_node_size, octree_origin);

  LightOctree* p_light_octree = new LightOctree(octree_origin, 
                                                minimum_leaf_node_size,
                                                octree_depth);

  SingleLightTree* p_slt;
  world::PointLight* pl;
  for (unsigned int i = 0; i < p_lights.size(); i++) {
    pl = p_lights[i];
    p_slt = builder.buildSingleLightTree(*pl, i);
    p_slts.push_back(p_slt);
    
    p_light_octree->addSLT(*p_slt);
  }

  return p_light_octree;
}

} // hierarchical
} // pipeline
} // nTiled