#include <catch.hpp>
#include "pipeline\light-management\hashed\linkless-octree\SpatialHashFunction.h"

#include "pipeline\light-management\hashed\linkless-octree\Exceptions.h"
#include <random>

// ----------------------------------------------------------------------------
//  getData Scenarios
// ----------------------------------------------------------------------------
SCENARIO("getData should throw a SpatialHashFunctionException if the access variants are violated.",
         "[LinklessOctreeFull][SpatialHashFunctionFull][SpatialHashFunction]") {
  GIVEN("An empty SpatialHashFunction") {
    nTiled::pipeline::hashed::Table<glm::u8vec2>* hash_table_1=
      new nTiled::pipeline::hashed::Table<glm::u8vec2>(10);

    nTiled::pipeline::hashed::Table<glm::u8vec3>* offset_table_1 =
      new nTiled::pipeline::hashed::Table<glm::u8vec3>(3);

    nTiled::pipeline::hashed::SpatialHashFunction<glm::u8vec2> hash_function_empty_1 =
      nTiled::pipeline::hashed::SpatialHashFunction<glm::u8vec2>(hash_table_1,
                                                                 offset_table_1);

    nTiled::pipeline::hashed::Table<glm::u8vec2>* hash_table_2=
      new nTiled::pipeline::hashed::Table<glm::u8vec2>(15);

    nTiled::pipeline::hashed::Table<glm::u8vec3>* offset_table_2 =
      new nTiled::pipeline::hashed::Table<glm::u8vec3>(5);

    nTiled::pipeline::hashed::SpatialHashFunction<glm::u8vec2> hash_function_empty_2 =
      nTiled::pipeline::hashed::SpatialHashFunction<glm::u8vec2>(hash_table_2,
                                                                 offset_table_2);

    nTiled::pipeline::hashed::Table<glm::u8vec2>* hash_table_3 =
      new nTiled::pipeline::hashed::Table<glm::u8vec2>(5);

    nTiled::pipeline::hashed::Table<glm::u8vec3>* offset_table_3 =
      new nTiled::pipeline::hashed::Table<glm::u8vec3>(2);

    nTiled::pipeline::hashed::SpatialHashFunction<glm::u8vec2> hash_function_empty_3 =
      nTiled::pipeline::hashed::SpatialHashFunction<glm::u8vec2>(hash_table_3,
                                                                 offset_table_3);

    WHEN("any point is provided for access") {
      THEN("SpatialHashFunctionIllegalAccessException is thrown.") {
        REQUIRE_THROWS_AS(hash_function_empty_1.getData(glm::uvec3(0, 0, 0)),
                          nTiled::pipeline::hashed::SpatialHashFunctionIllegalAccessException);
        REQUIRE_THROWS_AS(hash_function_empty_1.getData(glm::uvec3(5, 10, 9)),
                          nTiled::pipeline::hashed::SpatialHashFunctionIllegalAccessException);
        REQUIRE_THROWS_AS(hash_function_empty_1.getData(glm::uvec3(305, 1, 900)),
                          nTiled::pipeline::hashed::SpatialHashFunctionIllegalAccessException);

        REQUIRE_THROWS_AS(hash_function_empty_2.getData(glm::uvec3(0, 0, 0)),
                          nTiled::pipeline::hashed::SpatialHashFunctionIllegalAccessException);
        REQUIRE_THROWS_AS(hash_function_empty_2.getData(glm::uvec3(5, 10, 9)),
                          nTiled::pipeline::hashed::SpatialHashFunctionIllegalAccessException);
        REQUIRE_THROWS_AS(hash_function_empty_2.getData(glm::uvec3(305, 1, 900)),
                          nTiled::pipeline::hashed::SpatialHashFunctionIllegalAccessException);

        REQUIRE_THROWS_AS(hash_function_empty_3.getData(glm::uvec3(0, 0, 0)),
                          nTiled::pipeline::hashed::SpatialHashFunctionIllegalAccessException);
        REQUIRE_THROWS_AS(hash_function_empty_3.getData(glm::uvec3(5, 10, 9)),
                          nTiled::pipeline::hashed::SpatialHashFunctionIllegalAccessException);
        REQUIRE_THROWS_AS(hash_function_empty_3.getData(glm::uvec3(305, 1, 900)),
                          nTiled::pipeline::hashed::SpatialHashFunctionIllegalAccessException);
      }
    }
  }

  GIVEN("A SpatialHashFunction with m = 10, r=3, and several defined points") {
    nTiled::pipeline::hashed::Table<glm::u8vec2>* hash_table =
      new nTiled::pipeline::hashed::Table<glm::u8vec2>(10);

    nTiled::pipeline::hashed::Table<glm::u8vec3>* offset_table =
      new nTiled::pipeline::hashed::Table<glm::u8vec3>(3);

    // generate 16 random points
    std::mt19937 gen = std::mt19937(31);
    std::uniform_int_distribution<unsigned short> distribution_u8vec3 =
      std::uniform_int_distribution<unsigned short>(0, 255);

    std::uniform_int_distribution<unsigned int> distribution_uvec3 =
      std::uniform_int_distribution<unsigned int>(0, 1000000);

    std::vector<glm::uvec3> used = {};

    std::vector<std::pair<glm::uvec3, glm::uvec3>> used_hashes = {};

    glm::uvec3 loc;
    glm::uvec3 loc_total;
    glm::uvec3 h_0;
    glm::uvec3 h_1;

    glm::u8vec3 offset;

    glm::u8vec2 data;
    bool found_value;
    bool found_candidate;

    for (unsigned int i = 0; i < 16; ++i) {
      // generate data
      data = glm::u8vec2(distribution_u8vec3(gen),
                         distribution_u8vec3(gen));
      // find unique location
      found_value = false;
      while (!found_value) {
        loc = glm::uvec3(distribution_uvec3(gen),
                         distribution_uvec3(gen),
                         distribution_uvec3(gen));
        h_0 = glm::uvec3(loc.x % 10,
                         loc.y % 10,
                         loc.z % 10);
        h_1 = glm::uvec3(loc.x % 3,
                         loc.y % 3,
                         loc.z % 3);

        found_value = true;
        for (const std::pair<glm::uvec3, glm::uvec3>& hashes : used_hashes) {
          if ((hashes.first.x == h_0.x) &&
              (hashes.first.y == h_0.y) &&
              (hashes.first.z == h_0.z) &&
              (hashes.second.x == h_1.x) &&
              (hashes.second.y == h_1.y) &&
              (hashes.second.z == h_1.z)) {
            found_value = false;
          }
        }

        if (offset_table->isDefined(h_1)) {
          offset = offset_table->getPoint(h_1);
          
          loc_total = glm::uvec3((h_0.x + offset.x) % 10,
                                 (h_0.y + offset.y) % 10,
                                 (h_0.z + offset.z) % 10);

          if (hash_table->isDefined(loc_total)) {
            found_value = false;
          }
        } else {
          while (true) {
            offset = glm::u8vec3(distribution_u8vec3(gen),
                                 distribution_u8vec3(gen),
                                 distribution_u8vec3(gen));

            loc_total = glm::uvec3((h_0.x + offset.x) % 10,
                                   (h_0.y + offset.y) % 10,
                                   (h_0.z + offset.z) % 10);
            if (!hash_table->isDefined(loc_total)) {
              offset_table->setPoint(h_1, offset);
              break;
            }
          }
        }
      }

      used.push_back(loc);
      used_hashes.push_back(std::pair<glm::uvec3, glm::uvec3>(h_0, h_1));
      hash_table->setPoint(loc_total, data);
    }

    nTiled::pipeline::hashed::SpatialHashFunction<glm::u8vec2> hash_function =
      nTiled::pipeline::hashed::SpatialHashFunction<glm::u8vec2>(hash_table,
                                                                 offset_table);
    WHEN("Any point not defined is accessed") {
      THEN("SpatialHashFunctionIllegalAccessException is thrown.") {
        for (int i = 0; i < 10; i++) {
          while (true) {
            loc = glm::uvec3(distribution_uvec3(gen),
                             distribution_uvec3(gen),
                             distribution_uvec3(gen));

            if (std::find(used.begin(),
                          used.end(),
                          loc) == used.end()) {
              break;
            }
          }
          REQUIRE_THROWS_AS(hash_function.getData(loc),
                          nTiled::pipeline::hashed::SpatialHashFunctionIllegalAccessException);

        }
      }
    }
  }
}

SCENARIO("getData should return the associated data with a point, if this point is provided",
         "[LinklessOctreeFull][SpatialHashFunctionFull][SpatialHashFunction]") {
  GIVEN("A SpatialHashFunction with m = 10, r=3 and several defined points") {
    nTiled::pipeline::hashed::Table<glm::u8vec2>* hash_table =
      new nTiled::pipeline::hashed::Table<glm::u8vec2>(10);

    nTiled::pipeline::hashed::Table<glm::u8vec3>* offset_table =
      new nTiled::pipeline::hashed::Table<glm::u8vec3>(3);

    // generate 16 random points
    std::mt19937 gen = std::mt19937(31);
    std::uniform_int_distribution<unsigned short> distribution_u8vec3 =
      std::uniform_int_distribution<unsigned short>(0, 255);

    std::uniform_int_distribution<unsigned int> distribution_uvec3 =
      std::uniform_int_distribution<unsigned int>(0, 1000000);

    std::vector<std::pair<glm::uvec3, glm::u8vec2>> used = {};

    std::vector<std::pair<glm::uvec3, glm::uvec3>> used_hashes = {};

    glm::uvec3 loc;
    glm::uvec3 loc_total;
    glm::uvec3 h_0;
    glm::uvec3 h_1;

    glm::u8vec3 offset;

    glm::u8vec2 data;
    bool found_value;

    for (unsigned int i = 0; i < 16; ++i) {
      // generate data
      data = glm::u8vec2(distribution_u8vec3(gen),
                         distribution_u8vec3(gen));
      // find unique location
      found_value = false;
      while (!found_value) {
        loc = glm::uvec3(distribution_uvec3(gen),
                         distribution_uvec3(gen),
                         distribution_uvec3(gen));
        h_0 = glm::uvec3(loc.x % 10,
                         loc.y % 10,
                         loc.z % 10);
        h_1 = glm::uvec3(loc.x % 3,
                         loc.y % 3,
                         loc.z % 3);

        found_value = true;
        for (const std::pair<glm::uvec3, glm::uvec3>& hashes : used_hashes) {
          if ((hashes.first.x == h_0.x) &&
              (hashes.first.y == h_0.y) &&
              (hashes.first.z == h_0.z) &&
              (hashes.second.x == h_1.x) &&
              (hashes.second.y == h_1.y) &&
              (hashes.second.z == h_1.z)) {
            found_value = false;
          }
        }

        if (offset_table->isDefined(h_1)) {
          offset = offset_table->getPoint(h_1);
          
          loc_total = glm::uvec3((h_0.x + offset.x) % 10,
                                 (h_0.y + offset.y) % 10,
                                 (h_0.z + offset.z) % 10);

          if (hash_table->isDefined(loc_total)) {
            found_value = false;
          }
        } else {
          while (true) {
            offset = glm::u8vec3(distribution_u8vec3(gen),
                                 distribution_u8vec3(gen),
                                 distribution_u8vec3(gen));

            loc_total = glm::uvec3((h_0.x + offset.x) % 10,
                                   (h_0.y + offset.y) % 10,
                                   (h_0.z + offset.z) % 10);
            if (!hash_table->isDefined(loc_total)) {
              offset_table->setPoint(h_1, offset);
              break;
            }
          }
        }
      }

      used.push_back(std::pair<glm::uvec3, glm::u8vec2>(loc, data));
      used_hashes.push_back(std::pair<glm::uvec3, glm::uvec3>(h_0, h_1));
      hash_table->setPoint(loc_total, data);
    }

    nTiled::pipeline::hashed::SpatialHashFunction<glm::u8vec2> hash_function =
      nTiled::pipeline::hashed::SpatialHashFunction<glm::u8vec2>(hash_table,
                                                                 offset_table);
    WHEN("Any of these points are accessed") {
      THEN("The correct associated data should be returned") {
        for (const std::pair<glm::uvec3, glm::u8vec2>& p : used) {
          glm::u8vec2 val_spatial = hash_function.getData(p.first);
          glm::u8vec2 val_entered = p.second;

          REQUIRE(val_spatial.x == val_entered.x);
          REQUIRE(val_spatial.y == val_entered.y);
        }
      }
    }
  }
}
