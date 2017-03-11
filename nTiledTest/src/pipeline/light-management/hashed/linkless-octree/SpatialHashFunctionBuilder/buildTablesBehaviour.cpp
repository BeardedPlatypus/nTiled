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

    WHEN("entries contains no elements") {
      std::vector<nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::u8vec2>::ConstructionElement> entry_vector = {};
      
      nTiled::pipeline::hashed::Table<glm::u8vec2> hash_table =
        nTiled::pipeline::hashed::Table<glm::u8vec2>(5);
      
      nTiled::pipeline::hashed::Table<glm::u8vec3> offset_table =
        nTiled::pipeline::hashed::Table<glm::u8vec3>(2);

      THEN("A SpatialHashFunctionConstructionInvalidArgException is thrown") {
        REQUIRE_THROWS_AS(builder.buildTables(entry_vector,
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

      std::vector<nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::u8vec2>::ConstructionElement> entry_vector_1 = {};
      std::vector<nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::u8vec2>::ConstructionElement> entry_vector_2 = {};
      std::vector<nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::u8vec2>::ConstructionElement> entry_vector_3 = {};
      builder.mapEntryVector(entries_1, 1, 1, entry_vector_1);
      builder.mapEntryVector(entries_2, 1, 1, entry_vector_2);
      builder.mapEntryVector(entries_3, 1, 1, entry_vector_3);

      
      // ----------------------------------------------------------------------
      nTiled::pipeline::hashed::Table<glm::u8vec2> hash_table_1 =
        nTiled::pipeline::hashed::Table<glm::u8vec2>(1);
      
      nTiled::pipeline::hashed::Table<glm::u8vec3> offset_table_1 =
        nTiled::pipeline::hashed::Table<glm::u8vec3>(1);

      // ----------------------------------------------------------------------
      nTiled::pipeline::hashed::Table<glm::u8vec2> hash_table_2 =
        nTiled::pipeline::hashed::Table<glm::u8vec2>(1);
      
      nTiled::pipeline::hashed::Table<glm::u8vec3> offset_table_2 =
        nTiled::pipeline::hashed::Table<glm::u8vec3>(1);

      // ----------------------------------------------------------------------
      nTiled::pipeline::hashed::Table<glm::u8vec2> hash_table_3 =
        nTiled::pipeline::hashed::Table<glm::u8vec2>(1);
      
      nTiled::pipeline::hashed::Table<glm::u8vec3> offset_table_3 =
        nTiled::pipeline::hashed::Table<glm::u8vec3>(1);
      THEN("The hash table contains only the single entry and the offset table contains a single entry") {
        
        REQUIRE(builder.buildTables(entry_vector_1, hash_table_1, offset_table_1));
        REQUIRE(builder.buildTables(entry_vector_2, hash_table_2, offset_table_2));
        REQUIRE(builder.buildTables(entry_vector_3, hash_table_3, offset_table_3));

        REQUIRE(hash_table_1.getPoint(glm::uvec3(0, 0, 0)).x == entry_1.second.x);
        REQUIRE(hash_table_1.getPoint(glm::uvec3(0, 0, 0)).y == entry_1.second.y);

        REQUIRE(hash_table_2.getPoint(glm::uvec3(0, 0, 0)).x == entry_2.second.x);
        REQUIRE(hash_table_2.getPoint(glm::uvec3(0, 0, 0)).y == entry_2.second.y);

        REQUIRE(hash_table_3.getPoint(glm::uvec3(0, 0, 0)).x == entry_3.second.x);
        REQUIRE(hash_table_3.getPoint(glm::uvec3(0, 0, 0)).y == entry_3.second.y);
      }
    }


    WHEN("The entry list contains a set of unique entries greater than one that do not contain conflicting elements") {
      // generate different sized sets of entries
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

      REQUIRE(entries.size() == 100);

      std::vector<nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::u8vec2>::ConstructionElement> entry_vector = {};
      builder.mapEntryVector(entries, 7, 3, entry_vector);

      nTiled::pipeline::hashed::Table<glm::u8vec2> hash_table =
        nTiled::pipeline::hashed::Table<glm::u8vec2>(7);
      
      nTiled::pipeline::hashed::Table<glm::u8vec3> offset_table =
        nTiled::pipeline::hashed::Table<glm::u8vec3>(3);

      glm::u8vec2 val_hash;
      glm::u8vec2 val_actual;

      THEN("All elements are in the hash table H, and no collisions occur for these entries, and size of hash table H == m * m * m, and size of offset table Phi == r * r * r") {
        bool has_build_tables = builder.buildTables(entry_vector, hash_table, offset_table);
        REQUIRE(has_build_tables);

        glm::uvec3 loc_offset;
        glm::uvec3 loc_hash;
        glm::u8vec3 offset;
        for (const auto& entry : entries) {
          loc_offset = glm::uvec3(entry.first.x % 3,
                                  entry.first.y % 3,
                                  entry.first.z % 3);
          offset = offset_table.getPoint(loc_offset);
          
          loc_hash = glm::uvec3(((entry.first.x  % 7 ) + offset.x ) % 7,
                                ((entry.first.y  % 7 ) + offset.y ) % 7,
                                ((entry.first.z  % 7 ) + offset.z ) % 7);

          val_hash = hash_table.getPoint(loc_hash);
          val_actual = entry.second;

          REQUIRE(( val_actual.x == val_hash.x && 
                    val_actual.y == val_hash.y ));
        }
      }
    }
  }
}
