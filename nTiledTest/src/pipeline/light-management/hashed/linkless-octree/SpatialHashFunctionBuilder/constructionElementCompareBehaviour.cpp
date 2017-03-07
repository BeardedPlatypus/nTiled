#include <catch.hpp>
#include "pipeline\light-management\hashed\linkless-octree\SpatialHashFunctionBuilder.h"


// ----------------------------------------------------------------------------
//  ConstructionCompare Scenarios
// ----------------------------------------------------------------------------
SCENARIO("ConstructionElementCompare should return the ConstructionElement with the largest amount of EntryElements",
         "[LinklessOctreeFull][SpatialHashFunctionFull][SpatialHashFunctionBuilder]") {
  GIVEN("The ConstructionElementCompare") {
    nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::uvec2>::ConstructionElementCompare compare;
    WHEN("ConstructionElement A contains more EntryElements than ConstructionElement B") {
      nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::uvec2>::ConstructionElement A1 = 
        nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::uvec2>::ConstructionElement();
      A1.elements.push_back(nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::uvec2>::EntryElement(glm::uvec3(0,0,0),
                                                                                                           glm::uvec3(0,0,0),
                                                                                                           glm::uvec2(0, 0)));

      nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::uvec2>::ConstructionElement A5 = nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::uvec2>::ConstructionElement();
      for (unsigned int i = 0; i < 5; ++i) {
        A5.elements.push_back(nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::uvec2>::EntryElement(glm::uvec3(0,0,0),
                                                                                 glm::uvec3(0,0,0),
                                                                                 glm::uvec2(0, 0)));
      }

      nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::uvec2>::ConstructionElement A10 = nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::uvec2>::ConstructionElement();
      for (unsigned int i = 0; i < 10; ++i) {
        A10.elements.push_back(nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::uvec2>::EntryElement(glm::uvec3(0,0,0),
                                                                                  glm::uvec3(0,0,0),
                                                                                  glm::uvec2(0, 0)));
      }

      nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::uvec2>::ConstructionElement A20 = nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::uvec2>::ConstructionElement();
      for (unsigned int i = 0; i < 20; ++i) {
        A20.elements.push_back(nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::uvec2>::EntryElement(glm::uvec3(0,0,0),
                                                                                  glm::uvec3(0,0,0),
                                                                                  glm::uvec2(0, 0)));
      }


      nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::uvec2>::ConstructionElement A50 = nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::uvec2>::ConstructionElement();
      for (unsigned int i = 0; i < 50; ++i) {
        A50.elements.push_back(nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::uvec2>::EntryElement(glm::uvec3(0,0,0),
                                                                                  glm::uvec3(0,0,0),
                                                                                  glm::uvec2(0, 0)));
      }


      nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::uvec2>::ConstructionElement B0 = nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::uvec2>::ConstructionElement();

      nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::uvec2>::ConstructionElement B1 = nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::uvec2>::ConstructionElement();
      for (unsigned int i = 0; i < 1; ++i) {
        B1.elements.push_back(nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::uvec2>::EntryElement(glm::uvec3(0,0,0),
                                                                                  glm::uvec3(0,0,0),
                                                                                  glm::uvec2(0, 0)));
      }

      nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::uvec2>::ConstructionElement B4 = nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::uvec2>::ConstructionElement();
      for (unsigned int i = 0; i < 4; ++i) {
        B1.elements.push_back(nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::uvec2>::EntryElement(glm::uvec3(0,0,0),
                                                                                  glm::uvec3(0,0,0),
                                                                                  glm::uvec2(0, 0)));
      }

      nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::uvec2>::ConstructionElement B9 = nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::uvec2>::ConstructionElement();
      for (unsigned int i = 0; i < 9; ++i) {
        B9.elements.push_back(nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::uvec2>::EntryElement(glm::uvec3(0,0,0),
                                                                                  glm::uvec3(0,0,0),
                                                                                  glm::uvec2(0, 0)));
      }

      nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::uvec2>::ConstructionElement B49 = nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::uvec2>::ConstructionElement();
      for (unsigned int i = 0; i < 49; ++i) {
        B49.elements.push_back(nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::uvec2>::EntryElement(glm::uvec3(0,0,0),
                                                                                  glm::uvec3(0,0,0),
                                                                                  glm::uvec2(0, 0)));
      }

      THEN("A should be bigger or equal to B") {
        REQUIRE(compare(A1, B0));
        REQUIRE(compare(A5, B0));
        REQUIRE(compare(A5, B4));
        REQUIRE(compare(A10, B4));
        REQUIRE(compare(A10, B9));
        REQUIRE(compare(A20, B9));
        REQUIRE(compare(A50, B9));
        REQUIRE(compare(A50, B49));
      }
    }
    
    WHEN("ConstructionElement A contains the same number EntryElements as ConstructionElement B") {
      nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::uvec2>::ConstructionElement A1 = nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::uvec2>::ConstructionElement();
      A1.elements.push_back(nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::uvec2>::EntryElement(glm::uvec3(0,0,0),
                                                                               glm::uvec3(0,0,0),
                                                                               glm::uvec2(0, 0)));

      nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::uvec2>::ConstructionElement A5 = nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::uvec2>::ConstructionElement();
      for (unsigned int i = 0; i < 5; ++i) {
        A5.elements.push_back(nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::uvec2>::EntryElement(glm::uvec3(0,0,0),
                                                                                 glm::uvec3(0,0,0),
                                                                                 glm::uvec2(0, 0)));
      }

      nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::uvec2>::ConstructionElement A10 = nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::uvec2>::ConstructionElement();
      for (unsigned int i = 0; i < 10; ++i) {
        A10.elements.push_back(nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::uvec2>::EntryElement(glm::uvec3(0,0,0),
                                                                                  glm::uvec3(0,0,0),
                                                                                  glm::uvec2(0, 0)));
      }

      nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::uvec2>::ConstructionElement A20 = nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::uvec2>::ConstructionElement();
      for (unsigned int i = 0; i < 20; ++i) {
        A20.elements.push_back(nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::uvec2>::EntryElement(glm::uvec3(0,0,0),
                                                                                  glm::uvec3(0,0,0),
                                                                                  glm::uvec2(0, 0)));
      }


      nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::uvec2>::ConstructionElement A50 = nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::uvec2>::ConstructionElement();
      for (unsigned int i = 0; i < 50; ++i) {
        A50.elements.push_back(nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::uvec2>::EntryElement(glm::uvec3(0,0,0),
                                                                                  glm::uvec3(0,0,0),
                                                                                  glm::uvec2(0, 0)));
      }

      nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::uvec2>::ConstructionElement B1 = nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::uvec2>::ConstructionElement();
      B1.elements.push_back(nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::uvec2>::EntryElement(glm::uvec3(0,0,0),
                                                                               glm::uvec3(0,0,0),
                                                                               glm::uvec2(0, 0)));

      nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::uvec2>::ConstructionElement B5 = nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::uvec2>::ConstructionElement();
      for (unsigned int i = 0; i < 5; ++i) {
        B5.elements.push_back(nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::uvec2>::EntryElement(glm::uvec3(0,0,0),
                                                                                 glm::uvec3(0,0,0),
                                                                                 glm::uvec2(0, 0)));
      }

      nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::uvec2>::ConstructionElement B10 = nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::uvec2>::ConstructionElement();
      for (unsigned int i = 0; i < 10; ++i) {
        B10.elements.push_back(nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::uvec2>::EntryElement(glm::uvec3(0,0,0),
                                                                                  glm::uvec3(0,0,0),
                                                                                  glm::uvec2(0, 0)));
      }

      nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::uvec2>::ConstructionElement B20 = nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::uvec2>::ConstructionElement();
      for (unsigned int i = 0; i < 20; ++i) {
        B20.elements.push_back(nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::uvec2>::EntryElement(glm::uvec3(0,0,0),
                                                                                  glm::uvec3(0,0,0),
                                                                                  glm::uvec2(0, 0)));
      }


      nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::uvec2>::ConstructionElement B50 = nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::uvec2>::ConstructionElement();
      for (unsigned int i = 0; i < 50; ++i) {
        B50.elements.push_back(nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::uvec2>::EntryElement(glm::uvec3(0,0,0),
                                                                                  glm::uvec3(0,0,0),
                                                                                  glm::uvec2(0, 0)));
      }

      THEN("A should be bigger or equal to B") {
        REQUIRE(compare(A1, B1));
        REQUIRE(compare(A5, B5));
        REQUIRE(compare(A10, B10));
        REQUIRE(compare(A20, B20));
        REQUIRE(compare(A50, B50));
      }
    }

    WHEN("ConstructionElement A contains less EntryElements than ConstructionElement B") {
      nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::uvec2>::ConstructionElement A0 = nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::uvec2>::ConstructionElement();

      nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::uvec2>::ConstructionElement A4 = nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::uvec2>::ConstructionElement();
      for (unsigned int i = 0; i < 4; ++i) {
        A4.elements.push_back(nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::uvec2>::EntryElement(glm::uvec3(0,0,0),
                                                                                 glm::uvec3(0,0,0),
                                                                                 glm::uvec2(0, 0)));
      }

      nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::uvec2>::ConstructionElement A9 = nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::uvec2>::ConstructionElement();
      for (unsigned int i = 0; i < 9; ++i) {
        A9.elements.push_back(nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::uvec2>::EntryElement(glm::uvec3(0,0,0),
                                                                                  glm::uvec3(0,0,0),
                                                                                  glm::uvec2(0, 0)));
      }

      nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::uvec2>::ConstructionElement A19 = nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::uvec2>::ConstructionElement();
      for (unsigned int i = 0; i < 19; ++i) {
        A19.elements.push_back(nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::uvec2>::EntryElement(glm::uvec3(0,0,0),
                                                                                  glm::uvec3(0,0,0),
                                                                                  glm::uvec2(0, 0)));
      }


      nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::uvec2>::ConstructionElement A49 = nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::uvec2>::ConstructionElement();
      for (unsigned int i = 0; i < 49; ++i) {
        A49.elements.push_back(nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::uvec2>::EntryElement(glm::uvec3(0,0,0),
                                                                                  glm::uvec3(0,0,0),
                                                                                  glm::uvec2(0, 0)));
      }

      nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::uvec2>::ConstructionElement B1 = nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::uvec2>::ConstructionElement();
      B1.elements.push_back(nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::uvec2>::EntryElement(glm::uvec3(0,0,0),
                                                                               glm::uvec3(0,0,0),
                                                                               glm::uvec2(0, 0)));

      nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::uvec2>::ConstructionElement B5 = nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::uvec2>::ConstructionElement();
      for (unsigned int i = 0; i < 5; ++i) {
        B5.elements.push_back(nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::uvec2>::EntryElement(glm::uvec3(0,0,0),
                                                                                 glm::uvec3(0,0,0),
                                                                                 glm::uvec2(0, 0)));
      }

      nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::uvec2>::ConstructionElement B10 = nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::uvec2>::ConstructionElement();
      for (unsigned int i = 0; i < 10; ++i) {
        B10.elements.push_back(nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::uvec2>::EntryElement(glm::uvec3(0,0,0),
                                                                                  glm::uvec3(0,0,0),
                                                                                  glm::uvec2(0, 0)));
      }

      nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::uvec2>::ConstructionElement B20 = nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::uvec2>::ConstructionElement();
      for (unsigned int i = 0; i < 20; ++i) {
        B20.elements.push_back(nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::uvec2>::EntryElement(glm::uvec3(0,0,0),
                                                                                  glm::uvec3(0,0,0),
                                                                                  glm::uvec2(0, 0)));
      }


      nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::uvec2>::ConstructionElement B50 = nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::uvec2>::ConstructionElement();
      for (unsigned int i = 0; i < 50; ++i) {
        B50.elements.push_back(nTiled::pipeline::hashed::SpatialHashFunctionBuilder<glm::uvec2>::EntryElement(glm::uvec3(0,0,0),
                                                                                  glm::uvec3(0,0,0),
                                                                                  glm::uvec2(0, 0)));
      }
      THEN("B should be returned") {
        REQUIRE(compare(A0, B1));
        REQUIRE(compare(A4, B5));
        REQUIRE(compare(A9, B10));
        REQUIRE(compare(A19, B20));
        REQUIRE(compare(A49, B50));
      }
    }
  }
}