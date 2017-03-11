#include <catch.hpp>
#include "pipeline\light-management\hashed\linkless-octree\SpatialHashFunctionBuilder.h"

// ----------------------------------------------------------------------------
//  nTiled Headers
// ----------------------------------------------------------------------------
#include "pipeline\light-management\hashed\linkless-octree\Exceptions.h"

// ----------------------------------------------------------------------------
//  isValidCandidate Scenarios
// ----------------------------------------------------------------------------
/*
 * bool isValidCandidate(glm::u8vec3 candidate,
 *                       unsigned int m_dim,
 *                       const std::vector<glm::uvec3>& elements,
 *                       const std::vector<bool>& hash_defined_table);
 */
namespace nTiled {
namespace pipeline {
namespace hashed {

SCENARIO("isValidCandidate should throw a SpatialHashFunctionConstructionInvalidArgException if an invalid argument is provided",
         "[LinklessOctreeFull][SpatialHashFunctionFull][SpatialHashFunctionBuilder]") {
  GIVEN("A SpatialHashFunctionBuilder") {
    SpatialHashFunctionBuilder<glm::u8vec2> builder =
      SpatialHashFunctionBuilder<glm::u8vec2>();

    nTiled::pipeline::hashed::Table<glm::u8vec2> table_1 =
      nTiled::pipeline::hashed::Table<glm::u8vec2>(2);

    nTiled::pipeline::hashed::Table<glm::u8vec2> table_2 =
      nTiled::pipeline::hashed::Table<glm::u8vec2>(3);

    WHEN("Element size exceeds hash table H size") {
      glm::u8vec3 cand_1 = glm::u8vec3(0, 0, 0);

      std::vector<glm::uvec3> elements_p_1 = { 
        glm::uvec3(1, 0, 0),
        glm::uvec3(0, 1, 0),
        glm::uvec3(0, 0, 1),
        glm::uvec3(0, 0, 2),
        glm::uvec3(1, 0, 5), 
        glm::uvec3(0, 2, 5), 
        glm::uvec3(1, 1, 5), 
        glm::uvec3(0, 2, 4),
        glm::uvec3(2, 5, 5)
      };

      std::vector<SpatialHashFunctionBuilder<glm::u8vec2>::EntryElement> elements_1 = {};
      for (glm::uvec3 p : elements_p_1) {
        elements_1.push_back(SpatialHashFunctionBuilder<glm::u8vec2>::EntryElement(p,
                                                                                   glm::uvec3(p.x % 2,
                                                                                              p.y % 2,
                                                                                              p.z % 2),
                                                                                   glm::u8vec2(0)));
      }


      std::vector<glm::uvec3> elements_p_2 = { glm::uvec3(1, 0, 0),
                                              glm::uvec3(0, 1, 0),
                                              glm::uvec3(0, 2, 0),
                                              glm::uvec3(2, 1, 0),
                                              glm::uvec3(0, 1, 0),
                                              glm::uvec3(5, 1, 0),
                                              glm::uvec3(0, 1, 5),
                                              glm::uvec3(0, 1, 2),
                                              glm::uvec3(0, 1, 3),
                                              glm::uvec3(3, 1, 4),
                                              glm::uvec3(2, 1, 2),
                                              glm::uvec3(0, 1, 6),
                                              glm::uvec3(6, 1, 0),
                                              glm::uvec3(5, 2, 0),
                                              glm::uvec3(2, 1, 1),
                                              glm::uvec3(3, 9, 1),
                                              glm::uvec3(2, 4, 1),
                                              glm::uvec3(3, 2, 1),
                                              glm::uvec3(2, 1, 1),
                                              glm::uvec3(2, 2, 2),
                                              glm::uvec3(2, 4, 4),
                                              glm::uvec3(2, 1, 8),
                                              glm::uvec3(2, 5, 8),
                                              glm::uvec3(2, 8, 1),
                                              glm::uvec3(2, 9, 1),
                                              glm::uvec3(2, 9, 2),
                                              glm::uvec3(2, 9, 3),
                                              glm::uvec3(2, 9, 2),
                                              glm::uvec3(0, 0, 2),
                                              glm::uvec3(3, 3, 1) };

      std::vector<SpatialHashFunctionBuilder<glm::u8vec2>::EntryElement> elements_2 = {};
      for (glm::uvec3 p : elements_p_2) {
        elements_2.push_back(SpatialHashFunctionBuilder<glm::u8vec2>::EntryElement(p,
                                                                                   glm::uvec3(p.x % 3,
                                                                                              p.y % 3,
                                                                                              p.z % 3),
                                                                                   glm::u8vec2(0)));
      }



      THEN("A SpatialHashFunctionConstructionInvalidArgException is thrown") {
        REQUIRE_THROWS_AS(builder.isValidCandidate(cand_1,
                                                   elements_1, 
                                                   table_1),
                          SpatialHashFunctionConstructionInvalidArgException);

        REQUIRE_THROWS_AS(builder.isValidCandidate(cand_1,
                                                   elements_2, 
                                                   table_2),
                          SpatialHashFunctionConstructionInvalidArgException);
      }
    }

    WHEN("Elements is empty") {
      glm::u8vec3 cand_1 = glm::u8vec3(0, 0, 0);
      glm::u8vec3 cand_2 = glm::u8vec3(1, 0, 3);

      std::vector<SpatialHashFunctionBuilder<glm::u8vec2>::EntryElement> elements_empty = {};


      THEN("A SpatialHashFunctionConstructionInvalidArgException is thrown") {
        REQUIRE_THROWS_AS(builder.isValidCandidate(cand_1,
                                                elements_empty, 
                                                table_1),
                          SpatialHashFunctionConstructionInvalidArgException);

        REQUIRE_THROWS_AS(builder.isValidCandidate(cand_2,
                                                elements_empty, 
                                                table_2),
                          SpatialHashFunctionConstructionInvalidArgException);

        REQUIRE_THROWS_AS(builder.isValidCandidate(cand_2,
                                                elements_empty, 
                                                table_1),
                          SpatialHashFunctionConstructionInvalidArgException);

        REQUIRE_THROWS_AS(builder.isValidCandidate(cand_1,
                                                elements_empty, 
                                                table_2),
                          SpatialHashFunctionConstructionInvalidArgException);
      }
    }
  }
}

SCENARIO("isValidCandidate should return True if the candidate does not cause any conflicts, False otherwise",
         "[LinklessOctreeFull][SpatialHashFunctionFull][SpatialHashFunctionBuilder]") {
  GIVEN("A SpatialHashFunctionBuilder and a set of hash_table_def") {
    SpatialHashFunctionBuilder<glm::u8vec2> builder =
      SpatialHashFunctionBuilder<glm::u8vec2>();

    nTiled::pipeline::hashed::Table<glm::u8vec2> table_empty =
      nTiled::pipeline::hashed::Table<glm::u8vec2>(3);
      

    nTiled::pipeline::hashed::Table<glm::u8vec2> table_0 =
      nTiled::pipeline::hashed::Table<glm::u8vec2>(3);
    table_0.setPoint(glm::uvec3(0, 2, 0), glm::u8vec2(0, 0));
    table_0.setPoint(glm::uvec3(1, 2, 0), glm::u8vec2(0, 0));
    table_0.setPoint(glm::uvec3(1, 2, 1), glm::u8vec2(0, 0));
    
    nTiled::pipeline::hashed::Table<glm::u8vec2> table_1 =
      nTiled::pipeline::hashed::Table<glm::u8vec2>(3);
    table_1.setPoint(glm::uvec3(2, 0, 0), glm::u8vec2(0, 0));
    table_1.setPoint(glm::uvec3(2, 2, 1), glm::u8vec2(0, 0));
    table_1.setPoint(glm::uvec3(2, 1, 2), glm::u8vec2(0, 0));

    nTiled::pipeline::hashed::Table<glm::u8vec2> table_2 =
      nTiled::pipeline::hashed::Table<glm::u8vec2>(3);
    table_2.setPoint(glm::uvec3(0, 0, 0), glm::u8vec2(0, 0));
    table_2.setPoint(glm::uvec3(1, 0, 0), glm::u8vec2(0, 0));
    table_2.setPoint(glm::uvec3(2, 0, 0), glm::u8vec2(0, 0));
    table_2.setPoint(glm::uvec3(0, 1, 0), glm::u8vec2(0, 0));
    table_2.setPoint(glm::uvec3(1, 1, 0), glm::u8vec2(0, 0));
    table_2.setPoint(glm::uvec3(2, 1, 0), glm::u8vec2(0, 0));
    table_2.setPoint(glm::uvec3(0, 2, 0), glm::u8vec2(0, 0));
    table_2.setPoint(glm::uvec3(1, 2, 0), glm::u8vec2(0, 0));
    table_2.setPoint(glm::uvec3(2, 2, 0), glm::u8vec2(0, 0));

    WHEN("A list of elements consisting of a single element is provided that does not collide") {
      glm::u8vec3 cand_1 = glm::u8vec3(0, 1, 1);
      glm::u8vec3 cand_2 = glm::u8vec3(0, 2, 1);
      glm::u8vec3 cand_3 = glm::u8vec3(2, 2, 2);

      std::vector<SpatialHashFunctionBuilder<glm::u8vec2>::EntryElement> elements_single = { 
        SpatialHashFunctionBuilder<glm::u8vec2>::EntryElement(glm::uvec3(0, 0, 0),
                                                              glm::uvec3(0, 0, 0),
                                                              glm::u8vec2(0)) };

      THEN("The candidate is valid and the method should return True") {
        REQUIRE(builder.isValidCandidate(cand_1,
                                         elements_single,
                                         table_empty));
        REQUIRE(builder.isValidCandidate(cand_2,
                                         elements_single,
                                         table_empty));
        REQUIRE(builder.isValidCandidate(cand_3,
                                         elements_single,
                                         table_empty));

        REQUIRE(builder.isValidCandidate(cand_1,
                                         elements_single,
                                         table_0));
        REQUIRE(builder.isValidCandidate(cand_2,
                                         elements_single,
                                         table_0));
        REQUIRE(builder.isValidCandidate(cand_3,
                                         elements_single,
                                         table_0));

        REQUIRE(builder.isValidCandidate(cand_1,
                                         elements_single,
                                         table_1));
        REQUIRE(builder.isValidCandidate(cand_2,
                                         elements_single,
                                         table_1));
        REQUIRE(builder.isValidCandidate(cand_3,
                                         elements_single,
                                         table_1));

        REQUIRE(builder.isValidCandidate(cand_1,
                                         elements_single,
                                         table_2));
        REQUIRE(builder.isValidCandidate(cand_2,
                                         elements_single,
                                         table_2));
        REQUIRE(builder.isValidCandidate(cand_3,
                                         elements_single,
                                         table_2));
      }
    }

    WHEN("Candidate is valid for a list of elements consisting of multiple elements") {
      glm::u8vec3 cand = glm::u8vec3(2, 1, 0);

      std::vector<glm::uvec3> elements_two_p = std::vector<glm::uvec3>();
      elements_two_p.push_back(glm::uvec3(1, 1, 2));
      elements_two_p.push_back(glm::uvec3(2, 1, 2));

      std::vector<SpatialHashFunctionBuilder<glm::u8vec2>::EntryElement> elements_two = {};
      for (glm::uvec3 p : elements_two_p) {
        elements_two.push_back(SpatialHashFunctionBuilder<glm::u8vec2>::EntryElement(p,
                                                                                   glm::uvec3(p.x % 3,
                                                                                              p.y % 3,
                                                                                              p.z % 3),
                                                                                   glm::u8vec2(0)));
      }


      std::vector<glm::uvec3> elements_three_p = std::vector<glm::uvec3>();
      elements_three_p.push_back(glm::uvec3(1, 1, 2));
      elements_three_p.push_back(glm::uvec3(2, 1, 2));
      elements_three_p.push_back(glm::uvec3(1, 0, 2));

      std::vector<SpatialHashFunctionBuilder<glm::u8vec2>::EntryElement> elements_three = {};
      for (glm::uvec3 p : elements_three_p) {
        elements_three.push_back(SpatialHashFunctionBuilder<glm::u8vec2>::EntryElement(p,
                                                                                   glm::uvec3(p.x % 3,
                                                                                              p.y % 3,
                                                                                              p.z % 3),
                                                                                   glm::u8vec2(0)));
      }

      std::vector<glm::uvec3> elements_six_p= std::vector<glm::uvec3>();
      elements_six_p.push_back(glm::uvec3(1, 1, 2));
      elements_six_p.push_back(glm::uvec3(2, 1, 2));
      elements_six_p.push_back(glm::uvec3(1, 0, 2));
      elements_six_p.push_back(glm::uvec3(2, 0, 2));
      elements_six_p.push_back(glm::uvec3(1, 0, 1));
      elements_six_p.push_back(glm::uvec3(1, 1, 1));

      std::vector<SpatialHashFunctionBuilder<glm::u8vec2>::EntryElement> elements_six = {};
      for (glm::uvec3 p : elements_six_p) {
        elements_six.push_back(SpatialHashFunctionBuilder<glm::u8vec2>::EntryElement(p,
                                                                                   glm::uvec3(p.x % 3,
                                                                                              p.y % 3,
                                                                                              p.z % 3),
                                                                                   glm::u8vec2(0)));
      }

      THEN("A the method should return True") {
        REQUIRE(builder.isValidCandidate(cand,
                                         elements_two,
                                         table_0));
        REQUIRE(builder.isValidCandidate(cand,
                                         elements_three,
                                         table_0));
        REQUIRE(builder.isValidCandidate(cand,
                                         elements_six,
                                         table_0));

        REQUIRE(builder.isValidCandidate(cand,
                                         elements_two,
                                         table_1));
        REQUIRE(builder.isValidCandidate(cand,
                                         elements_three,
                                         table_1));
        REQUIRE(builder.isValidCandidate(cand,
                                         elements_six,
                                         table_1));

        REQUIRE(builder.isValidCandidate(cand,
                                         elements_two,
                                         table_2));
        REQUIRE(builder.isValidCandidate(cand,
                                         elements_three,
                                         table_2));
        REQUIRE(builder.isValidCandidate(cand,
                                         elements_six,
                                         table_2));
      }
    }

    WHEN("Candidate is invalid for a single element in a list of elements consisting of a single element") {
      glm::u8vec3 cand_1 = glm::u8vec3(0, 2, 0);
      glm::u8vec3 cand_2 = glm::u8vec3(2, 2, 1);
      glm::u8vec3 cand_3 = glm::u8vec3(1, 2, 0);

      std::vector<SpatialHashFunctionBuilder<glm::u8vec2>::EntryElement> elements_single = { 
        SpatialHashFunctionBuilder<glm::u8vec2>::EntryElement(glm::uvec3(0, 0, 0),
                                                              glm::uvec3(0, 0, 0),
                                                              glm::u8vec2(0)) };

      THEN("A the method should return False") {
        REQUIRE_FALSE(builder.isValidCandidate(cand_1,
                                               elements_single,
                                               table_0));
        REQUIRE_FALSE(builder.isValidCandidate(cand_2,
                                               elements_single,
                                               table_1));
        REQUIRE_FALSE(builder.isValidCandidate(cand_3,
                                               elements_single,
                                               table_2));
      }
    }

    WHEN("Candidate is invalid for a single element in a list of elements consisting of multiple elements") {
      glm::u8vec3 cand = glm::u8vec3(2, 1, 0);

      std::vector<glm::uvec3> elements_two_p = std::vector<glm::uvec3>();
      elements_two_p.push_back(glm::uvec3(1, 1, 0));
      elements_two_p.push_back(glm::uvec3(2, 1, 2));

      std::vector<SpatialHashFunctionBuilder<glm::u8vec2>::EntryElement> elements_two = {};
      for (glm::uvec3 p : elements_two_p) {
        elements_two.push_back(SpatialHashFunctionBuilder<glm::u8vec2>::EntryElement(p,
                                                                                   glm::uvec3(p.x % 3,
                                                                                              p.y % 3,
                                                                                              p.z % 3),
                                                                                   glm::u8vec2(0)));
      }

      std::vector<glm::uvec3> elements_three_p = std::vector<glm::uvec3>();
      elements_three_p.push_back(glm::uvec3(1, 1, 0));
      elements_three_p.push_back(glm::uvec3(2, 1, 2));
      elements_three_p.push_back(glm::uvec3(1, 0, 2));

      std::vector<SpatialHashFunctionBuilder<glm::u8vec2>::EntryElement> elements_three = {};
      for (glm::uvec3 p : elements_three_p) {
        elements_three.push_back(SpatialHashFunctionBuilder<glm::u8vec2>::EntryElement(p,
                                                                                   glm::uvec3(p.x % 3,
                                                                                              p.y % 3,
                                                                                              p.z % 3),
                                                                                   glm::u8vec2(0)));
      }

      std::vector<glm::uvec3> elements_six_p= std::vector<glm::uvec3>();
      elements_six_p.push_back(glm::uvec3(1, 1, 0));
      elements_six_p.push_back(glm::uvec3(2, 1, 2));
      elements_six_p.push_back(glm::uvec3(1, 0, 2));
      elements_six_p.push_back(glm::uvec3(2, 0, 2));
      elements_six_p.push_back(glm::uvec3(1, 0, 1));
      elements_six_p.push_back(glm::uvec3(1, 1, 1));

      std::vector<SpatialHashFunctionBuilder<glm::u8vec2>::EntryElement> elements_six = {};
      for (glm::uvec3 p : elements_six_p) {
        elements_six.push_back(SpatialHashFunctionBuilder<glm::u8vec2>::EntryElement(p,
                                                                                   glm::uvec3(p.x % 3,
                                                                                              p.y % 3,
                                                                                              p.z % 3),
                                                                                   glm::u8vec2(0)));
      }

      THEN("A the method should return False") {
        REQUIRE_FALSE(builder.isValidCandidate(cand,
                                               elements_two,
                                               table_2));
        REQUIRE_FALSE(builder.isValidCandidate(cand,
                                               elements_three,                                         
                                               table_2));
        REQUIRE_FALSE(builder.isValidCandidate(cand,
                                               elements_six,
                                               table_2));
      }
    }

    WHEN("Candidate is invalid for multiple elements in a list of elements consisting of multiple elements") {
      glm::u8vec3 cand = glm::u8vec3(2, 1, 0);

      std::vector<glm::uvec3> elements_two_p = std::vector<glm::uvec3>();
      elements_two_p.push_back(glm::uvec3(1, 1, 0));
      elements_two_p.push_back(glm::uvec3(2, 1, 0));

      std::vector<SpatialHashFunctionBuilder<glm::u8vec2>::EntryElement> elements_two = {};
      for (glm::uvec3 p : elements_two_p) {
        elements_two.push_back(SpatialHashFunctionBuilder<glm::u8vec2>::EntryElement(p,
                                                                                   glm::uvec3(p.x % 3,
                                                                                              p.y % 3,
                                                                                              p.z % 3),
                                                                                   glm::u8vec2(0)));
      }

      std::vector<glm::uvec3> elements_three_p = std::vector<glm::uvec3>();
      elements_three_p.push_back(glm::uvec3(1, 1, 0));
      elements_three_p.push_back(glm::uvec3(2, 1, 0));
      elements_three_p.push_back(glm::uvec3(1, 0, 2));

      std::vector<SpatialHashFunctionBuilder<glm::u8vec2>::EntryElement> elements_three = {};
      for (glm::uvec3 p : elements_three_p) {
        elements_three.push_back(SpatialHashFunctionBuilder<glm::u8vec2>::EntryElement(p,
                                                                                   glm::uvec3(p.x % 3,
                                                                                              p.y % 3,
                                                                                              p.z % 3),
                                                                                   glm::u8vec2(0)));
      }

      std::vector<glm::uvec3> elements_six_p= std::vector<glm::uvec3>();
      elements_six_p.push_back(glm::uvec3(1, 1, 0));
      elements_six_p.push_back(glm::uvec3(2, 1, 0));
      elements_six_p.push_back(glm::uvec3(1, 0, 2));
      elements_six_p.push_back(glm::uvec3(2, 0, 2));
      elements_six_p.push_back(glm::uvec3(1, 0, 1));
      elements_six_p.push_back(glm::uvec3(1, 1, 1));

      std::vector<SpatialHashFunctionBuilder<glm::u8vec2>::EntryElement> elements_six = {};
      for (glm::uvec3 p : elements_six_p) {
        elements_six.push_back(SpatialHashFunctionBuilder<glm::u8vec2>::EntryElement(p,
                                                                                   glm::uvec3(p.x % 3,
                                                                                              p.y % 3,
                                                                                              p.z % 3),
                                                                                   glm::u8vec2(0)));
      }

      THEN("A the method should return False") {
        REQUIRE_FALSE(builder.isValidCandidate(cand,
                                               elements_two,
                                               table_2));
        REQUIRE_FALSE(builder.isValidCandidate(cand,
                                               elements_three,                                         
                                               table_2));
        REQUIRE_FALSE(builder.isValidCandidate(cand,
                                               elements_six,
                                               table_2));
      }
    }
  }
}

}
}
}
