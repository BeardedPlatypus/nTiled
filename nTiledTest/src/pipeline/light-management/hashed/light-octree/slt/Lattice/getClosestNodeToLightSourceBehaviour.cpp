#include <catch.hpp>
#include "pipeline\light-management\hashed\light-octree\slt\Lattice.h"
#include "pipeline\light-management\hashed\light-octree\slt\SingleLightTreeBuilder.h"
#include "pipeline\light-management\hashed\light-octree\slt\Exceptions.h"
#include "math\octree.h"


// ----------------------------------------------------------------------------
//  getClosestNodeBehaviour
// ----------------------------------------------------------------------------
SCENARIO("getClosestNodeBehaviour should throw a SLTOutOfLatticeBoundsException when a cube is returned which does not partly cover the Lattice",
         "[LightOctreeFull][SLTFull][Lattice]") {
  GIVEN("A series of lattices") {
    nTiled::pipeline::hashed::SingleLightTreeBuilder builder =
      nTiled::pipeline::hashed::SingleLightTreeBuilder(0.5,
                                                       glm::vec3(0.0, 0.0, 0.0));

    std::string name = "just_testing_things";
    glm::vec3 intensity = glm::vec3(1.0);
    std::map<std::string, nTiled::world::Object*> empty_map =
      std::map<std::string, nTiled::world::Object*>();

    nTiled::world::PointLight light_1 =
      nTiled::world::PointLight(name,
                                glm::vec4(5.0, 5.0, 5.0, 1.0),
                                intensity,
                                1.8,
                                true,
                                empty_map);
    nTiled::world::PointLight light_2 = 
      nTiled::world::PointLight(name,
                                glm::vec4(2.0, 2.0, 2.0, 1.0),
                                intensity,
                                0.4,
                                true,
                                empty_map);
    nTiled::world::PointLight light_3 =
      nTiled::world::PointLight(name,
                                glm::vec4(1.7, 1.7, 1.7, 1.0),
                                intensity,
                                0.2,
                                true,
                                empty_map);

    nTiled::pipeline::hashed::Lattice* lattice_1 = builder.constructLattice(light_1);
    nTiled::pipeline::hashed::Lattice* lattice_2 = builder.constructLattice(light_2);
    nTiled::pipeline::hashed::Lattice* lattice_3 = builder.constructLattice(light_3);

    WHEN("The upperbound of the cube is smaller than lattice origin") {
      glm::vec3 lowerbound_1 = glm::vec3(-1.0, -1.0, -1.0);
      glm::vec3 lowerbound_2 = glm::vec3(0.0, 0.0, -1.0);
      glm::vec3 lowerbound_3 = glm::vec3(0.0, -1.0, 0.0);
      glm::vec3 lowerbound_4 = glm::vec3(-1.0, 0.0, 0.0);

      THEN("getClosestNodeBehaviour should throw a SLTOutOfLatticeBoundsException") {
        REQUIRE_THROWS_AS(lattice_1->getClosestNodeToLightSource(lowerbound_1, 1.0),
                       nTiled::pipeline::hashed::SLTOutOfLatticeBoundsException);
        REQUIRE_THROWS_AS(lattice_2->getClosestNodeToLightSource(lowerbound_1, 1.0),
                       nTiled::pipeline::hashed::SLTOutOfLatticeBoundsException);
        REQUIRE_THROWS_AS(lattice_3->getClosestNodeToLightSource(lowerbound_1, 1.0),
                       nTiled::pipeline::hashed::SLTOutOfLatticeBoundsException);

        REQUIRE_THROWS_AS(lattice_1->getClosestNodeToLightSource(lowerbound_2, 1.0),
                       nTiled::pipeline::hashed::SLTOutOfLatticeBoundsException);
        REQUIRE_THROWS_AS(lattice_2->getClosestNodeToLightSource(lowerbound_2, 1.0),
                       nTiled::pipeline::hashed::SLTOutOfLatticeBoundsException);
        REQUIRE_THROWS_AS(lattice_3->getClosestNodeToLightSource(lowerbound_2, 1.0),
                       nTiled::pipeline::hashed::SLTOutOfLatticeBoundsException);

        REQUIRE_THROWS_AS(lattice_1->getClosestNodeToLightSource(lowerbound_3, 1.0),
                       nTiled::pipeline::hashed::SLTOutOfLatticeBoundsException);
        REQUIRE_THROWS_AS(lattice_2->getClosestNodeToLightSource(lowerbound_3, 1.0),
                       nTiled::pipeline::hashed::SLTOutOfLatticeBoundsException);
        REQUIRE_THROWS_AS(lattice_3->getClosestNodeToLightSource(lowerbound_3, 1.0),
                       nTiled::pipeline::hashed::SLTOutOfLatticeBoundsException);

        REQUIRE_THROWS_AS(lattice_1->getClosestNodeToLightSource(lowerbound_4, 1.0),
                       nTiled::pipeline::hashed::SLTOutOfLatticeBoundsException);
        REQUIRE_THROWS_AS(lattice_2->getClosestNodeToLightSource(lowerbound_4, 1.0),
                       nTiled::pipeline::hashed::SLTOutOfLatticeBoundsException);
        REQUIRE_THROWS_AS(lattice_3->getClosestNodeToLightSource(lowerbound_4, 1.0),
                       nTiled::pipeline::hashed::SLTOutOfLatticeBoundsException);
      }
    }

    WHEN("The lowerbound of the cube is greater than lattice origin") {
      glm::vec3 lowerbound_1 = glm::vec3(10.0, 10.0, 10.0);
      glm::vec3 lowerbound_2 = glm::vec3(0.0, 0.0, 10.0);
      glm::vec3 lowerbound_3 = glm::vec3(0.0, 10.0, 0.0);
      glm::vec3 lowerbound_4 = glm::vec3(10.0, 0.0, 0.0);

      THEN("getClosestNodeBehaviour should throw a SLTOutOfLatticeBoundsException") {
        REQUIRE_THROWS_AS(lattice_1->getClosestNodeToLightSource(lowerbound_1, 1.0),
                       nTiled::pipeline::hashed::SLTOutOfLatticeBoundsException);
        REQUIRE_THROWS_AS(lattice_2->getClosestNodeToLightSource(lowerbound_1, 1.0),
                       nTiled::pipeline::hashed::SLTOutOfLatticeBoundsException);
        REQUIRE_THROWS_AS(lattice_3->getClosestNodeToLightSource(lowerbound_1, 1.0),
                       nTiled::pipeline::hashed::SLTOutOfLatticeBoundsException);

        REQUIRE_THROWS_AS(lattice_1->getClosestNodeToLightSource(lowerbound_2, 1.0),
                       nTiled::pipeline::hashed::SLTOutOfLatticeBoundsException);
        REQUIRE_THROWS_AS(lattice_2->getClosestNodeToLightSource(lowerbound_2, 1.0),
                       nTiled::pipeline::hashed::SLTOutOfLatticeBoundsException);
        REQUIRE_THROWS_AS(lattice_3->getClosestNodeToLightSource(lowerbound_2, 1.0),
                       nTiled::pipeline::hashed::SLTOutOfLatticeBoundsException);

        REQUIRE_THROWS_AS(lattice_1->getClosestNodeToLightSource(lowerbound_3, 1.0),
                       nTiled::pipeline::hashed::SLTOutOfLatticeBoundsException);
        REQUIRE_THROWS_AS(lattice_2->getClosestNodeToLightSource(lowerbound_3, 1.0),
                       nTiled::pipeline::hashed::SLTOutOfLatticeBoundsException);
        REQUIRE_THROWS_AS(lattice_3->getClosestNodeToLightSource(lowerbound_3, 1.0),
                       nTiled::pipeline::hashed::SLTOutOfLatticeBoundsException);

        REQUIRE_THROWS_AS(lattice_1->getClosestNodeToLightSource(lowerbound_4, 1.0),
                       nTiled::pipeline::hashed::SLTOutOfLatticeBoundsException);
        REQUIRE_THROWS_AS(lattice_2->getClosestNodeToLightSource(lowerbound_4, 1.0),
                       nTiled::pipeline::hashed::SLTOutOfLatticeBoundsException);
        REQUIRE_THROWS_AS(lattice_3->getClosestNodeToLightSource(lowerbound_4, 1.0),
                       nTiled::pipeline::hashed::SLTOutOfLatticeBoundsException);
      }
    }
    delete lattice_1;
    delete lattice_2;
    delete lattice_3;
  }
}


SCENARIO("getClosestNodeBehaviour should return the value of the node closest to the light source when provided with a correct cube",
         "[LightOctreeFull][SLTFull][Lattice]") {
  GIVEN("a SLTBuilder and a set of lattices") {
    nTiled::pipeline::hashed::SingleLightTreeBuilder builder =
      nTiled::pipeline::hashed::SingleLightTreeBuilder(0.5,
                                                       glm::vec3(5.0, 3.0, 1.0));

    std::string name = "just_testing_things";
    glm::vec3 intensity = glm::vec3(1.0);
    std::map<std::string, nTiled::world::Object*> empty_map =
      std::map<std::string, nTiled::world::Object*>();

    std::vector<nTiled::world::PointLight> lights = {};
    std::vector<nTiled::pipeline::hashed::Lattice*> lattices = {};
    std::vector<unsigned int> indices = {};

    glm::vec3 light_position;
    double radius;
    for (unsigned int i = 1; i <= 20; ++i) {
      indices.push_back(i);

      radius = 0.5 * builder.getMinimalNodeSize() * i - 0.1;
      light_position = builder.getOriginOctree() + glm::vec3(0.5 * builder.getMinimalNodeSize() * i);

      lights.push_back(nTiled::world::PointLight(name,
                                                 glm::vec4(light_position, 1.0),
                                                 intensity,
                                                 radius,
                                                 true,
                                                 empty_map));

      lattices.push_back(builder.constructLattice(lights.back()));
    }

    WHEN("A cube smaller than the minimal node is provided") {
      glm::vec3 octree_origin = builder.getOriginOctree();

      glm::uvec3 result;
      THEN("The index of the node in which the cube resides is returned") {
        glm::vec3 cube_origin;
        for (unsigned int i = 0; i < 20; i++) {
          for (unsigned int x_i = 0; x_i < indices.at(i); x_i++) {
            for (unsigned int y_i = 0; y_i < indices.at(i); y_i++) {
              for (unsigned int z_i = 0; z_i < indices.at(i); z_i++) {
                cube_origin = lattices.at(i)->getOrigin() + glm::vec3(x_i * builder.getMinimalNodeSize() + 0.1,
                                                                      y_i * builder.getMinimalNodeSize() + 0.1,
                                                                      z_i * builder.getMinimalNodeSize() + 0.1);
                result = lattices.at(i)->getClosestNodeToLightSource(cube_origin, 0.2);

                REQUIRE(result.x == x_i);
                REQUIRE(result.y == y_i);
                REQUIRE(result.z == z_i);
              }
            }
          }
        }
      }
    }

    WHEN("A cube consisting of multiple nodes is provided") {
      glm::vec3 octree_origin = builder.getOriginOctree();
      glm::uvec3 result;
      THEN("The value of the node closest to the origin is returned") {
        for (unsigned int i = 0; i < 20; i++) {
          for (unsigned int j = 1; j <= unsigned int(floor(indices.at(i) * 0.5)); j++) {
            result = lattices.at(i)->getClosestNodeToLightSource(lattices.at(i)->getOrigin(), 
                                                                 j * builder.getMinimalNodeSize() - 0.05);
            REQUIRE(result.x == j - 1);
            REQUIRE(result.y == j - 1);
            REQUIRE(result.z == j - 1);
          }
        }
      }
    }

    WHEN("A cube of the width and origin similar to the lattice is provided") {
      glm::vec3 octree_origin = builder.getOriginOctree();
      glm::uvec3 result;
      THEN("Then the center node should be returned") {
        for (unsigned int i = 0; i < 20; i++) {
          result = lattices.at(i)->getClosestNodeToLightSource(lattices.at(i)->getOrigin(), lattices.at(i)->getWidth());

          REQUIRE(result.x == unsigned int(floor(lattices.at(i)->getNumberNodesDim() * 0.5)));
          REQUIRE(result.y == unsigned int(floor(lattices.at(i)->getNumberNodesDim() * 0.5)));
          REQUIRE(result.z == unsigned int(floor(lattices.at(i)->getNumberNodesDim() * 0.5)));
        }
      }
    }

    WHEN("A cube partly covering the lattice is provided") {
      glm::vec3 octree_origin = builder.getOriginOctree();
      glm::uvec3 result;
      THEN("The value of the node closest to the origin is returned") {
        for (unsigned int i = 0; i < 20; i++) {
          for (unsigned int j = 1; j <= unsigned int(floor(indices.at(i) * 0.5)); j++) {
            result = lattices.at(i)->getClosestNodeToLightSource(lattices.at(i)->getOrigin() - glm::vec3(1.0), 
                                                                 j * builder.getMinimalNodeSize() + 0.9);
            REQUIRE(result.x == j - 1);
            REQUIRE(result.y == j - 1);
            REQUIRE(result.z == j - 1);
          }
        }
      }
    }

    WHEN("A cube completely covers the lattice") {
      glm::vec3 octree_origin = builder.getOriginOctree();
      glm::uvec3 result;
      THEN("The value of the center node should be returned") {
        for (unsigned int i = 0; i < 20; i++) {
          result = lattices.at(i)->getClosestNodeToLightSource(lattices.at(i)->getOrigin() - glm::vec3(3.0), lattices.at(i)->getWidth() + 6.0);

          REQUIRE(result.x == unsigned int(floor(lattices.at(i)->getNumberNodesDim() * 0.5)));
          REQUIRE(result.y == unsigned int(floor(lattices.at(i)->getNumberNodesDim() * 0.5)));
          REQUIRE(result.z == unsigned int(floor(lattices.at(i)->getNumberNodesDim() * 0.5)));
        }
      }
    }

    for (nTiled::pipeline::hashed::Lattice* p_lattice : lattices) {
      delete p_lattice;
    }
  }
}
