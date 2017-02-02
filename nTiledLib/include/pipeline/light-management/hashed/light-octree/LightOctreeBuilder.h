#pragma once

#include <vector>

#include <world\PointLight.h>
#include "LightOctree.h"

namespace nTiled {
namespace pipeline {
namespace hashed {

class LightOctreeBuilder {
public:
  // --------------------------------------------------------------------------
  //  Constructor | Destructor
  // --------------------------------------------------------------------------
  LightOctreeBuilder();
  ~LightOctreeBuilder();

  // --------------------------------------------------------------------------
  //  Construction method
  // --------------------------------------------------------------------------
  /*! @brief Build a new LightOctree with the given pointers to point lights
  *  and minimal node size.
  *
  * @param p_lights Vector containing pointers to all point lights to be
  *                 included in this new LightOctree.
  * @param minimum_leaf_node_size the size of the smallest nodes in the new
  *                               light octree
  *
  * @return A pointer to the newly constructed LightOctree
  */
  LightOctree* buildLightOctree(const std::vector<world::PointLight*>& p_lights,
                                float minimum_leaf_node_size);
};


}
}
}
