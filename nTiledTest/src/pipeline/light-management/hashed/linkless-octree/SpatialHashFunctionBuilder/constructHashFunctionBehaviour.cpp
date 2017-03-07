#include <catch.hpp>
#include "pipeline\light-management\hashed\linkless-octree\SpatialHashFunctionBuilder.h"

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <random>
#include <algorithm>

// ----------------------------------------------------------------------------
//  nTiled Headers
// ----------------------------------------------------------------------------
#include "pipeline\light-management\hashed\linkless-octree\Exceptions.h"

// ----------------------------------------------------------------------------
//  constructHashFunction Scenarios
// ----------------------------------------------------------------------------
SCENARIO("A constructed SpatialHashFunction should contain all entry elements with which it was build.",
         "[LinklessOctreeFull][SpatialHashFunctionFull][SpatialHashFunctionBuilder]") {

  GIVEN("A SpatialHashFunctionBuilder") {
    nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::u8vec2> builder = 
      nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::u8vec2>();

    WHEN("The entry list is empty") {
      std::vector<std::pair<glm::uvec3, glm::u8vec2>> entries =
        std::vector<std::pair<glm::uvec3, glm::u8vec2>>();

      REQUIRE(entries.empty());

      THEN("A SpatialHashFunctionConstructionException should be thrown") {
        REQUIRE_THROWS_AS(builder.constructHashFunction(entries, 10, 1.5),
                          nTiled::pipeline::hashed::SpatialHashFunctionConstructionException);
      }
    }

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

      nTiled::pipeline::hashed::SpatialHashFunction<glm::u8vec2>* p_hash_function_1 = 
        builder.constructHashFunction(entries_1, 10, 1.5);
      nTiled::pipeline::hashed::SpatialHashFunction<glm::u8vec2>* p_hash_function_2 = 
        builder.constructHashFunction(entries_2, 10, 1.5);
      nTiled::pipeline::hashed::SpatialHashFunction<glm::u8vec2>* p_hash_function_3 = 
        builder.constructHashFunction(entries_3, 10, 1.5);

      REQUIRE(entries_1.size() == 1);
      REQUIRE(entries_2.size() == 1);
      REQUIRE(entries_3.size() == 1);

      THEN("A SpatialHashFunction with a single entry equal to that entry should be constructed") {
        REQUIRE(p_hash_function_1->getM() == 1);
        REQUIRE(p_hash_function_1->getR() == 1);
        REQUIRE(p_hash_function_1->getData(entry_1.first) == entry_1.second);

        REQUIRE(p_hash_function_2->getM() == 1);
        REQUIRE(p_hash_function_2->getR() == 1);
        REQUIRE(p_hash_function_2->getData(entry_2.first) == entry_2.second);

        REQUIRE(p_hash_function_3->getM() == 1);
        REQUIRE(p_hash_function_3->getR() == 1);
        REQUIRE(p_hash_function_3->getData(entry_3.first) == entry_3.second);
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
      for (unsigned int i = 0; i <= 1000; ++i) {
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

      REQUIRE(entries.size() == 1000);

      nTiled::pipeline::hashed::SpatialHashFunction<glm::u8vec2>* p_hash_function_1 = 
        builder.constructHashFunction(entries, 10, 1.5);

      THEN("Each entry should be contained in the constructed SpatialHashFunction") {
        for (const std::pair<glm::uvec3, glm::u8vec2>& entry : entries) {
          REQUIRE(p_hash_function_1->getData(entry.first) == entry.second);
        }
      }
    }
  }
}
