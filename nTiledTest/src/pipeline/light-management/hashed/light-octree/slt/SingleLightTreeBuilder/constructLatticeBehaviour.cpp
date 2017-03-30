#include <catch.hpp>
#include "pipeline\light-management\hashed\light-octree\slt\SingleLightTreeBuilder.h"
#include "pipeline\light-management\hashed\light-octree\slt\Exceptions.h"
#include "math\octree.h"


// ----------------------------------------------------------------------------
//  constructLatticeBehaviour
// ----------------------------------------------------------------------------
SCENARIO("constructLattice should throw a SLTException when the invariants are violated",
         "[LightOctreeFull][SLTFull][SLTBuilder]") {
  GIVEN("A SLTBuilder") {
    nTiled::pipeline::hashed::SingleLightTreeBuilder builder =
      nTiled::pipeline::hashed::SingleLightTreeBuilder(0.5,
                                                       glm::vec3(0.0, 0.0, 0.0));

    WHEN("a overlapping with a section smaller than the origin of the octree is provided") {
      std::string name = "just_testing_things";
      glm::vec3 intensity = glm::vec3(1.0);
      std::map<std::string, nTiled::world::Object*> empty_map =
        std::map<std::string, nTiled::world::Object*>();

      nTiled::world::PointLight light_1 =
        nTiled::world::PointLight(name,
                                  glm::vec4(0.0, 0.0, 0.0, 1.0),
                                  intensity,
                                  0.5,
                                  true,
                                  empty_map);
      nTiled::world::PointLight light_2 = 
        nTiled::world::PointLight(name,
                                  glm::vec4(-2.0, 2.0, 2.0, 1.0),
                                  intensity,
                                  0.5,
                                  true,
                                  empty_map);
      nTiled::world::PointLight light_3 =
        nTiled::world::PointLight(name,
                                  glm::vec4(-2.0, -2.0, 2.0, 1.0),
                                  intensity,
                                  0.5,
                                  true,
                                  empty_map);
      nTiled::world::PointLight light_4 =
        nTiled::world::PointLight(name,
                                  glm::vec4(-2.0, -2.0, -2.0, 1.0),
                                  intensity,
                                  0.5,
                                  true,
                                  empty_map);

      THEN("A SLTLightNotInOctreeException should be thrown") {
        REQUIRE_THROWS_AS(builder.constructLattice(light_1),
                          nTiled::pipeline::hashed::SLTLightNotInOctreeException);
        REQUIRE_THROWS_AS(builder.constructLattice(light_2),
                          nTiled::pipeline::hashed::SLTLightNotInOctreeException);
        REQUIRE_THROWS_AS(builder.constructLattice(light_3),
                          nTiled::pipeline::hashed::SLTLightNotInOctreeException);
        REQUIRE_THROWS_AS(builder.constructLattice(light_4),
                          nTiled::pipeline::hashed::SLTLightNotInOctreeException);

      }
    }
  }
}


SCENARIO("constructLattice should construct a valid Lattice when provided with correct values",
         "[LightOctreeFull][SLTFull][SLTBuilder]") {
  GIVEN("A SLTBuilder, a full light node and a no light node") {
    float slt_size = 0.5;
    nTiled::pipeline::hashed::SingleLightTreeBuilder builder =
      nTiled::pipeline::hashed::SingleLightTreeBuilder(slt_size,
                                                       glm::vec3(0.0, 0.0, 0.0));

    WHEN("a light smaller than the minimal node size is provided") {
      std::string name = "just_testing_things";
      glm::vec3 intensity = glm::vec3(1.0);
      std::map<std::string, nTiled::world::Object*> empty_map =
        std::map<std::string, nTiled::world::Object*>();

      nTiled::world::PointLight small_light_1 =
        nTiled::world::PointLight(name,
                                  glm::vec4(1.7, 1.7, 1.8, 1.0),
                                  intensity,
                                  0.1,
                                  true,
                                  empty_map);
      nTiled::world::PointLight small_light_2 =
        nTiled::world::PointLight(name,
                                  glm::vec4(1.75, 5.25, 1.25, 1.0),
                                  intensity,
                                  0.2,
                                  true,
                                  empty_map);
      THEN("a layer containing a single full light node should be returned") {
        nTiled::pipeline::hashed::Lattice* layer_1 =
          builder.constructLattice(small_light_1);

        REQUIRE(layer_1->getNumberNodesDim() == 1);
        REQUIRE(layer_1->getNodeSize() == 0.5);

        glm::vec3 origin_1 = layer_1->getOrigin();
        REQUIRE(origin_1.x == 3 * 0.5);
        REQUIRE(origin_1.y == 3 * 0.5);
        REQUIRE(origin_1.z == 3 * 0.5);

        REQUIRE(layer_1->getNode(glm::uvec3(0, 0, 0)) == nTiled::pipeline::hashed::NodeType::Filled);
        delete layer_1;

        // ------------------------------------------------------------------------------------------------------------
        nTiled::pipeline::hashed::Lattice* layer_2 =
          builder.constructLattice(small_light_2);

        REQUIRE(layer_2->getNumberNodesDim() == 1);
        REQUIRE(layer_2->getNodeSize() == 0.5);

        glm::vec3 origin_2 = layer_2->getOrigin();
        REQUIRE(origin_2.x ==  3 * 0.5);
        REQUIRE(origin_2.y == 10 * 0.5);
        REQUIRE(origin_2.z ==  2 * 0.5);

        REQUIRE(layer_2->getNode(glm::uvec3(0, 0, 0)) == nTiled::pipeline::hashed::NodeType::Filled);
        delete layer_2;
      }
    }

    WHEN("a light greater than the minimal node is provided") {
      std::string name = "just_testing_things";
      glm::vec3 intensity = glm::vec3(1.0);
      std::map<std::string, nTiled::world::Object*> empty_map =
        std::map<std::string, nTiled::world::Object*>();

      std::vector<std::pair<unsigned int, nTiled::world::PointLight>> lights = {};

      unsigned int n_nodes;
      glm::vec4 origin;
      float radius;
      for (unsigned int i = 1; i < 7; i++) {
        // construct a light with 2^i number of cells in each direction
        n_nodes = (1 << i);
        origin = glm::vec4(glm::vec3(slt_size * (n_nodes / 2)), 1.0);
        radius = slt_size * (n_nodes / 2) - 0.2;

        nTiled::world::PointLight light =
          nTiled::world::PointLight(name,
                                    origin,
                                    intensity,
                                    radius,
                                    true,
                                    empty_map);
        lights.push_back(std::pair<unsigned int, nTiled::world::PointLight>(i, light));
      }

      THEN("a layer containing multiple cells should be returned") {
        nTiled::pipeline::hashed::Lattice* p_layer;

        for (const std::pair<unsigned int, nTiled::world::PointLight>& l : lights) {
          p_layer = builder.constructLattice(l.second);

          n_nodes = (1 << l.first);

          REQUIRE(p_layer->getNumberNodesDim() == n_nodes);
          REQUIRE(p_layer->getNodeSize() == slt_size);

          glm::vec3 u_origin = p_layer->getOrigin();
          REQUIRE(u_origin.x == 0.0);
          REQUIRE(u_origin.y == 0.0);
          REQUIRE(u_origin.z == 0.0);

          for (unsigned int x_i = 0; x_i < n_nodes; x_i++) {
            for (unsigned int y_i = 0; y_i < n_nodes; y_i++) {
              for (unsigned int z_i = 0; z_i < n_nodes; z_i++) {
                nTiled::pipeline::hashed::NodeType t = p_layer->getNode(glm::uvec3(x_i, y_i, z_i));

                if (builder.nodeWithinLight(l.second,
                                            glm::vec3(x_i * slt_size, y_i * slt_size, z_i * slt_size),
                                            slt_size)) {
                  REQUIRE(t == nTiled::pipeline::hashed::NodeType::Filled);
                } else {
                  REQUIRE(t == nTiled::pipeline::hashed::NodeType::Empty);
                }
              }
            }
          }
          delete p_layer;
        }
      }
    }
  }
}
