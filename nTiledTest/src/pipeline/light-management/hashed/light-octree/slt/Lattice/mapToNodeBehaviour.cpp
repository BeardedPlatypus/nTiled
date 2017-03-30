#include <catch.hpp>
#include "pipeline\light-management\hashed\light-octree\slt\Lattice.h"
#include "pipeline\light-management\hashed\light-octree\slt\SingleLightTreeBuilder.h"
#include "pipeline\light-management\hashed\light-octree\slt\Exceptions.h"
#include "math\octree.h"


// ----------------------------------------------------------------------------
//  getClosestNodeBehaviour
// ----------------------------------------------------------------------------
SCENARIO("mapToNode should throw a SLTOutOfLatticeBoundsException when a point is sampled that is outside the Lattice",
         "[LightOctreeFull][SLTFull][Lattice]") {
  GIVEN("A series of lattices") {
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
    WHEN("The point is outside the lattice") {
      glm::vec3 point_s;
      glm::vec3 point_b;
      glm::uvec3 result;
      THEN("mapToNode should throw a SLTOutOfLatticeBoundsException") {
        for (unsigned int i = 0; i < 20; i++) {
          double lattice_size = lattices.at(i)->getWidth();
          glm::vec3 lattice_origin = lattices.at(i)->getOrigin();
          for (unsigned int x_i = 0; x_i < 5; x_i++) {
            for (unsigned int y_i = 0; y_i < 5; y_i++) {
              for (unsigned int z_i = 0; z_i < 5; z_i++) {
                point_s = lattice_origin - glm::vec3(x_i * 0.84 + 0.1,
                                                     y_i * 0.34 + 0.1,
                                                     z_i * 0.67 + 0.1);
                point_b = lattice_origin + glm::vec3(x_i * 0.84 + lattice_size + 0.1,
                                                     y_i * 0.34 + lattice_size + 0.1,
                                                     z_i * 0.67 + lattice_size + 0.1);

                REQUIRE_THROWS_AS(lattices.at(i)->mapToNode(point_s),
                                  nTiled::pipeline::hashed::SLTOutOfLatticeBoundsException);
                REQUIRE_THROWS_AS(lattices.at(i)->mapToNode(point_b),
                                  nTiled::pipeline::hashed::SLTOutOfLatticeBoundsException);
                                
              }
            }
          }
        }
      }
    }
  }
}


SCENARIO("mapToNode should return the index of the node it is in when provided with a point within the lattice",
         "[LightOctreeFull][SLTFull][Lattice]") {
  GIVEN("A series of lattices") {
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
    for (unsigned int i = 1; i <= 10; ++i) {
      indices.push_back(i);

      radius = 0.5 * builder.getMinimalNodeSize() * i - 0.1;
      light_position = builder.getOriginOctree() + glm::vec3(0.5 * builder.getMinimalNodeSize() * i);

      lights.push_back(nTiled::world::PointLight(name,
                                                 glm::vec4(light_position, 1.0),
                                                 intensity,
                                                 radius,
                                                 true,
                                                 empty_map));
    }

    for (const nTiled::world::PointLight l : lights) {
      lattices.push_back(builder.constructLattice(l));
    }

    WHEN("The point is inside the lattice") {
      glm::vec3 point;
      glm::uvec3 result;
      double node_size = builder.getMinimalNodeSize();
      THEN("mapToNode should return the correct index") {
        for (unsigned int i = 0; i < 10; i++) {
          double lattice_size = lattices.at(i)->getWidth();
          glm::vec3 lattice_origin = lattices.at(i)->getOrigin();
          for (unsigned int x_i = 0; x_i < i; x_i++) {
            for (unsigned int y_i = 0; y_i < i; y_i++) {
              for (unsigned int z_i = 0; z_i < i; z_i++) {
                for (unsigned int j = 1; j <= 6; j++) {
                  point = lattice_origin + glm::vec3(x_i * node_size + j * 0.15 * node_size,
                                                     y_i * node_size + j * 0.15 * node_size,
                                                     z_i * node_size + j * 0.15 * node_size);

                  result = lattices.at(i)->mapToNode(point);

                  REQUIRE(result.x == x_i);
                  REQUIRE(result.y == y_i);
                  REQUIRE(result.z == z_i);
                }
              }
            }
          }
        }
      }
    }
  }
}
