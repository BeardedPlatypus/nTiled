#include <catch.hpp>
#include "pipeline\light-management\hashed\light-octree\slt\SingleLightTreeBuilder.h"
#include "pipeline\light-management\hashed\light-octree\slt\Exceptions.h"

#include "math\points.h"

// ----------------------------------------------------------------------------
//  constructSLTBehaviour
// ----------------------------------------------------------------------------
SCENARIO("constructSLT should throw a SLTException when the invariants are violated",
         "[LightOctreeFull][SLTFull][SLTBuilder][constructSLT]") {
  GIVEN("A SLTBuilder") {
    nTiled::pipeline::hashed::SingleLightTreeBuilder builder =
      nTiled::pipeline::hashed::SingleLightTreeBuilder(0.5,
                                                       glm::vec3(7.0, 4.0, -10.0));

    WHEN("A light does not fit in the Octree") {
      std::string name = "just_testing_things";
      glm::vec3 intensity = glm::vec3(1.0f);
      std::map<std::string, nTiled::world::Object*> empty_map =
        std::map<std::string, nTiled::world::Object*>();

      nTiled::world::PointLight light_1 =
        nTiled::world::PointLight(name,
                                  glm::vec4(0.0, 0.0, 0.0, 1.0),
                                  intensity,
                                  11,
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
                                  glm::vec4(6.0, 10.0, 0.0, 1.0),
                                  intensity,
                                  0.5,
                                  true,
                                  empty_map);

      nTiled::world::PointLight light_0 =
        nTiled::world::PointLight(name,
                                  glm::vec4(-10.0, -10.0, -10.0, 1.0),
                                  intensity,
                                  0.5,
                                  true,
                                  empty_map);

      THEN("A SLTLightNotInOctreeException should be thrown") {
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


SCENARIO("constructSLT should return a valid SingleLightTree when a valid light is provided",
         "[LightOctreeFull][SLTFull][SLTBuilder][constructSLT]") {
  GIVEN("A SLTBuilder and a set of lights") {
    nTiled::pipeline::hashed::SingleLightTreeBuilder builder =
      nTiled::pipeline::hashed::SingleLightTreeBuilder(0.5,
                                                       glm::vec3(5.0, 3.0, 1.0));

    std::string name = "just_testing_things";
    glm::vec3 intensity = glm::vec3(1.0);
    std::map<std::string, nTiled::world::Object*> empty_map =
      std::map<std::string, nTiled::world::Object*>();

    std::vector<nTiled::world::PointLight> lights = {};
    std::vector<unsigned int> indices = {};

    glm::vec3 light_position;
    double radius;
    for (unsigned int i = 1; i <= 10; ++i) {
      indices.push_back(i);

      radius = 0.5 * builder.getMinimalNodeSize() * i - 0.1;
      light_position = builder.getOriginOctree() + glm::vec3(0.5 * builder.getMinimalNodeSize() * i);

      lights.push_back(nTiled::world::PointLight(name,
                                                 glm::vec4(light_position, 1.0),
                                                 intensity,
                                                 radius,
                                                 true,
                                                 empty_map));
    }

    WHEN("A set of SLTs is created out of these lights") {
      std::vector<nTiled::pipeline::hashed::SingleLightTree*> slts = {};

      for (const nTiled::world::PointLight& l : lights) {
        slts.push_back(builder.constructSLT(l));
      }

      THEN("Each SLT should correctly model the corresponding light") {
        for (unsigned int i = 0; i < 10; i++) {
          nTiled::pipeline::hashed::SingleLightTree* slt = slts.at(i);
          const nTiled::world::PointLight& light = lights.at(i);

          unsigned int n_steps = 3 + 3 + 3 * slt->getNNodes() + 1;
          double step_size = (slt->getMinimalNodeSize() * 2 + slt->getWidth()) / n_steps;
          double step_offset = 0.1 * slt->getMinimalNodeSize();

          glm::vec3 slt_origin = slt->getOrigin();
          double slt_width = slt->getWidth();
          double minimal_size = slt->getMinimalNodeSize();

          for (unsigned int x_i = 0; x_i < n_steps; x_i++) {
            for (unsigned int y_i = 0; y_i < n_steps; y_i++) {
              for (unsigned int z_i = 0; z_i < n_steps; z_i++) {
                glm::vec3 point = (slt->getOrigin() + glm::vec3(x_i * step_size + step_offset,
                                                                y_i * step_size + step_offset,
                                                                z_i * step_size + step_offset) -
                                   glm::vec3(slt->getMinimalNodeSize()));


                glm::vec3 node_origin_point = glm::vec3(floor((point.x - slt_origin.x) / minimal_size) * minimal_size + slt_origin.x,
                                                        floor((point.y - slt_origin.y) / minimal_size) * minimal_size + slt_origin.y,
                                                        floor((point.z - slt_origin.z) / minimal_size) * minimal_size + slt_origin.z);

                bool result_math_lid = builder.nodeWithinLight(light, node_origin_point, minimal_size);
                bool result_slt_lid = slt->isInLight(point);

                REQUIRE(result_math_lid == result_slt_lid);
              }
            }
          }
        }
      }
    }
  }
}
