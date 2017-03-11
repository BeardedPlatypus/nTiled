#include <catch.hpp>
#include "pipeline\light-management\hashed\linkless-octree\Table.h"


// ----------------------------------------------------------------------------
//  nTiled Headers
// ----------------------------------------------------------------------------
#include "pipeline\light-management\hashed\linkless-octree\Exceptions.h"


// ----------------------------------------------------------------------------
//  tableConstructor Scenarios
// ----------------------------------------------------------------------------
SCENARIO("Table.getPoint should throw a SpatialHashFunctionConstructionIllegalAccesTableException if an undefined point is provided",
         "[LinklessOctreeFull][Table]") {
  GIVEN("A set of tables filled with different amount of values") {
    nTiled::pipeline::hashed::Table<glm::u8vec2> table_5_empty =
      nTiled::pipeline::hashed::Table<glm::u8vec2>(5);

    nTiled::pipeline::hashed::Table<glm::u8vec2> table_10_empty =
      nTiled::pipeline::hashed::Table<glm::u8vec2>(10);

    // ------------------------------------------------------------------------
    nTiled::pipeline::hashed::Table<glm::u8vec2> table_5_20p =
      nTiled::pipeline::hashed::Table<glm::u8vec2>(5);

    for (unsigned int i = 0; i < 5; i++) {
      for (unsigned int j = 0; j < 5; j++) {
        table_5_20p.setPoint(glm::uvec3(i, j, 0), glm::u8vec2(0, 0));
      }
    }

    nTiled::pipeline::hashed::Table<glm::u8vec2> table_10_20p =
      nTiled::pipeline::hashed::Table<glm::u8vec2>(10);

    for (unsigned int i = 0; i < 10; i++) {
      for (unsigned int j = 0; j < 10; j++) {
        for (unsigned int k = 0; k < 2; k++) {
          table_10_20p.setPoint(glm::uvec3(i, j, k), glm::u8vec2(0, 0));
        }
      }
    }

    // ------------------------------------------------------------------------
    nTiled::pipeline::hashed::Table<glm::u8vec2> table_5_50p =
      nTiled::pipeline::hashed::Table<glm::u8vec2>(5);
    for (unsigned int i = 0; i < 5; i++) {
      for (unsigned int j = 0; j < 5; j++) {
        for (unsigned int k = 0; k < 3; k++) {
          table_5_50p.setPoint(glm::uvec3(i, j, k), glm::u8vec2(0, 0));
        }
      }
    }

    nTiled::pipeline::hashed::Table<glm::u8vec2> table_10_50p =
      nTiled::pipeline::hashed::Table<glm::u8vec2>(10);
    for (unsigned int i = 0; i < 10; i++) {
      for (unsigned int j = 0; j < 10; j++) {
        for (unsigned int k = 0; k < 5; k++) {
          table_10_50p.setPoint(glm::uvec3(i, j, k), glm::u8vec2(0, 0));
        }
      }
    }

    // ------------------------------------------------------------------------
    WHEN("A point is retrieved that is not defined") {
      THEN("A SpatialHashFunctionConstructionIllegalAccessTable is thrown") {
        for (unsigned int i = 0; i < 5; i++) {
          for (unsigned int j = 0; j < 5; j++) {
            for (unsigned int k = 1; k < 5; k++) {
              REQUIRE_THROWS_AS(table_5_empty.getPoint(glm::uvec3(i, j, k)),
                                nTiled::pipeline::hashed::SpatialHashFunctionConstructionIllegalAccessTableException);
            }
          }
        }

        for (unsigned int i = 0; i < 10; i++) {
          for (unsigned int j = 0; j < 10; j++) {
            for (unsigned int k = 2; k < 10; k++) {
              REQUIRE_THROWS_AS(table_10_empty.getPoint(glm::uvec3(i, j, k)),
                                nTiled::pipeline::hashed::SpatialHashFunctionConstructionIllegalAccessTableException);
            }
          }
        }
        // ------------------------------------------------------------------------
        for (unsigned int i = 0; i < 5; i++) {
          for (unsigned int j = 0; j < 5; j++) {
            for (unsigned int k = 3; k < 5; k++) {
              REQUIRE_THROWS_AS(table_5_20p.getPoint(glm::uvec3(i, j, k)),
                                nTiled::pipeline::hashed::SpatialHashFunctionConstructionIllegalAccessTableException);
            }
          }
        }

        for (unsigned int i = 0; i < 10; i++) {
          for (unsigned int j = 0; j < 10; j++) {
            for (unsigned int k = 5; k < 10; k++) {
              REQUIRE_THROWS_AS(table_10_20p.getPoint(glm::uvec3(i, j, k)),
                                nTiled::pipeline::hashed::SpatialHashFunctionConstructionIllegalAccessTableException);
            }
          }
        }

        // ------------------------------------------------------------------------
        for (unsigned int i = 0; i < 5; i++) {
          for (unsigned int j = 0; j < 5; j++) {
            for (unsigned int k = 3; k < 5; k++) {
              REQUIRE_THROWS_AS(table_5_50p.getPoint(glm::uvec3(i, j, k)),
                                nTiled::pipeline::hashed::SpatialHashFunctionConstructionIllegalAccessTableException);
            }
          }
        }

        for (unsigned int i = 0; i < 10; i++) {
          for (unsigned int j = 0; j < 10; j++) {
            for (unsigned int k = 5; k < 10; k++) {
              REQUIRE_THROWS_AS(table_10_50p.getPoint(glm::uvec3(i, j, k)),
                                nTiled::pipeline::hashed::SpatialHashFunctionConstructionIllegalAccessTableException);
            }
          }
        }
      }
    }
  }
}


SCENARIO("Table.getPoint should retrieve the point associated with p if it is defined.",
         "[LinklessOctreeFull][Table]") {
  GIVEN("A set of tables filled with different amount of values") {
    nTiled::pipeline::hashed::Table<glm::u8vec2> table_5_1 =
      nTiled::pipeline::hashed::Table<glm::u8vec2>(5);

    table_5_1.setPoint(glm::uvec3(0, 0, 0), glm::u8vec2(10, 10));

    nTiled::pipeline::hashed::Table<glm::u8vec2> table_10_1 =
      nTiled::pipeline::hashed::Table<glm::u8vec2>(10);

    table_10_1.setPoint(glm::uvec3(0, 0, 0), glm::u8vec2(10, 10));
    // ----------------------------------------------------------------------

    nTiled::pipeline::hashed::Table<glm::u8vec2> table_5_5 =
      nTiled::pipeline::hashed::Table<glm::u8vec2>(5);

    nTiled::pipeline::hashed::Table<glm::u8vec2> table_10_5 =
      nTiled::pipeline::hashed::Table<glm::u8vec2>(10);


    std::vector<std::pair<glm::uvec3, glm::u8vec2>> points_5 = {};
    for (int i = 0; i < 5; i++) {
      points_5.push_back(std::pair<glm::uvec3, glm::u8vec2>(
        glm::uvec3(i, 0, 0), glm::u8vec2(i, i)));
    }

    for (auto& l : points_5) {
      table_5_5.setPoint(l.first, l.second);
      table_10_5.setPoint(l.first, l.second);
    }

    // ----------------------------------------------------------------------
    nTiled::pipeline::hashed::Table<glm::u8vec2> table_5_25 =
      nTiled::pipeline::hashed::Table<glm::u8vec2>(5);

    nTiled::pipeline::hashed::Table<glm::u8vec2> table_10_25 =
      nTiled::pipeline::hashed::Table<glm::u8vec2>(10);


    std::vector<std::pair<glm::uvec3, glm::u8vec2>> points_25 = {};
    for (int x = 0; x < 5; x++) {
      for (int y = 0; y < 5; y++) {
        points_25.push_back(std::pair<glm::uvec3, glm::u8vec2>(
          glm::uvec3(x, y, 0), glm::u8vec2(x, y)));
      }
    }

    for (auto& l : points_25) {
      table_5_25.setPoint(l.first, l.second);
      table_10_25.setPoint(l.first, l.second);
    }

    // ----------------------------------------------------------------------
    nTiled::pipeline::hashed::Table<glm::u8vec2> table_5_125 =
      nTiled::pipeline::hashed::Table<glm::u8vec2>(5);

    nTiled::pipeline::hashed::Table<glm::u8vec2> table_10_125 =
      nTiled::pipeline::hashed::Table<glm::u8vec2>(10);

    std::vector<std::pair<glm::uvec3, glm::u8vec2>> points_125 = {};
    for (int x = 0; x < 5; x++) {
      for (int y = 0; y < 5; y++) {
        for (int z = 0; z < 5; z++) {
          points_125.push_back(std::pair<glm::uvec3, glm::u8vec2>(
            glm::uvec3(x, y, z), glm::u8vec2(x + 5 * z, y + 5 * z)));
        }
      }
    }

    for (auto& l : points_125) {
      table_5_125.setPoint(l.first, l.second);
      table_10_125.setPoint(l.first, l.second);
    }

    // ------------------------------------------------------------------------
    WHEN("A point is retrieved that is defined") {
      THEN("The correct value is returned") {
        for (std::pair<glm::uvec3, glm::u8vec2> l : points_5) {
          glm::u8vec2 p_5 = table_5_5.getPoint(l.first);
          glm::u8vec2 p_10 = table_10_5.getPoint(l.first);
          REQUIRE(p_5.x == l.second.x);
          REQUIRE(p_5.y == l.second.y);
          REQUIRE(p_10.x == l.second.x);
          REQUIRE(p_10.y == l.second.y);
        }

        for (std::pair<glm::uvec3, glm::u8vec2> l : points_25) {
          glm::u8vec2 p_5 = table_5_25.getPoint(l.first);
          glm::u8vec2 p_10 = table_10_25.getPoint(l.first);
          REQUIRE(p_5.x == l.second.x);
          REQUIRE(p_5.y == l.second.y);
          REQUIRE(p_10.x == l.second.x);
          REQUIRE(p_10.y == l.second.y);
        }

        for (std::pair<glm::uvec3, glm::u8vec2> l : points_125) {
          glm::u8vec2 p_5 = table_5_125.getPoint(l.first);
          glm::u8vec2 p_10 = table_10_125.getPoint(l.first);
          REQUIRE(p_5.x == l.second.x);
          REQUIRE(p_5.y == l.second.y);
          REQUIRE(p_10.x == l.second.x);
          REQUIRE(p_10.y == l.second.y);
        }
      }
    }
  }
}