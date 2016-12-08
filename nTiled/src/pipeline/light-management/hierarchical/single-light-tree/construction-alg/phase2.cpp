#include "pipeline\light-management\hierarchical\single-light-tree\SLTBuilder.h"


namespace nTiled {
namespace pipeline {
namespace hierarchical {

slt::Offsets SLTBuilder::calculateOffsets(const slt::Lattice& lattice) {
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


void SLTBuilder::updateCoreLattice(const slt::Lattice& current_lattice,
                                   const slt::Offsets& offsets,
                                   slt::Lattice* next_lattice,
                                   std::vector<slt::PartialLightNode const *>& partial_light_nodes) {
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

        slt::LatticeNode const * const sub_nodes[8] = { 
          &current_lattice.getLatticeNode(next_node_origin),
          &current_lattice.getLatticeNode(next_node_origin + glm::uvec3(1, 0, 0)),
          &current_lattice.getLatticeNode(next_node_origin + glm::uvec3(0, 1, 0)),
          &current_lattice.getLatticeNode(next_node_origin + glm::uvec3(1, 1, 0)),
          &current_lattice.getLatticeNode(next_node_origin + glm::uvec3(0, 0, 1)),
          &current_lattice.getLatticeNode(next_node_origin + glm::uvec3(1, 0, 1)),
          &current_lattice.getLatticeNode(next_node_origin + glm::uvec3(0, 1, 1)),
          &current_lattice.getLatticeNode(next_node_origin + glm::uvec3(1, 1, 1))
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
          next_lattice->setLatticeNode(glm::uvec3(x_i, y_i, z_i),
                                       *(sub_nodes[0]));
        } else {
          slt::PartialLightNode* p_partial_light_node =
            new slt::PartialLightNode(*(sub_nodes[0]->node),
                                      *(sub_nodes[1]->node),
                                      *(sub_nodes[2]->node),
                                      *(sub_nodes[3]->node),
                                      *(sub_nodes[4]->node),
                                      *(sub_nodes[5]->node),
                                      *(sub_nodes[6]->node),
                                      *(sub_nodes[7]->node));

          partial_light_nodes.push_back(p_partial_light_node);
          next_lattice->setLatticeNode(glm::uvec3(x_i, y_i, z_i),
                                       *p_partial_light_node);
        }
      }
    }
  }
}


void SLTBuilder::updateCornersLattice(const slt::Lattice& current_lattice,
                                      const slt::Offsets& offsets,
                                      slt::NoLightNode const * const no_light,
                                      slt::Lattice* next_lattice,
                                      std::vector<slt::PartialLightNode const *>& partial_light_nodes) {
  unsigned int last_current = current_lattice.getNNodes() - 1;
  unsigned int last_next = next_lattice->getNNodes() - 1;

  for (const auto& c : offsets.corners) {
    const slt::LatticeNode& corner_node = 
      current_lattice.getLatticeNode((glm::uvec3(c) * last_current));

    if (corner_node.type == slt::NodeType::NoLight) {
      next_lattice->setLatticeNode((glm::uvec3(c) * last_next),
                                   slt::LatticeNode(slt::NodeType::NoLight,
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

      slt::PartialLightNode* p_partial_light_node = new slt::PartialLightNode(new_nodes);
      partial_light_nodes.push_back(p_partial_light_node);

      next_lattice->setLatticeNode((glm::uvec3(c) * last_next),
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

void SLTBuilder::updateEdgesLattice(const slt::Lattice& current_lattice,
                                    const slt::Offsets& offsets,
                                    slt::NoLightNode const * const no_light,
                                    slt::Lattice* next_lattice,
                                    std::vector<slt::PartialLightNode const *>& partial_light_nodes) {
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
      
      const slt::LatticeNode& edge_node1 = current_lattice.getLatticeNode(sub_node);
      const slt::LatticeNode& edge_node2 = current_lattice.getLatticeNode(sub_node + direction);
      
      if (edge_node1.type == slt::NodeType::NoLight &&
          edge_node2.type == slt::NodeType::NoLight) {
        next_lattice->setLatticeNode((next_node_position),
                                     slt::LatticeNode(slt::NodeType::NoLight,
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

        slt::PartialLightNode* p_partial_light_node = new slt::PartialLightNode(new_nodes);
        partial_light_nodes.push_back(p_partial_light_node);

        next_lattice->setLatticeNode(next_node_position,
                                     *p_partial_light_node);
      }
    }
  }
}


void SLTBuilder::updateSidesLattice(const slt::Lattice& current_lattice,
                                    const slt::Offsets& offsets,
                                    slt::NoLightNode const * const no_light,
                                    slt::Lattice* next_lattice,
                                    std::vector<slt::PartialLightNode const *>& partial_light_nodes) {
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
        
        sub_node00 = glm::uvec3(glm::ivec3(sub_node_offset + d_i * (2 * i) + d_j * ( 2 * j)) - 
                                (glm::ivec3(d_i) * int(offset_dimension_i.x)) -
                                (glm::ivec3(d_j) * int(offset_dimension_j.x)));

        sub_node01 = sub_node00 + d_j;
        sub_node10 = sub_node00 + d_i;
        sub_node11 = sub_node01 + d_i;

        const slt::LatticeNode& side_node00 = current_lattice.getLatticeNode(sub_node00);
        const slt::LatticeNode& side_node01 = current_lattice.getLatticeNode(sub_node01);
        const slt::LatticeNode& side_node10 = current_lattice.getLatticeNode(sub_node10);
        const slt::LatticeNode& side_node11 = current_lattice.getLatticeNode(sub_node11);

        if (side_node00.type == slt::NodeType::NoLight &&
            side_node01.type == slt::NodeType::NoLight &&
            side_node10.type == slt::NodeType::NoLight &&
            side_node11.type == slt::NodeType::NoLight) {
          next_lattice->setLatticeNode((next_node_position),
                                       slt::LatticeNode(slt::NodeType::NoLight,
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

          slt::PartialLightNode* p_partial_light_node = new slt::PartialLightNode(new_nodes);
          partial_light_nodes.push_back(p_partial_light_node);

          next_lattice->setLatticeNode(next_node_position,
                                       *p_partial_light_node);
        }
      }
    }
  }
}


slt::Lattice* SLTBuilder::combineLatticeStep(
    const slt::Lattice& current_lattice,
    slt::NoLightNode const * const no_light,
    std::vector<slt::PartialLightNode const *>& partial_light_nodes) {

  slt::Offsets offset = this->calculateOffsets(current_lattice);

  unsigned int next_nodes = unsigned int((current_lattice.getNNodes() + offset.x.x + offset.x.y) * 0.5);

  glm::uvec3 origin = (current_lattice.getOriginInLattice() - glm::uvec3(offset.x.x, offset.y.x, offset.z.x)) / (unsigned int (2));
  slt::Lattice* p_next_lattice = new slt::Lattice(current_lattice.getOctreeOffset(),
                                                  origin,
                                                  next_nodes,
                                                  current_lattice.getDepth() + 1,
                                                  current_lattice.getMinimumNodeSize());

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

  return p_next_lattice;
}

}
}
}