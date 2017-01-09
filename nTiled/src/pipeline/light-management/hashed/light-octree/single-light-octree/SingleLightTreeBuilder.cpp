#include "pipeline\light-management\hashed\light-octree\single-light-tree\SingleLightTreeBuilder.h"

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <queue>
#include <math.h>


// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "math\clamp.h"
#include "pipeline\light-management\hashed\light-octree\single-light-tree\octree-layer\FillElement.h"


namespace nTiled {
namespace pipeline {
namespace hashed {

SingleLightTreeBuilder::SingleLightTreeBuilder(
    const float minimum_leaf_node_size,
    const glm::vec4 origin_octree,
    const unsigned int octree_depth) :
  minimum_leaf_node_size(minimum_leaf_node_size),
  origin_octree(glm::vec3(origin_octree) / origin_octree.w),
  octree_depth(octree_depth) {
}


SingleLightTree* SingleLightTreeBuilder::buildSingleLightTree(const world::PointLight& light,                                                  
                                                              GLuint index) {
  slt::No* no_light = new slt::No();
  slt::Full* full_light = new slt::Full();

  slt::OctreeLayer* current_layer = this->lightToLattice(light,
                                                         no_light,
                                                         full_light);
  slt::OctreeLayer* next_layer;

  std::vector<slt::Partial const *> partial_light_nodes = std::vector<slt::Partial const *>();

  while (current_layer->getNNodes() > 1) {
    next_layer = combineLatticeStep(*current_layer,
                                    no_light,
                                    partial_light_nodes);

    delete current_layer;
    current_layer = next_layer;
  }

  return new SingleLightTree(light,
                             index,
                             current_layer->getOriginInLattice(),
                             *current_layer->getOctreeLayerNode(glm::uvec3(0)).node,
                             current_layer->getDepth(),
                             full_light,
                             no_light,
                             partial_light_nodes);
}


// --------------------------------------------------------------------------------------------------------------------
//  Phase 1
bool SingleLightTreeBuilder::lightWithinNode(const world::PointLight& light,
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
  return ((distance_x * distance_x
           + distance_y * distance_y
           + distance_z * distance_z) < (light.radius * light.radius));
}


slt::OctreeLayer* SingleLightTreeBuilder::lightToLattice(const world::PointLight& light,
                                                         slt::No const * const no_light,
                                                         slt::Full const * const full_light) {
  // Calculate size of the lattice
  // subtract the origin in lattice in order to have it relative to the origin of the lattice
  glm::uvec3 lower_bottom_left = glm::uvec3(int(floor((light.position.x - light.radius - this->origin_octree.x) / this->minimum_leaf_node_size)),
                                            int(floor((light.position.y - light.radius - this->origin_octree.y) / this->minimum_leaf_node_size)),
                                            int(floor((light.position.z - light.radius - this->origin_octree.z) / this->minimum_leaf_node_size)));

  unsigned int n_tiles = int(floor((light.position.x + light.radius - this->origin_octree.x) / this->minimum_leaf_node_size)) - lower_bottom_left.x + 1;

  // Construct Lattice with FullLights
  slt::OctreeLayer* layer = new slt::OctreeLayer(lower_bottom_left,
                                                 n_tiles,
                                                 this->octree_depth - 1,
                                                 this->minimum_leaf_node_size,
                                                 *full_light);

  // Construct Checked Lattice
  std::vector<bool> checked = std::vector<bool>(n_tiles * n_tiles * n_tiles, false);

  unsigned int last = n_tiles - 1;
  std::queue<slt::FillElement> to_check = std::queue<slt::FillElement>(
  { slt::FillElement(glm::uvec3(0,    0,    0), slt::FillDirection::LeftLowerClose),
    slt::FillElement(glm::uvec3(0,    0, last), slt::FillDirection::LeftLowerFar),
    slt::FillElement(glm::uvec3(0, last,    0), slt::FillDirection::LeftUpperClose),
    slt::FillElement(glm::uvec3(0, last, last), slt::FillDirection::LeftUpperFar),
    slt::FillElement(glm::uvec3(last,    0,    0), slt::FillDirection::RightLowerClose),
    slt::FillElement(glm::uvec3(last,    0, last), slt::FillDirection::RightLowerFar),
    slt::FillElement(glm::uvec3(last, last,    0), slt::FillDirection::RightUpperClose),
    slt::FillElement(glm::uvec3(last, last, last), slt::FillDirection::RightUpperFar)
  });

  slt::FillElement next_element;
  int element_index;

  glm::uvec3 origin_node_layer;
  glm::vec3 origin_node_world;
  while (!to_check.empty()) {
    // Get next element
    next_element = to_check.front();
    to_check.pop();

    element_index = (next_element.pos.x
                     + next_element.pos.y * n_tiles
                     + next_element.pos.z * n_tiles * n_tiles);

    if (!checked[element_index]) {
      checked[element_index] = true;

      // TODO CHECK IF THIS IS CORRECT
      origin_node_layer = next_element.pos + lower_bottom_left;
      origin_node_world = (glm::vec3(origin_node_layer) * minimum_leaf_node_size
                           + this->origin_octree);

      if (!this->lightWithinNode(light,
                                 glm::vec4(origin_node_world, 1.0),
                                 this->minimum_leaf_node_size)) {
        layer->setOctreeLayerNode(next_element.pos, *no_light);

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

  std::string path = ("C:/Users/Monthy/Documents/projects/thesis/hierarchical-shading/test/layer_" + 
                      std::to_string(layer->getDepth()) + "_" + light.name + ".json");
  layer->exportToJson(path, this->origin_octree);

  return layer;
}


// --------------------------------------------------------------------------------------------------------------------
//  Phase 2
slt::Offsets SingleLightTreeBuilder::calculateOffsets(const slt::OctreeLayer& lattice) {
  if (lattice.getNNodes() & 1) {
    // extend lattice by one
    glm::ivec3 origin = lattice.getOriginInLattice();
    return slt::Offsets(!(origin.x & 1),
                        !(origin.y & 1),
                        !(origin.z & 1));
  } else {
    // extend lattice by none or two
    return slt::Offsets(lattice.getOriginInLattice().x & 1);
  }
}


void SingleLightTreeBuilder::updateCoreLattice(const slt::OctreeLayer& current_lattice,
                                   const slt::Offsets& offsets,
                                   slt::OctreeLayer* next_lattice,
                                   std::vector<slt::Partial const *>& partial_light_nodes) {
  int next_n_tiles = next_lattice->getNNodes();

  glm::uvec2 x_offset = glm::uvec2(offsets.x);
  glm::uvec2 y_offset = glm::uvec2(offsets.y);
  glm::uvec2 z_offset = glm::uvec2(offsets.z);

  unsigned int n_full_lights;
  unsigned int n_no_lights;

  glm::uvec3 next_node_origin;
  // loop over the y layers
  for (unsigned int y_i = y_offset.x; y_i < next_n_tiles - y_offset.y; y_i++) {
    // loop over z rows in a y layer
    for (unsigned int z_i = z_offset.x; z_i < next_n_tiles - z_offset.y; z_i++) {
      // loop over the x elements in a z row
      for (unsigned int x_i = x_offset.x; x_i < next_n_tiles - x_offset.y; x_i++) {
        n_full_lights = 0;
        n_no_lights = 0;

        next_node_origin = glm::uvec3(x_i * 2 - x_offset.x,
                                      y_i * 2 - y_offset.x,
                                      z_i * 2 - z_offset.x);

        slt::OctreeLayerNode const * const sub_nodes[8] = {
          &current_lattice.getOctreeLayerNode(next_node_origin),
          &current_lattice.getOctreeLayerNode(next_node_origin + glm::uvec3(1, 0, 0)),
          &current_lattice.getOctreeLayerNode(next_node_origin + glm::uvec3(0, 1, 0)),
          &current_lattice.getOctreeLayerNode(next_node_origin + glm::uvec3(1, 1, 0)),
          &current_lattice.getOctreeLayerNode(next_node_origin + glm::uvec3(0, 0, 1)),
          &current_lattice.getOctreeLayerNode(next_node_origin + glm::uvec3(1, 0, 1)),
          &current_lattice.getOctreeLayerNode(next_node_origin + glm::uvec3(0, 1, 1)),
          &current_lattice.getOctreeLayerNode(next_node_origin + glm::uvec3(1, 1, 1))
        };

        for (int i = 0; i < 8; i++) {
          if (n_no_lights == i && sub_nodes[i]->type == slt::NodeType::NoLight) {
            n_no_lights += 1;
          } else if (n_full_lights == i && sub_nodes[i]->type == slt::NodeType::FullLight) {
            n_full_lights += 1;
          } else {
            break;
          }
        }


        if (n_no_lights == 8 || n_full_lights == 8) {
          // if it is either no lights or full lights, any sub node will also be
          // of that type and we will recycle it.
          next_lattice->setOctreeLayerNode(glm::uvec3(x_i, y_i, z_i),
                                           *(sub_nodes[0]));
        } else {
          slt::Partial* p_partial_light_node =
            new slt::Partial(*(sub_nodes[0]->node),
                             *(sub_nodes[1]->node),
                             *(sub_nodes[2]->node),
                             *(sub_nodes[3]->node),
                             *(sub_nodes[4]->node),
                             *(sub_nodes[5]->node),
                             *(sub_nodes[6]->node),
                             *(sub_nodes[7]->node));

          partial_light_nodes.push_back(p_partial_light_node);
          next_lattice->setOctreeLayerNode(glm::uvec3(x_i, y_i, z_i),
                                           *p_partial_light_node);
        }
      }
    }
  }
}


void SingleLightTreeBuilder::updateCornersLattice(const slt::OctreeLayer& current_lattice,
                                      const slt::Offsets& offsets,
                                      slt::No const * const no_light,
                                      slt::OctreeLayer* next_lattice,
                                      std::vector<slt::Partial const *>& partial_light_nodes) {
  unsigned int last_current = current_lattice.getNNodes() - 1;
  unsigned int last_next = next_lattice->getNNodes() - 1;

  for (const auto& c : offsets.corners) {
    const slt::OctreeLayerNode& corner_node =
      current_lattice.getOctreeLayerNode((glm::uvec3(c) * last_current));

    if (corner_node.type == slt::NodeType::NoLight) {
      next_lattice->setOctreeLayerNode((glm::uvec3(c) * last_next),
                                   slt::OctreeLayerNode(slt::NodeType::NoLight,
                                                        no_light));
    } else {
      // all but one subnode is not no_light, so we start by initialising 
      // the array with only no light.
      slt::Node const * new_nodes[8] = { no_light,
        no_light,
        no_light,
        no_light,
        no_light,
        no_light,
        no_light,
        no_light };
      // and replace the one corner
      new_nodes[7 - 4 * c.z - 2 * c.y - c.x] = corner_node.node;

      slt::Partial* p_partial_light_node = new slt::Partial(new_nodes);
      partial_light_nodes.push_back(p_partial_light_node);

      next_lattice->setOctreeLayerNode((glm::uvec3(c) * last_next),
                                       *p_partial_light_node);
    }
  }
}

//FIXME: DIRTY FIX
unsigned int normalise(bool x) {
  if (x) {
    return 1;
  } else {
    return 0;
  }
}

void SingleLightTreeBuilder::updateEdgesLattice(const slt::OctreeLayer& current_lattice,
                                    const slt::Offsets& offsets,
                                    slt::No const * const no_light,
                                    slt::OctreeLayer* next_lattice,
                                    std::vector<slt::Partial const *>& partial_light_nodes) {
  unsigned int last_current = current_lattice.getNNodes() - 1;
  unsigned int last_next = next_lattice->getNNodes() - 1;

  glm::uvec3 next_node_position;
  glm::uvec3 offset;
  glm::uvec2 offset_dimension;
  glm::uvec3 direction;

  glm::uvec3 sub_node_offset;

  glm::uvec3 sub_node;

  for (const auto& e : offsets.edges) {
    offset = glm::uvec3(e.offset_position) * last_next;
    direction = e.direction;

    offset_dimension = glm::uvec2(normalise(e.offset_dimension.x), normalise(e.offset_dimension.y));

    sub_node_offset = glm::uvec3(e.offset_position) * last_current;

    // sub nodes always exist at same same place, thus can be calculated per edge
    unsigned int i_edge_node2 = 7 - 4 * e.offset_position.z
      - 2 * e.offset_position.y
      - e.offset_position.x;
    unsigned int i_edge_node1 = i_edge_node2 - 4 * direction.z - 2 * direction.y - direction.x;

    for (unsigned int i = offset_dimension.x; i < (next_lattice->getNNodes() - offset_dimension.y); i++) {
      next_node_position = offset + direction * i;

      sub_node = glm::uvec3(glm::ivec3(sub_node_offset + direction * (2 * i)) -
                            (glm::ivec3(direction) * int(offset_dimension.x)));

      const slt::OctreeLayerNode& edge_node1 = current_lattice.getOctreeLayerNode(sub_node);
      const slt::OctreeLayerNode& edge_node2 = current_lattice.getOctreeLayerNode(sub_node + direction);

      if (edge_node1.type == slt::NodeType::NoLight &&
          edge_node2.type == slt::NodeType::NoLight) {
        next_lattice->setOctreeLayerNode((next_node_position),
                                     slt::OctreeLayerNode(slt::NodeType::NoLight,
                                                          no_light));
      } else {
        // all but one subnode is not no_light, so we start by initialising 
        // the array with only no light.
        slt::Node const * new_nodes[8] = { no_light,
          no_light,
          no_light,
          no_light,
          no_light,
          no_light,
          no_light,
          no_light };
        // and replace sub nodes
        new_nodes[i_edge_node1] = edge_node1.node;
        new_nodes[i_edge_node2] = edge_node2.node;

        slt::Partial* p_partial_light_node = new slt::Partial(new_nodes);
        partial_light_nodes.push_back(p_partial_light_node);

        next_lattice->setOctreeLayerNode(next_node_position,
                                         *p_partial_light_node);
      }
    }
  }
}


void SingleLightTreeBuilder::updateSidesLattice(const slt::OctreeLayer& current_lattice,
                                    const slt::Offsets& offsets,
                                    slt::No const * const no_light,
                                    slt::OctreeLayer* next_lattice,
                                    std::vector<slt::Partial const *>& partial_light_nodes) {
  unsigned int last_current = current_lattice.getNNodes() - 1;
  unsigned int last_next = next_lattice->getNNodes() - 1;

  glm::uvec3 next_node_position;
  glm::uvec3 offset;

  glm::uvec3 sub_node_offset;

  glm::uvec3 d_i;
  glm::uvec3 d_j;

  glm::uvec2 offset_dimension_i;
  glm::uvec2 offset_dimension_j;

  glm::uvec3 sub_node00;
  glm::uvec3 sub_node01;
  glm::uvec3 sub_node10;
  glm::uvec3 sub_node11;

  for (const auto& s : offsets.sides) {
    offset = glm::uvec3(s.offset_position) * last_next;
    sub_node_offset = glm::uvec3(s.offset_position) * last_current;

    d_i = s.direction_i;
    d_j = s.direction_j;

    offset_dimension_i = glm::uvec2(normalise(s.offset_i.x), normalise(s.offset_i.y));
    offset_dimension_j = glm::uvec2(normalise(s.offset_j.x), normalise(s.offset_j.y));

    // sub nodes always exist at same same place, thus can be calculated per side
    // we assume that position 11 equals node i+1 j+1 etc.
    //             ij
    unsigned i_node11 = 7 - 4 * s.offset_position.z - 2 * s.offset_position.y - s.offset_position.x;
    unsigned i_node10 = i_node11 - 4 * d_j.z - 2 * d_j.y - d_j.x;
    unsigned i_node01 = i_node11 - 4 * d_i.z - 2 * d_i.y - d_i.x;
    unsigned i_node00 = i_node10 - 4 * d_i.z - 2 * d_i.y - d_i.x;

    for (unsigned int j = offset_dimension_j.x; j < next_lattice->getNNodes() - offset_dimension_j.y; j++) {
      for (unsigned int i = offset_dimension_i.x; i < next_lattice->getNNodes() - offset_dimension_i.y; i++) {
        next_node_position = offset + d_i * i + d_j * j;

        sub_node00 = glm::uvec3(glm::ivec3(sub_node_offset + d_i * (2 * i) + d_j * (2 * j)) -
                                (glm::ivec3(d_i) * int(offset_dimension_i.x)) -
                                (glm::ivec3(d_j) * int(offset_dimension_j.x)));

        sub_node01 = sub_node00 + d_j;
        sub_node10 = sub_node00 + d_i;
        sub_node11 = sub_node01 + d_i;

        const slt::OctreeLayerNode& side_node00 = current_lattice.getOctreeLayerNode(sub_node00);
        const slt::OctreeLayerNode& side_node01 = current_lattice.getOctreeLayerNode(sub_node01);
        const slt::OctreeLayerNode& side_node10 = current_lattice.getOctreeLayerNode(sub_node10);
        const slt::OctreeLayerNode& side_node11 = current_lattice.getOctreeLayerNode(sub_node11);

        if (side_node00.type == slt::NodeType::NoLight &&
            side_node01.type == slt::NodeType::NoLight &&
            side_node10.type == slt::NodeType::NoLight &&
            side_node11.type == slt::NodeType::NoLight) {
          next_lattice->setOctreeLayerNode((next_node_position),
                                       slt::OctreeLayerNode(slt::NodeType::NoLight,
                                                            no_light));
        } else {
          slt::Node const * new_nodes[8] = { no_light,
            no_light,
            no_light,
            no_light,
            no_light,
            no_light,
            no_light,
            no_light };
          // and replace sub nodes
          new_nodes[i_node00] = side_node00.node;
          new_nodes[i_node10] = side_node10.node;
          new_nodes[i_node01] = side_node01.node;
          new_nodes[i_node11] = side_node11.node;

          slt::Partial* p_partial_light_node = new slt::Partial(new_nodes);
          partial_light_nodes.push_back(p_partial_light_node);

          next_lattice->setOctreeLayerNode(next_node_position,
                                           *p_partial_light_node);
        }
      }
    }
  }
}


slt::OctreeLayer* SingleLightTreeBuilder::combineLatticeStep(
  const slt::OctreeLayer& current_lattice,
  slt::No const * const no_light,
  std::vector<slt::Partial const *>& partial_light_nodes) {

  slt::Offsets offset = this->calculateOffsets(current_lattice);

  unsigned int next_nodes = unsigned int((current_lattice.getNNodes() + offset.x.x + offset.x.y) * 0.5);

  glm::uvec3 origin = (current_lattice.getOriginInLattice() - glm::uvec3(offset.x.x, offset.y.x, offset.z.x)) / (unsigned int(2));
  slt::OctreeLayer* p_next_lattice = new slt::OctreeLayer(origin,
                                                          next_nodes,
                                                          current_lattice.getDepth() - 1,
                                                          current_lattice.getNodeSize() * 2);

  // Update middle section
  this->updateCoreLattice(current_lattice, offset, p_next_lattice, partial_light_nodes);

  // check if offsets need to be updated
  if (offset.x.x || offset.x.y) {
    this->updateCornersLattice(current_lattice,
                               offset,
                               no_light,
                               p_next_lattice,
                               partial_light_nodes);
    this->updateEdgesLattice(current_lattice,
                             offset,
                             no_light,
                             p_next_lattice,
                             partial_light_nodes);
    this->updateSidesLattice(current_lattice,
                             offset,
                             no_light,
                             p_next_lattice,
                             partial_light_nodes);
  }

  std::string path = ("C:/Users/Monthy/Documents/projects/thesis/hierarchical-shading/test/poop_layer_" + 
                      std::to_string(p_next_lattice->getDepth()) + ".json");
  p_next_lattice->exportToJson(path, this->origin_octree);

  return p_next_lattice;
}



} // hierarchical
} // pipeline
} // nTiled

