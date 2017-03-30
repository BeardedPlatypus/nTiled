#include <catch.hpp>
#include "pipeline\light-management\hashed\light-octree\slt\SingleLightTreeBuilder.h"

// ----------------------------------------------------------------------------
//  nTiled Header
// ----------------------------------------------------------------------------
#include "math\points.h"

// ----------------------------------------------------------------------------
//  determineNodeType behaviour
// ----------------------------------------------------------------------------
SCENARIO("determineNodeType behaviour should return the type of node with the provided dimensions is, given a lattice",
         "[LightOctreeFull][SLTFull][SLTBuilder][determineNodeType]") {
  GIVEN("A SingleLightTreeBuilder and a series of lattices") {
    nTiled::pipeline::hashed::SingleLightTreeBuilder builder =
      nTiled::pipeline::hashed::SingleLightTreeBuilder(0.5,
                                                       glm::vec3(5.0, 3.0, 1.0));

    std::string name = "just_testing_things";
    glm::vec3 intensity = glm::vec3(1.0);
    std::map<std::string, nTiled::world::Object*> empty_map =
      std::map<std::string, nTiled::world::Object*>();

    std::vector<nTiled::world::PointLight> lights = {};
    std::vector<nTiled::pipeline::hashed::Lattice*> lattices = {};
    std::vector<unsigned int> indices = {};

    glm::vec3 light_position;
    double radius;
    for (unsigned int i = 1; i <= 20; ++i) {
      indices.push_back(i);

      radius = 0.5 * builder.getMinimalNodeSize() * i - 0.1;
      light_position = builder.getOriginOctree() + glm::vec3(0.5 * builder.getMinimalNodeSize() * i);

      lights.push_back(nTiled::world::PointLight(name,
                                                 glm::vec4(light_position, 1.0),
                                                 intensity,
                                                 radius,
                                                 true,
                                                 empty_map));

      lattices.push_back(builder.constructLattice(lights.back()));
    }

    WHEN("Current node does not intersect with a lattice") {
      std::vector<glm::vec3> points = { glm::vec3(3.0, 3.0, 3.0),
                                        glm::vec3(3.0, 0.0, 0.0),
                                        glm::vec3(0.0, 3.0, 0.0),
                                        glm::vec3(0.0, 0.0, 3.0) };

      glm::vec3 n_orig_1;
      glm::vec3 n_orig_2;
      THEN("determineNodeType should return NodeType::Empty") {
        for (unsigned int i = 0; i < 20; i++) {
          for (glm::vec3 p : points) {
            n_orig_1 = lattices.at(i)->getOrigin() - p;
            n_orig_2 = lattices.at(i)->getOrigin() + p + glm::vec3(lattices.at(i)->getWidth());

            REQUIRE(builder.determineNodeType(n_orig_1, 2.0, *lattices.at(i)) == nTiled::pipeline::hashed::NodeType::Empty);
            REQUIRE(builder.determineNodeType(n_orig_2, 2.0, *lattices.at(i)) == nTiled::pipeline::hashed::NodeType::Empty);
          }
        }
      }
    }

    WHEN("Current node partly intersects with a lattice") {
      glm::vec3 offset_origin = glm::vec3(2.0, 2.0, 2.0);

      glm::vec3 node_origin;
      nTiled::pipeline::hashed::Lattice* lat;
      double size;
      nTiled::pipeline::hashed::NodeType value;
      nTiled::pipeline::hashed::NodeType result;
      THEN("determineNodeType should return NodeType::Empty if the closest node to the origin is empty, NodeType::Partial otherwise") {
        for (unsigned int i = 0; i < 20; i++) {
          lat = lattices.at(i);
          for (unsigned int j = 1; j < indices.at(i); j++) {
            size = 2.0 + j * lat->getNodeSize();
            node_origin = lat->getOrigin() - offset_origin;

            result = builder.determineNodeType(node_origin, 
                                               size,
                                               *lat);
            if ((lat->getNode(lat->getClosestNodeToLightSource(node_origin + glm::vec3(0.1 * lat->getNodeSize()),
                                                               size - 0.2 * lat->getNodeSize())) == 
                nTiled::pipeline::hashed::NodeType::Empty)) {
              value = nTiled::pipeline::hashed::NodeType::Empty;
            } else {
              value = nTiled::pipeline::hashed::NodeType::Partial;
            }
            REQUIRE(result == value);
          }
        }
      }
    }

    WHEN("Current node is within the lattice") {
      glm::vec3 origin;
      glm::vec3 size;


      double node_size;
      unsigned int n_pos_nodes;
      nTiled::pipeline::hashed::Lattice* lat;

      bool has_tested;
      nTiled::pipeline::hashed::NodeType result;
      nTiled::pipeline::hashed::NodeType node_type;
      THEN("determineNodeType should return NodeType::Filled if all nodes are Filled, Empty if all nodes are empty, Partial Otherwise") {
        for (unsigned int i = 0; i < 20; i++) {
          lat = lattices.at(i);
          node_size = lat->getNodeSize();

          // ------------------------------------------------------------------
          for (unsigned int x_i = 0; x_i < indices.at(i); x_i++) {
            for (unsigned int y_i = 0; y_i < indices.at(i); y_i++) {
              for (unsigned int z_i = 0; z_i < indices.at(i); z_i++) {
              // --------------------------------------------------------------
                n_pos_nodes = indices.at(i) - (nTiled::math::f_max(nTiled::math::f_max(z_i, y_i), x_i));

                origin = lat->getOrigin() + glm::vec3(x_i * node_size,
                                                      y_i * node_size,
                                                      z_i * node_size);
                node_type = lat->getNode(glm::uvec3(x_i, y_i, z_i));

                for (unsigned int size_i = 1; size_i <= n_pos_nodes; size_i++) {
                  result = builder.determineNodeType(origin, node_size * size_i, *lat);

                  has_tested = false;
                  
                  // --------------------------------------------------
                  for (unsigned int x_l = 0; x_l < size_i; x_l++) {
                    for (unsigned int y_l = 0; y_l < size_i; y_l++) {
                      for (unsigned int z_l = 0; z_l < size_i; z_l++) {
                      // ----------------------------------------------

                        if (node_type != lat->getNode(glm::uvec3(x_i + x_l,
                                                                 y_i + y_l,
                                                                 z_i + z_l))) {
                          REQUIRE(result == nTiled::pipeline::hashed::NodeType::Partial);
                          has_tested = true;
                          break;
                        }
                      }
                      if (has_tested) break;
                    }
                    if (has_tested) break;
                  }

                  if (!has_tested) {
                    REQUIRE(result == node_type);
                  }
                }
              }
            }
          }
        }
      }
    }
  }
}
