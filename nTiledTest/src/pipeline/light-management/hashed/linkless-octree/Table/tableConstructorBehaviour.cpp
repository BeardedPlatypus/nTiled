#include <catch.hpp>
#include "pipeline\light-management\hashed\linkless-octree\Table.h"


// ----------------------------------------------------------------------------
//  nTiled Headers
// ----------------------------------------------------------------------------
#include "pipeline\light-management\hashed\linkless-octree\Exceptions.h"


// ----------------------------------------------------------------------------
//  tableConstructor Scenarios
// ----------------------------------------------------------------------------
SCENARIO("tableConstructor should throw a SpatialHashFunctionConstructionInvalidArgException if an invalid argument is provided",
         "[LinklessOctreeFull][Table]") {
  GIVEN("The definition of a Table<R>") {
    WHEN("t_dim is zero") {
      THEN("A SpatialHashFunctionConstructionInvalidArgException is thrown") {
        REQUIRE_THROWS_AS(nTiled::pipeline::hashed::Table<glm::u8vec2>(0),
                          nTiled::pipeline::hashed::SpatialHashFunctionConstructionInvalidArgException);
      }
    }
  }
}


SCENARIO("tableConstructor should return a valid empty Table with valid arguments.",
         "[LinklessOctreeFull][Table]") {
  GIVEN("The definition of a Table<R>") {
    WHEN("A valid t_dim is provided") {
      THEN("A valid empty Table should be constructed") {
        for (unsigned int i = 1; i < 5; i++) {
          nTiled::pipeline::hashed::Table<glm::u8vec2> table =
            nTiled::pipeline::hashed::Table<glm::u8vec2>(i);

          REQUIRE(table.getDim() == i);
          
          for (unsigned int x = 0; x < i; x++) {
            for (unsigned int y = 0; y < i; y++) {
              for (unsigned int z = 0; z < i; z++) {
                REQUIRE_FALSE(table.isDefined(glm::uvec3(x, y, z)));
              }
            }
          }
        }
      }
    }
  }
}
