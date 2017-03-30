#include <catch.hpp>
#include "pipeline\light-management\hashed\light-octree\nodes\LOBranch.h"


SCENARIO("The constructor should return the appropriate LOBranch when provided with correct data",
         "[LightOctreeFull][LightOctree][LOBranch]") {
  GIVEN("") {
    WHEN("An empty LOBranch node is created") {
      nTiled::pipeline::hashed::LOBranch branch =
        nTiled::pipeline::hashed::LOBranch();

      THEN("It should not contain any light indices") {
        for (unsigned int x = 0; x < 2; ++x) {
          for (unsigned int y = 0; y < 2; ++y) {
            for (unsigned int z = 0; z < 2; ++z) {
              REQUIRE(branch.getChildNode(glm::bvec3(x == 1,
                                                     y == 1,
                                                     z == 1))->retrieveLights(glm::vec3(0.25 + 0.5 * x,
                                                                                        0.25 + 0.5 * y,
                                                                                        0.25 + 0.5 * z),
                                                                              nTiled::pipeline::hashed::NodeDimensions(glm::vec3(0.0), 1.0)).empty());
            }
          }
        }
      }
    }
  }

  // TODO
}
