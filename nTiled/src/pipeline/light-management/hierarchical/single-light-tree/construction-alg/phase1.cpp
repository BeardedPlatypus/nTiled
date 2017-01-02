#include "pipeline\light-management\hierarchical\single-light-tree\SLTBuilder.h"

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <queue>
#include <math.h>

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "math\clamp.h"
#include "pipeline\light-management\hierarchical\single-light-tree\construction-util\FillElement.h"


namespace nTiled {
namespace pipeline {
namespace hierarchical {

bool SLTBuilder::lightWithinNode(const world::PointLight& light,
                                 glm::vec4 octree_node_origin,
                                 float slt_node_edge) {
  // calculation happens in world coordinates.
  glm::vec3 position = glm::vec3(octree_node_origin / octree_node_origin.w);

  // calculate closest point to the light within the node in each dimension
  double closest_x = math::clamp(position.x, 
                                 light.position.x, 
                                 position.x + slt_node_edge);
  double closest_y = math::clamp(position.y, 
                                 light.position.y, 
                                 position.y + slt_node_edge);
  double closest_z = math::clamp(position.z, 
                                 light.position.z, 
                                 position.z + slt_node_edge);

  // calculate distance between the light and this closest point
  double distance_x = light.position.x - closest_x;
  double distance_y = light.position.y - closest_y;
  double distance_z = light.position.z - closest_z;

  // calculate whether the closest points exceeds the radius
  return (( distance_x * distance_x
          + distance_y * distance_y
          + distance_z * distance_z) < ( light.radius * light.radius ));
}


slt::Lattice* SLTBuilder::lightToLattice(
    const world::PointLight& light,
    slt::NoLightNode const * const no_light,
    slt::FullLightNode const * const full_light) {

  // Calculate size of the lattice
  // subtract the origin in lattice in order to have it relative to the origin of the lattice
  glm::uvec3 lower_bottom_left = glm::uvec3(int(floor((light.position.x - light.radius - this->origin_octree.x) / this->minimum_leaf_node_size)),
                                            int(floor((light.position.y - light.radius - this->origin_octree.y) / this->minimum_leaf_node_size)),
                                            int(floor((light.position.z - light.radius - this->origin_octree.z) / this->minimum_leaf_node_size)));

  unsigned int n_tiles = int(floor((light.position.x + light.radius - this->origin_octree.x) / this->minimum_leaf_node_size)) - lower_bottom_left.x + 1;

  // Construct Lattice with FullLights
  slt::Lattice* lattice = new slt::Lattice(this->origin_octree,
                                           lower_bottom_left,
                                           n_tiles,
                                           0,
                                           this->minimum_leaf_node_size,
                                           *full_light);

  // Construct Checked Lattice
  std::vector<bool> checked = std::vector<bool>(n_tiles * n_tiles * n_tiles, false);

  unsigned int last = n_tiles - 1;
  std::queue<slt::FillElement> to_check = std::queue<slt::FillElement>(
    { slt::FillElement(glm::uvec3(   0,    0,    0), slt::FillDirection::LeftLowerClose),
      slt::FillElement(glm::uvec3(   0,    0, last), slt::FillDirection::LeftLowerFar),
      slt::FillElement(glm::uvec3(   0, last,    0), slt::FillDirection::LeftUpperClose),
      slt::FillElement(glm::uvec3(   0, last, last), slt::FillDirection::LeftUpperFar),
      slt::FillElement(glm::uvec3(last,    0,    0), slt::FillDirection::RightLowerClose),
      slt::FillElement(glm::uvec3(last,    0, last), slt::FillDirection::RightLowerFar),
      slt::FillElement(glm::uvec3(last, last,    0), slt::FillDirection::RightUpperClose),
      slt::FillElement(glm::uvec3(last, last, last), slt::FillDirection::RightUpperFar)
  });

  slt::FillElement next_element;
  int element_index;

  glm::vec3 origin_in_world = glm::vec3(lattice->getOriginInWorld() / lattice->getOriginInWorld().w);
  while (!to_check.empty()) {
    // Get next element
    next_element = to_check.front();
    to_check.pop();

    element_index = ( next_element.pos.x
                    + next_element.pos.z * n_tiles
                    + next_element.pos.y * n_tiles * n_tiles );

    if (!checked[element_index]) {
      checked[element_index] = true;


      glm::vec4 node_origin = glm::vec4(((glm::vec3(next_element.pos) * minimum_leaf_node_size) + origin_in_world), 1.0);
      if (!this->lightWithinNode(light, 
                                 node_origin, 
                                 this->minimum_leaf_node_size)) {
        lattice->setLatticeNode(next_element.pos, *no_light);

        // add additional FillElements
        switch (next_element.direction) {
        case slt::FillDirection::LeftLowerClose:
          to_check.push(slt::FillElement(next_element.pos + glm::uvec3(1, 0, 0),
                                    next_element.direction));
          to_check.push(slt::FillElement(next_element.pos + glm::uvec3(0, 1, 0),
                                    next_element.direction));
          to_check.push(slt::FillElement(next_element.pos + glm::uvec3(0, 0, 1),
                                    next_element.direction));
          break;
        case slt::FillDirection::LeftLowerFar:
          to_check.push(slt::FillElement(next_element.pos + glm::uvec3(1, 0, 0),
                                    next_element.direction));
          to_check.push(slt::FillElement(next_element.pos + glm::uvec3(0, 1, 0),
                                    next_element.direction));
          to_check.push(slt::FillElement(next_element.pos + glm::uvec3(0, 0, -1),
                                    next_element.direction));
          break;
        case slt::FillDirection::LeftUpperClose:
          to_check.push(slt::FillElement(next_element.pos + glm::uvec3(1, 0, 0),
                                    next_element.direction));
          to_check.push(slt::FillElement(next_element.pos + glm::uvec3(0, -1, 0),
                                    next_element.direction));
          to_check.push(slt::FillElement(next_element.pos + glm::uvec3(0, 0, 1),
                                    next_element.direction));
          break;
        case slt::FillDirection::LeftUpperFar:
          to_check.push(slt::FillElement(next_element.pos + glm::uvec3(1, 0, 0),
                                    next_element.direction));
          to_check.push(slt::FillElement(next_element.pos + glm::uvec3(0, -1, 0),
                                    next_element.direction));
          to_check.push(slt::FillElement(next_element.pos + glm::uvec3(0, 0, -1),
                                    next_element.direction));
          break;
        case slt::FillDirection::RightLowerClose:
          to_check.push(slt::FillElement(next_element.pos + glm::uvec3(-1, 0, 0),
                                    next_element.direction));
          to_check.push(slt::FillElement(next_element.pos + glm::uvec3(0, 1, 0),
                                    next_element.direction));
          to_check.push(slt::FillElement(next_element.pos + glm::uvec3(0, 0, 1),
                                    next_element.direction));
          break;
        case slt::FillDirection::RightLowerFar:
          to_check.push(slt::FillElement(next_element.pos + glm::uvec3(-1, 0, 0),
                                    next_element.direction));
          to_check.push(slt::FillElement(next_element.pos + glm::uvec3(0, 1, 0),
                                    next_element.direction));
          to_check.push(slt::FillElement(next_element.pos + glm::uvec3(0, 0, -1),
                                    next_element.direction));
          break;
        case slt::FillDirection::RightUpperClose:
          to_check.push(slt::FillElement(next_element.pos + glm::uvec3(-1, 0, 0),
                                    next_element.direction));
          to_check.push(slt::FillElement(next_element.pos + glm::uvec3(0, -1, 0),
                                    next_element.direction));
          to_check.push(slt::FillElement(next_element.pos + glm::uvec3(0, 0, 1),
                                    next_element.direction));
          break;
        case slt::FillDirection::RightUpperFar:
          to_check.push(slt::FillElement(next_element.pos + glm::uvec3(-1, 0, 0),
                                    next_element.direction));
          to_check.push(slt::FillElement(next_element.pos + glm::uvec3(0, -1, 0),
                                    next_element.direction));
          to_check.push(slt::FillElement(next_element.pos + glm::uvec3(0, 0, -1),
                                    next_element.direction));
          break;
        default:
          // TODO: add proper error exception
          break;
        }
      }
    }
  }

  return lattice;
}
}
}
}
