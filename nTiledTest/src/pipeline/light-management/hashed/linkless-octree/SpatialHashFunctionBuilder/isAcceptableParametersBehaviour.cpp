#include <catch.hpp>
#include "pipeline\light-management\hashed\linkless-octree\SpatialHashFunctionBuilder.h"

// ----------------------------------------------------------------------------
//  isAcceptableParameters Scenarios
// ----------------------------------------------------------------------------
SCENARIO("isAcceptableParameters should return True if parameters are acceptable, False otherwise",
         "[LinklessOctreeFull][SpatialHashFunctionFull][SpatialHashFunctionBuilder]") {
  GIVEN("A SpatialHashFunctionBuilder") {
    nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::u8vec2> builder =
      nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::u8vec2>();

    WHEN("m_dim equals 1") {
      THEN("True should be returned") {
        REQUIRE(builder.isAcceptableParameters(1, 1));
        REQUIRE(builder.isAcceptableParameters(1, 5));
        REQUIRE(builder.isAcceptableParameters(1, 11));
        REQUIRE(builder.isAcceptableParameters(1, 101));
        REQUIRE(builder.isAcceptableParameters(1, 501));
      }
    }

    WHEN("r_dim equals 1") {
      THEN("True should be returned") {
        REQUIRE(builder.isAcceptableParameters(5, 1));
        REQUIRE(builder.isAcceptableParameters(7, 1));
        REQUIRE(builder.isAcceptableParameters(11, 1));
        REQUIRE(builder.isAcceptableParameters(101, 1));
        REQUIRE(builder.isAcceptableParameters(201, 1));
      }
    }

    WHEN("Acceptable parameters are provided") {
      THEN("True should be returned") {
        REQUIRE(builder.isAcceptableParameters(17, 5));
        REQUIRE(builder.isAcceptableParameters(23, 7));
        REQUIRE(builder.isAcceptableParameters(121, 11));
        REQUIRE(builder.isAcceptableParameters(173, 31));
        REQUIRE(builder.isAcceptableParameters(231, 51));
      }
    }

    WHEN("m and r share a common divisor") {
      THEN("False should be returned") {
        REQUIRE(!builder.isAcceptableParameters(15, 3));
        REQUIRE(!builder.isAcceptableParameters(15, 10));
        REQUIRE(!builder.isAcceptableParameters(45, 25));
        REQUIRE(!builder.isAcceptableParameters(210, 30));
        REQUIRE(!builder.isAcceptableParameters(500, 100));
      }
    }

    WHEN("m mod r = 1") {
      THEN("False should be returned") {
        REQUIRE(!builder.isAcceptableParameters(16, 3));
        REQUIRE(!builder.isAcceptableParameters(21, 4));
        REQUIRE(!builder.isAcceptableParameters(121, 30));
        REQUIRE(!builder.isAcceptableParameters(151, 25));
        REQUIRE(!builder.isAcceptableParameters(211, 30));
      }
    }

    WHEN("m mod r = r - 1") {
      THEN("False should be returned") {
        REQUIRE(!builder.isAcceptableParameters(154, 5));
        REQUIRE(!builder.isAcceptableParameters(279, 70));
        REQUIRE(!builder.isAcceptableParameters(216, 31));
        REQUIRE(!builder.isAcceptableParameters(599, 100));
        REQUIRE(!builder.isAcceptableParameters(59, 15));
      }
    }
  }
}
