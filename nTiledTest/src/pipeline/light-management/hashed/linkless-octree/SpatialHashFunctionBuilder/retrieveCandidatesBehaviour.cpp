#include <catch.hpp>
#include "pipeline\light-management\hashed\linkless-octree\SpatialHashFunctionBuilder.h"

// ----------------------------------------------------------------------------
//  nTiled Headers
// ----------------------------------------------------------------------------
#include "pipeline\light-management\hashed\linkless-octree\Exceptions.h"

// ----------------------------------------------------------------------------
//  retrieveCandidates Scenarios
// ----------------------------------------------------------------------------
/* 
 * Retrieve the neighbouring values within the offset table Phi
 *         around point p, and store them in candidate_vector.
 *
 * @param p The 3d point around which the values should be retrieved.
 * @param r_dim The size of a single dimension of the offset table Phi
 * @param offset_defined_table Vector describing whether a corresponding 
 *                             value in the offset table Phi is defined.
 * @param offset_table Offset table Phi
 * @param candidate_vector The output vector in which the candidates are 
 *                         stored.
 *
 * @post FORALL p_i: next_to(p_i, p) -> Phi[p_i] IN candidate_vector
 *
 * void retrieveCandidates(glm::uvec3 p,
 *                         unsigned int r_dim,
 *                         const std::vector<bool>& offset_defined_table,
 *                         const std::vector<glm::u8vec3>& offset_table,
 *                         std::vector<glm::u8vec3>& candidate_vector);
 */
SCENARIO("retrieveCandidates should return all neighbouring offset values around the given point p",
         "[LinklessOctreeFull][SpatialHashFunctionFull][SpatialHashFunctionBuilder]") {
  GIVEN("A SpatialHashFunctionBuilder") {
    nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::u8vec2> builder =
      nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::u8vec2>();

    WHEN("No offset values are defined") {
    nTiled::pipeline::hashed::Table<glm::u8vec3> offset_table_1 =
      nTiled::pipeline::hashed::Table<glm::u8vec3>(2);

      std::vector<glm::u8vec3> candidate_vector_1 = std::vector<glm::u8vec3>();
      std::vector<glm::u8vec3> candidate_vector_2 = std::vector<glm::u8vec3>();
      std::vector<glm::u8vec3> candidate_vector_3 = std::vector<glm::u8vec3>();
      std::vector<glm::u8vec3> candidate_vector_4 = std::vector<glm::u8vec3>();

      THEN("candidate_vector should be empty") {
        REQUIRE(candidate_vector_1.empty());
        builder.retrieveCandidates(glm::uvec3(0, 0, 0),
                                   offset_table_1,
                                   candidate_vector_1);
        REQUIRE(candidate_vector_1.empty());

        REQUIRE(candidate_vector_2.empty());
        builder.retrieveCandidates(glm::uvec3(0, 0, 1),
                                   offset_table_1,
                                   candidate_vector_2);
        REQUIRE(candidate_vector_2.empty());

        REQUIRE(candidate_vector_3.empty());
        builder.retrieveCandidates(glm::uvec3(0, 1, 1),
                                   offset_table_1,
                                   candidate_vector_3);
        REQUIRE(candidate_vector_3.empty());

        REQUIRE(candidate_vector_4.empty());
        builder.retrieveCandidates(glm::uvec3(1, 1, 1),
                                   offset_table_1,
                                   candidate_vector_4);
        REQUIRE(candidate_vector_4.empty());
      }
    }

    WHEN("Offset values are defined and neighbouring the point p") {
      nTiled::pipeline::hashed::Table<glm::u8vec3> offset_table_1 =
        nTiled::pipeline::hashed::Table<glm::u8vec3>(2);
      offset_table_1.setPoint(glm::uvec3(0, 0, 0), glm::u8vec3(0, 1, 0));
      offset_table_1.setPoint(glm::uvec3(0, 1, 0), glm::u8vec3(2, 1, 1));
      offset_table_1.setPoint(glm::uvec3(1, 1, 0), glm::u8vec3(3, 4, 0));
      offset_table_1.setPoint(glm::uvec3(1, 0, 1), glm::u8vec3(5, 5, 5));

      std::vector<glm::u8vec3> candidate_vector_1 = std::vector<glm::u8vec3>();
      std::vector<glm::u8vec3> candidate_vector_2 = std::vector<glm::u8vec3>();
      std::vector<glm::u8vec3> candidate_vector_3 = std::vector<glm::u8vec3>();
      std::vector<glm::u8vec3> candidate_vector_4 = std::vector<glm::u8vec3>();
      THEN("candidate_vector should contain these offset values") {
        REQUIRE(candidate_vector_1.empty());
        builder.retrieveCandidates(glm::uvec3(1, 0, 0),
                                   offset_table_1,
                                   candidate_vector_1);
        REQUIRE(candidate_vector_1.size() == 4);
        for (glm::u8vec3 val : candidate_vector_1) {
          REQUIRE((val == glm::u8vec3(0, 1, 0) || 
                   val == glm::u8vec3(3, 4, 0) ||
                   val == glm::u8vec3(5, 5, 5) ||
                   val == glm::u8vec3(2, 1, 1)));
        }

        REQUIRE(candidate_vector_2.empty());
        builder.retrieveCandidates(glm::uvec3(0, 0, 1),
                                   offset_table_1,
                                   candidate_vector_2);
        REQUIRE(candidate_vector_2.size() == 4);
        for (glm::u8vec3 val : candidate_vector_2) {
          REQUIRE((val == glm::u8vec3(0, 1, 0) || 
                   val == glm::u8vec3(3, 4, 0) ||
                   val == glm::u8vec3(5, 5, 5) ||
                   val == glm::u8vec3(2, 1, 1)));
        }

        REQUIRE(candidate_vector_3.empty());
        builder.retrieveCandidates(glm::uvec3(1, 1, 1),
                                   offset_table_1,
                                   candidate_vector_3);
        REQUIRE(candidate_vector_3.size() == 4);
        for (glm::u8vec3 val : candidate_vector_3) {
          REQUIRE((val == glm::u8vec3(0, 1, 0) || 
                   val == glm::u8vec3(3, 4, 0) ||
                   val == glm::u8vec3(5, 5, 5) ||
                   val == glm::u8vec3(2, 1, 1)));
        }

        REQUIRE(candidate_vector_4.empty());
        builder.retrieveCandidates(glm::uvec3(0, 1, 1),
                                   offset_table_1,
                                   candidate_vector_4);
        REQUIRE(candidate_vector_4.size() == 4);
        for (glm::u8vec3 val : candidate_vector_4) {
          REQUIRE((val == glm::u8vec3(0, 1, 0) || 
                   val == glm::u8vec3(3, 4, 0) ||
                   val == glm::u8vec3(5, 5, 5) ||
                   val == glm::u8vec3(2, 1, 1)));
        }
      }
    }

    WHEN("One single offset values neighbours p multiple times") {
      nTiled::pipeline::hashed::Table<glm::u8vec3> offset_table_1 =
        nTiled::pipeline::hashed::Table<glm::u8vec3>(2);
      offset_table_1.setPoint(glm::uvec3(0, 0, 0), glm::u8vec3(0, 1, 0));
      offset_table_1.setPoint(glm::uvec3(0, 1, 0), glm::u8vec3(0, 1, 0));
      offset_table_1.setPoint(glm::uvec3(1, 1, 0), glm::u8vec3(0, 1, 0));

      offset_table_1.setPoint(glm::uvec3(0, 0, 1), glm::u8vec3(0, 1, 0));
      offset_table_1.setPoint(glm::uvec3(1, 0, 1), glm::u8vec3(0, 1, 0));
      offset_table_1.setPoint(glm::uvec3(0, 1, 1), glm::u8vec3(0, 1, 0));
      offset_table_1.setPoint(glm::uvec3(1, 1, 1), glm::u8vec3(0, 1, 0));

      std::vector<glm::u8vec3> candidate_vector_1 = std::vector<glm::u8vec3>();
      THEN("candidate_vector should only contain a single entry of this single offset") {
        REQUIRE(candidate_vector_1.empty());
        builder.retrieveCandidates(glm::uvec3(1, 0, 0),
                                   offset_table_1,
                                   candidate_vector_1);
        REQUIRE(candidate_vector_1.size() == 1);
        for (glm::u8vec3 val : candidate_vector_1) {
          REQUIRE(val == glm::u8vec3(0, 1, 0));
        }
      }
    }
  }
}
