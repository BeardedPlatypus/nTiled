#include <catch.hpp>
#include "pipeline\light-management\hashed\linkless-octree\SpatialHashFunctionBuilder.h"

// ----------------------------------------------------------------------------
//  nTiled Headers
// ----------------------------------------------------------------------------
#include "pipeline\light-management\hashed\linkless-octree\Exceptions.h"

// ----------------------------------------------------------------------------
//  mapEntryVector Scenarios
// ----------------------------------------------------------------------------
SCENARIO("mapEntryVectorBehaviour should throw an error when invalid arguments are provided.",
         "[LinklessOctreeFull][SpatialHashFunctionFull][SpatialHashFunctionBuilder]") {
  GIVEN("A SpatialHashFunctionBuilder") {
    nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::u8vec2> builder =
      nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::u8vec2>();

    WHEN("An empty entry set is provided") {
      std::vector<std::pair<glm::uvec3, glm::u8vec2>> empty_entries = {};
      std::vector<nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::u8vec2>::ConstructionElement> result = {};

      THEN("SpatialHashFunctionConstructionInvalidArgException should be thrown") {
        REQUIRE_THROWS_AS(builder.mapEntryVector(empty_entries, 1, 1, result),
                          nTiled::pipeline::hashed::SpatialHashFunctionConstructionInvalidArgException);
        REQUIRE_THROWS_AS(builder.mapEntryVector(empty_entries, 5, 3, result),
                          nTiled::pipeline::hashed::SpatialHashFunctionConstructionInvalidArgException);
        REQUIRE_THROWS_AS(builder.mapEntryVector(empty_entries, 20, 5, result),
                          nTiled::pipeline::hashed::SpatialHashFunctionConstructionInvalidArgException);
      }
    }

    WHEN("An m_dim value of 0 is provided") {
      std::pair<glm::uvec3, glm::u8vec2> entry_1 =
        std::pair<glm::uvec3, glm::u8vec2>(glm::uvec3(0, 0, 0),
                                           glm::u8vec2(1, 1));

      std::pair<glm::uvec3, glm::u8vec2> entry_2 =
        std::pair<glm::uvec3, glm::u8vec2>(glm::uvec3(5, 5, 5),
                                           glm::u8vec2(1, 1));

      std::pair<glm::uvec3, glm::u8vec2> entry_3 =
        std::pair<glm::uvec3, glm::u8vec2>(glm::uvec3(256, 0, 0),
                                           glm::u8vec2(1, 1));

      std::vector<std::pair<glm::uvec3, glm::u8vec2>> entries_0 = { entry_1 };
      std::vector<std::pair<glm::uvec3, glm::u8vec2>> entries_1 = { entry_2 };
      std::vector<std::pair<glm::uvec3, glm::u8vec2>> entries_2 = { entry_3 };

      std::vector<nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::u8vec2>::ConstructionElement> result = {};
      THEN("SpatialHashFunctionConstructionInvalidArgException should be thrown") {
        REQUIRE_THROWS_AS(builder.mapEntryVector(entries_0, 0, 1, result),
                          nTiled::pipeline::hashed::SpatialHashFunctionConstructionInvalidArgException);
        REQUIRE_THROWS_AS(builder.mapEntryVector(entries_1, 0, 3, result),
                          nTiled::pipeline::hashed::SpatialHashFunctionConstructionInvalidArgException);
        REQUIRE_THROWS_AS(builder.mapEntryVector(entries_2, 0, 5, result),
                          nTiled::pipeline::hashed::SpatialHashFunctionConstructionInvalidArgException);
      }
    }

    WHEN("An m_dim value such that m_dim * m_dim * m_dim is smaller than the number of entries is provided") {
      std::pair<glm::uvec3, glm::u8vec2> entry_1 =
        std::pair<glm::uvec3, glm::u8vec2>(glm::uvec3(0, 0, 0),
                                           glm::u8vec2(1, 1));

      std::pair<glm::uvec3, glm::u8vec2> entry_2 =
        std::pair<glm::uvec3, glm::u8vec2>(glm::uvec3(5, 5, 5),
                                           glm::u8vec2(1, 1));

      std::pair<glm::uvec3, glm::u8vec2> entry_3 =
        std::pair<glm::uvec3, glm::u8vec2>(glm::uvec3(256, 0, 0),
                                           glm::u8vec2(1, 1));

      std::vector<std::pair<glm::uvec3, glm::u8vec2>> entries = { 
        entry_1,
        entry_2,
        entry_3
      };

      std::vector<nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::u8vec2>::ConstructionElement> result = {};
      THEN("SpatialHashFunctionConstructionInvalidArgException should be thrown") {
        REQUIRE_THROWS_AS(builder.mapEntryVector(entries, 1, 1, result),
                          nTiled::pipeline::hashed::SpatialHashFunctionConstructionInvalidArgException);
      }
    }

    WHEN("An r_dim value of 0 is provided") {
      std::pair<glm::uvec3, glm::u8vec2> entry_1 =
        std::pair<glm::uvec3, glm::u8vec2>(glm::uvec3(0, 0, 0),
                                           glm::u8vec2(1, 1));

      std::pair<glm::uvec3, glm::u8vec2> entry_2 =
        std::pair<glm::uvec3, glm::u8vec2>(glm::uvec3(5, 5, 5),
                                           glm::u8vec2(1, 1));

      std::pair<glm::uvec3, glm::u8vec2> entry_3 =
        std::pair<glm::uvec3, glm::u8vec2>(glm::uvec3(256, 0, 0),
                                           glm::u8vec2(1, 1));

      std::vector<std::pair<glm::uvec3, glm::u8vec2>> entries_0 = { entry_1 };
      std::vector<std::pair<glm::uvec3, glm::u8vec2>> entries_1 = { entry_2 };
      std::vector<std::pair<glm::uvec3, glm::u8vec2>> entries_2 = { entry_3 };

      std::vector<nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::u8vec2>::ConstructionElement> result = {};
      THEN("SpatialHashFunctionConstructionInvalidArgException should be thrown") {
        REQUIRE_THROWS_AS(builder.mapEntryVector(entries_0, 1, 0, result),
                          nTiled::pipeline::hashed::SpatialHashFunctionConstructionInvalidArgException);
        REQUIRE_THROWS_AS(builder.mapEntryVector(entries_1, 3, 0, result),
                          nTiled::pipeline::hashed::SpatialHashFunctionConstructionInvalidArgException);
        REQUIRE_THROWS_AS(builder.mapEntryVector(entries_2, 5, 0, result),
                          nTiled::pipeline::hashed::SpatialHashFunctionConstructionInvalidArgException);
 
      }
    }
  }
}

SCENARIO("mapEntryVectorBehaviour should create a sorted vector of ConstructionElements containing all entries, or return false.",
         "[LinklessOctreeFull][SpatialHashFunctionFull][SpatialHashFunctionBuilder]") {
  GIVEN("A SpatialHashFunctionBuilder") {
    nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::u8vec2> builder =
      nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::u8vec2>();

    WHEN("A set containing elements with different data colliding for both h_0 and h_1 is provided") {
      std::vector<std::pair<glm::uvec3, glm::u8vec2>> entries_1 = {
        std::pair<glm::uvec3, glm::u8vec2>(glm::uvec3(15, 15, 15),
                                           glm::u8vec2(0, 0)),
        std::pair<glm::uvec3, glm::u8vec2>(glm::uvec3(30, 30, 30),
                                           glm::u8vec2(1, 1))
      };

      std::mt19937 gen = std::mt19937(31);
      std::uniform_int_distribution<unsigned short> distribution_u8vec3 =
        std::uniform_int_distribution<unsigned short>(0, 255);

      std::uniform_int_distribution<unsigned int> distribution_uvec3 =
        std::uniform_int_distribution<unsigned int>(0, 1000000);

      std::vector<std::pair<glm::uvec3, glm::u8vec2>> entries_2 =
        std::vector<std::pair<glm::uvec3, glm::u8vec2>>();

      std::vector<glm::uvec3> used_values = std::vector<glm::uvec3>();

      glm::uvec3 loc;
      glm::u8vec2 data;
      for (unsigned int i = 0; i < 100; ++i) {
        // generate data
        data = glm::u8vec2(distribution_u8vec3(gen),
                           distribution_u8vec3(gen));
        // find unique location
        do {
          loc = glm::uvec3(distribution_uvec3(gen),
                           distribution_uvec3(gen),
                           distribution_u8vec3(gen));
        } while (std::find(used_values.begin(), 
                           used_values.end(),
                           loc) != used_values.end());
        used_values.push_back(loc);
       
        // build entries list
        entries_2.push_back(std::pair<glm::uvec3, glm::u8vec2>(loc, data));
      }
      entries_2.push_back(std::pair<glm::uvec3, glm::u8vec2>(glm::uvec3(15, 15, 15),
                                                             glm::u8vec2(0, 0)));
      entries_2.push_back(std::pair<glm::uvec3, glm::u8vec2>(glm::uvec3(30, 30, 30),
                                                             glm::u8vec2(0, 0)));

      std::vector<nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::u8vec2>::ConstructionElement> result = {};

      THEN("mapEntryVector should return false.") {
        REQUIRE_FALSE(builder.mapEntryVector(entries_1, 5, 3, result));
        REQUIRE_FALSE(builder.mapEntryVector(entries_2, 5, 3, result));
      }
    }

    WHEN("A set containing elements with similar data colliding for both h_0 and h_1 is provided") {
      std::vector<std::pair<glm::uvec3, glm::u8vec2>> entries_1 = {
        std::pair<glm::uvec3, glm::u8vec2>(glm::uvec3(15, 15, 15),
                                           glm::u8vec2(9, 4)),
        std::pair<glm::uvec3, glm::u8vec2>(glm::uvec3(30, 30, 30),
                                           glm::u8vec2(9, 4))
      };

      std::vector<nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::u8vec2>::ConstructionElement> result = {};
      THEN("mapEntryVector should return true, return sorted data with only a single instance of the data.") {
        REQUIRE(builder.mapEntryVector(entries_1, 3, 1, result));
        REQUIRE(result.at(0).elements.size() == 1);
        REQUIRE(result.at(0).elements[0].data.x == 9);
        REQUIRE(result.at(0).elements[0].data.y == 4);
      }
    }

    WHEN("A set containing elements not colliding for both h_0 and h_1") {
      std::mt19937 gen = std::mt19937(31);
      std::uniform_int_distribution<unsigned short> distribution_u8vec3 =
        std::uniform_int_distribution<unsigned short>(0, 255);

      std::uniform_int_distribution<unsigned int> distribution_uvec3 =
        std::uniform_int_distribution<unsigned int>(0, 1000000);

      std::vector<std::pair<glm::uvec3, glm::u8vec2>> entries =
        std::vector<std::pair<glm::uvec3, glm::u8vec2>>();

      std::vector<std::pair<glm::uvec3, glm::uvec3>> used_hashes = {};

      glm::uvec3 loc;
      glm::uvec3 h_0;
      glm::uvec3 h_1;
      glm::u8vec2 data;
      bool found_value;

      for (unsigned int i = 0; i < 100; ++i) {
        // generate data
        data = glm::u8vec2(distribution_u8vec3(gen),
                           distribution_u8vec3(gen));
        // find unique location
        found_value = false;
        while (!found_value) {
          loc = glm::uvec3(distribution_uvec3(gen),
                           distribution_uvec3(gen),
                           distribution_u8vec3(gen));
          h_0 = glm::uvec3(loc.x % 7,
                           loc.y % 7,
                           loc.z % 7);
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
        }
        entries.push_back(std::pair<glm::uvec3, glm::u8vec2>(loc, data));
        used_hashes.push_back(std::pair<glm::uvec3, glm::uvec3>(h_0, h_1));
      }

      std::vector<nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::u8vec2>::ConstructionElement> result = {};
      THEN("mapEntryVector should return true and result should contain a sorted vector containing all elements in Entries") {
        // TODO add additional tests here
        REQUIRE(glm::u8vec2(0, 1) == glm::u8vec2(0, 1));
        REQUIRE(builder.mapEntryVector(entries, 7, 3, result));

        int n_entries = 0;
        unsigned int previous_numbers = result.at(0).elements.size();
        for (const nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::u8vec2>::ConstructionElement& element : result) {
          REQUIRE(previous_numbers >= element.elements.size());
          n_entries += element.elements.size();
        }

        REQUIRE(n_entries == 100);
      }
    }
  }
}
