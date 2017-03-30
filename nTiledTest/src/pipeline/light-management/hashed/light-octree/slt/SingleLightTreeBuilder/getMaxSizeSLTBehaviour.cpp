#include <catch.hpp>
#include "pipeline\light-management\hashed\light-octree\slt\SingleLightTreeBuilder.h"
#include "pipeline\light-management\hashed\light-octree\slt\Exceptions.h"


// ----------------------------------------------------------------------------
//  getMaxSizeSLTBehaviour
// ----------------------------------------------------------------------------
SCENARIO("getMaxSLTBehaviour should throw a SLTException when the light does not fit within the octree.",
         "[LightOctreeFull][SLTFull][SLTBuilder]") {
  GIVEN("A SLTBuilder") {
    nTiled::pipeline::hashed::SingleLightTreeBuilder builder =
      nTiled::pipeline::hashed::SingleLightTreeBuilder(0.5,
                                                       glm::vec3(0.0, 0.0, 0.0));

    WHEN("A light is provided that is smaller than the origin of the octree") {
      std::string name = "just_testing_things";
      glm::vec3 intensity = glm::vec3(1.0);
      std::map<std::string, nTiled::world::Object*> empty_map =
        std::map<std::string, nTiled::world::Object*>();

      nTiled::world::PointLight light_1 =
        nTiled::world::PointLight(name,
                                  glm::vec4(-1.0, 0.0, 0.0, 1.0),
                                  intensity,
                                  0.5,
                                  true,
                                  empty_map);
      nTiled::world::PointLight light_2 =
        nTiled::world::PointLight(name,
                                  glm::vec4(0.0, 0.0, -10.0, 1.0),
                                  intensity,
                                  0.5,
                                  true,
                                  empty_map);
      nTiled::world::PointLight light_3 =
        nTiled::world::PointLight(name,
                                  glm::vec4(1.0, -1.0, 0.0, 1.0),
                                  intensity,
                                  0.5,
                                  true,
                                  empty_map);

      nTiled::world::PointLight light_0 =
        nTiled::world::PointLight(name,
                                  glm::vec4(0.0, 0.0, 0.0, 1.0),
                                  intensity,
                                  0.5,
                                  true,
                                  empty_map);

      THEN("getMaxSLTBehaviour should throw a SLTNotInOctreeException") {
        REQUIRE_THROWS_AS(builder.getMaxSizeSLT(light_0),
                          nTiled::pipeline::hashed::SLTLightNotInOctreeException);
        REQUIRE_THROWS_AS(builder.getMaxSizeSLT(light_1),
                          nTiled::pipeline::hashed::SLTLightNotInOctreeException);
        REQUIRE_THROWS_AS(builder.getMaxSizeSLT(light_2),
                          nTiled::pipeline::hashed::SLTLightNotInOctreeException);
        REQUIRE_THROWS_AS(builder.getMaxSizeSLT(light_3),
                          nTiled::pipeline::hashed::SLTLightNotInOctreeException);
      }
    }
  }
}


SCENARIO("getMaxSizeSLTBehaviour should return the correct size of the octree when provided with correct values",
         "[LightOctreeFull][SLTFull][SLTBuilder]") {

  GIVEN("A SLTBuilder") {
    nTiled::pipeline::hashed::SingleLightTreeBuilder builder =
      nTiled::pipeline::hashed::SingleLightTreeBuilder(0.5,
                                                       glm::vec3(0.0, 0.0, 0.0));
    WHEN("A light is provided that is smaller than minimum_node_size") {
      std::string name = "just_testing_things";
      glm::vec3 intensity = glm::vec3(1.0);
      std::map<std::string, nTiled::world::Object*> empty_map =
        std::map<std::string, nTiled::world::Object*>();

      nTiled::world::PointLight light_0 =
        nTiled::world::PointLight(name,
                                  glm::vec4(2.2, 2.2, 2.2, 1.0),
                                  intensity,
                                  0.05,
                                  true,
                                  empty_map);
      nTiled::world::PointLight light_1 =
        nTiled::world::PointLight(name,
                                  glm::vec4(2.2, 2.2, 2.2, 1.0),
                                  intensity,
                                  0.1,
                                  true,
                                  empty_map);
      nTiled::world::PointLight light_2 =
        nTiled::world::PointLight(name,
                                  glm::vec4(2.2, 2.2, 2.2, 1.0),
                                  intensity,
                                  0.15,
                                  true,
                                  empty_map);
      nTiled::world::PointLight light_3 =
        nTiled::world::PointLight(name,
                                  glm::vec4(2.2, 2.2, 2.2, 1.0),
                                  intensity,
                                  0.2,
                                  true,
                                  empty_map);

      THEN("minimum_node_size should be returned") {
        REQUIRE(builder.getMaxSizeSLT(light_0) == 0.5);
        REQUIRE(builder.getMaxSizeSLT(light_1) == 0.5);
        REQUIRE(builder.getMaxSizeSLT(light_2) == 0.5);
        REQUIRE(builder.getMaxSizeSLT(light_3) == 0.5);
      }
    }

    WHEN("A light is provided of size 2^n * minimum_node_size that falls within a single node ") {
      std::string name = "just_testing_things";
      glm::vec3 intensity = glm::vec3(1.0);
      std::map<std::string, nTiled::world::Object*> empty_map =
        std::map<std::string, nTiled::world::Object*>();

      std::vector<std::pair<unsigned int, nTiled::world::PointLight>> lights = {};

      unsigned int n_nodes;
      glm::vec4 origin;
      float radius;

      for (unsigned int i = 1; i < 6; i++) {
        n_nodes = (1 << i);
        origin = glm::vec4(glm::vec3(0.5 * (n_nodes / 2)), 1.0);
        radius = 0.5 * (n_nodes / 2) - 0.1;

        nTiled::world::PointLight light =
          nTiled::world::PointLight(name,
                                    origin,
                                    intensity,
                                    radius,
                                    true,
                                    empty_map);
        lights.push_back(std::pair<unsigned int, nTiled::world::PointLight>(n_nodes, light));
      }      

      THEN("2^n * minimum_node_size should be returned") {
        for (const std::pair<unsigned int, nTiled::world::PointLight>& l : lights) {
          REQUIRE(builder.getMaxSizeSLT(l.second) == l.first * 0.5);
        }
      }
    }

    WHEN("A light is provided of size 2^n * minimum_node_size that falls in two or more single nodes") {
      std::string name = "just_testing_things";
      glm::vec3 intensity = glm::vec3(1.0);
      std::map<std::string, nTiled::world::Object*> empty_map =
        std::map<std::string, nTiled::world::Object*>();

      std::vector<std::pair<unsigned int, nTiled::world::PointLight>> lights = {};

      unsigned int n_nodes;
      glm::vec4 origin;
      float radius;

      for (unsigned int i = 1; i < 6; i++) {
        n_nodes = (1 << i);
        origin = glm::vec4(glm::vec3((n_nodes / 2)), 1.0);
        radius = 0.5 * (n_nodes / 2) - 0.1;

        nTiled::world::PointLight light =
          nTiled::world::PointLight(name,
                                    origin,
                                    intensity,
                                    radius,
                                    true,
                                    empty_map);
        lights.push_back(std::pair<unsigned int, nTiled::world::PointLight>(n_nodes, light));
      }      

      THEN("2^n+1 * minimum_node_size should be returned") {
        for (const std::pair<unsigned int, nTiled::world::PointLight>& l : lights) {
          REQUIRE(builder.getMaxSizeSLT(l.second) == l.first * 0.5 * 2);
        }
      }
    }
  }
}