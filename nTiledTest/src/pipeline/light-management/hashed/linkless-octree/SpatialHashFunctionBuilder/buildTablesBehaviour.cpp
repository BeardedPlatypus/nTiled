#include <catch.hpp>
#include "pipeline\light-management\hashed\linkless-octree\SpatialHashFunctionBuilder.h"

// ----------------------------------------------------------------------------
//  nTiled Headers
// ----------------------------------------------------------------------------
#include "pipeline\light-management\hashed\linkless-octree\Exceptions.h"
#include "math\util.h"


// ----------------------------------------------------------------------------
//  buildTables Scenarios
// ----------------------------------------------------------------------------
SCENARIO("buildTables should throw a SpatialHashFunctionConstructionInvalidArgException if an invalid argument is provided",
         "[LinklessOctreeFull][SpatialHashFunctionFull][SpatialHashFunctionBuilder]") {
  GIVEN("A SpatialHashFunctionBuilder") {
    nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::u8vec2> builder =
      nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::u8vec2>();

    std::vector<glm::u8vec2> hash_table = {};
    std::vector<glm::u8vec3> offset_table = {};

    WHEN("m_dim is zero") {
      std::pair<glm::uvec3, glm::u8vec2> entry =
        std::pair<glm::uvec3, glm::u8vec2>(glm::uvec3(0, 0, 0),
                                           glm::u8vec2(1, 1));

      std::vector<std::pair<glm::uvec3, glm::u8vec2>> entries = { entry };

      hash_table.clear();
      offset_table.clear();

      THEN("A SpatialHashFunctionConstructionInvalidArgException is thrown") {
        REQUIRE_THROWS_AS(builder.buildTables(0, 5, entries, 
                                              hash_table, 
                                              offset_table),
                          nTiled::pipeline::hashed::SpatialHashFunctionConstructionInvalidArgException);
      }
    }

    WHEN("r_dim is zero") {
      std::pair<glm::uvec3, glm::u8vec2> entry =
        std::pair<glm::uvec3, glm::u8vec2>(glm::uvec3(0, 0, 0),
                                           glm::u8vec2(1, 1));

      std::vector<std::pair<glm::uvec3, glm::u8vec2>> entries = { entry };

      hash_table.clear();
      offset_table.clear();

      THEN("A SpatialHashFunctionConstructionInvalidArgException is thrown") {
        REQUIRE_THROWS_AS(builder.buildTables(5, 0, entries,
                                              hash_table, 
                                              offset_table),
                          nTiled::pipeline::hashed::SpatialHashFunctionConstructionInvalidArgException);
      }
    }

    WHEN("entries contains no elements") {
      std::vector<std::pair<glm::uvec3, glm::u8vec2>> entries = {};

      hash_table.clear();
      offset_table.clear();

      THEN("A SpatialHashFunctionConstructionInvalidArgException is thrown") {
        REQUIRE_THROWS_AS(builder.buildTables(5, 2, entries,
                                              hash_table, 
                                              offset_table),
                          nTiled::pipeline::hashed::SpatialHashFunctionConstructionInvalidArgException);

      }
    }

    WHEN("entries is larger than m * m * m") {
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

      hash_table.clear();
      offset_table.clear();

      THEN("A SpatialHashFunctionConstructionInvalidArgException is thrown") {
        REQUIRE_THROWS_AS(builder.buildTables(1, 2, entries,
                                              hash_table, 
                                              offset_table),
                          nTiled::pipeline::hashed::SpatialHashFunctionConstructionInvalidArgException);

      }
    }
  }
}


SCENARIO("build tables should return a valid hash and offset table when presented with valid arguments, should return an exception otherwise",
         "[LinklessOctreeFull][SpatialHashFunctionFull][SpatialHashFunctionBuilder]") {
  GIVEN("A SpatialHashFunctionBuilder") {
    nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::u8vec2> builder =
      nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::u8vec2>();

    WHEN("The entry list contains a single entry") {
      std::pair<glm::uvec3, glm::u8vec2> entry_1 =
        std::pair<glm::uvec3, glm::u8vec2>(glm::uvec3(0, 0, 0),
                                           glm::u8vec2(1, 1));

      std::pair<glm::uvec3, glm::u8vec2> entry_2 =
        std::pair<glm::uvec3, glm::u8vec2>(glm::uvec3(5, 5, 5),
                                           glm::u8vec2(1, 1));

      std::pair<glm::uvec3, glm::u8vec2> entry_3 =
        std::pair<glm::uvec3, glm::u8vec2>(glm::uvec3(256, 0, 0),
                                           glm::u8vec2(1, 1));

      std::vector<std::pair<glm::uvec3, glm::u8vec2>> entries_1 = { entry_1 };
      std::vector<std::pair<glm::uvec3, glm::u8vec2>> entries_2 = { entry_2 };
      std::vector<std::pair<glm::uvec3, glm::u8vec2>> entries_3 = { entry_3 };

      REQUIRE(entries_1.size() == 1);
      REQUIRE(entries_2.size() == 1);
      REQUIRE(entries_3.size() == 1);
      
      std::vector<glm::u8vec2> hash_table_1 = {};
      std::vector<glm::u8vec3> offset_table_1 = {};

      std::vector<glm::u8vec2> hash_table_2 = {};
      std::vector<glm::u8vec3> offset_table_2 = {};

      std::vector<glm::u8vec2> hash_table_3 = {};
      std::vector<glm::u8vec3> offset_table_3 = {};
      THEN("The hash table contains only the single entry and the offset table contains a single entry") {
        
        REQUIRE(builder.buildTables(1, 1, entries_1, hash_table_1, offset_table_1));
        REQUIRE(builder.buildTables(1, 1, entries_2, hash_table_2, offset_table_2));
        REQUIRE(builder.buildTables(1, 1, entries_3, hash_table_3, offset_table_3));

        REQUIRE(hash_table_1.size() == 1);
        REQUIRE(offset_table_1.size() == 1);
        REQUIRE(hash_table_1.at(0) == entry_1.second);

        REQUIRE(hash_table_2.size() == 1);
        REQUIRE(offset_table_2.size() == 1);
        REQUIRE(hash_table_2.at(0) == entry_2.second);

        REQUIRE(hash_table_3.size() == 1);
        REQUIRE(offset_table_3.size() == 1);
        REQUIRE(hash_table_3.at(0) == entry_3.second);
      }
    }

    WHEN("The entry list contains a set of unique entries greater than one") {
      // generate different sized sets of entries
      std::mt19937 gen = std::mt19937(31);
      std::uniform_int_distribution<unsigned short> distribution_u8vec3 =
        std::uniform_int_distribution<unsigned short>(0, 255);

      std::uniform_int_distribution<unsigned int> distribution_uvec3 =
        std::uniform_int_distribution<unsigned int>(0, 1000000);

      std::vector<std::pair<glm::uvec3, glm::u8vec2>> entries =
        std::vector<std::pair<glm::uvec3, glm::u8vec2>>();

      std::vector<glm::uvec3> used_values = std::vector<glm::uvec3>();

      glm::uvec3 loc;
      glm::u8vec2 data;
      for (unsigned int i = 0; i <= 100; ++i) {
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
        entries.push_back(std::pair<glm::uvec3, glm::u8vec2>(loc, data));
      }

      REQUIRE(entries.size() == 100);

      std::vector<glm::u8vec2> hash_table = {};
      std::vector<glm::u8vec3> offset_table = {};

      THEN("All elements are in the hash table H, and no collisions occur for these entries, and size of hash table H == m * m * m, and size of offset table Phi == r * r * r") {
        REQUIRE(builder.buildTables(5, 3, entries, hash_table, offset_table));

        REQUIRE(hash_table.size() == 125);
        REQUIRE(offset_table.size() == 9);

        glm::uvec3 loc_offset;
        glm::uvec3 loc_hash;
        glm::u8vec3 offset;
        for (const auto& entry : entries) {
          loc_offset = glm::uvec3(entry.first.x % 3,
                                  entry.first.y % 3,
                                  entry.first.z % 3);
          offset = offset_table.at(nTiled::math::toIndex(loc_offset, 3));
          
          loc_hash = glm::uvec3((entry.first.x + offset.x) % 5,
                                (entry.first.y + offset.y) % 5,
                                (entry.first.z + offset.z) % 5);

          REQUIRE(hash_table.at(nTiled::math::toIndex(loc_hash, 5)) == entry.second);
        }
      }
    }
  }
}
