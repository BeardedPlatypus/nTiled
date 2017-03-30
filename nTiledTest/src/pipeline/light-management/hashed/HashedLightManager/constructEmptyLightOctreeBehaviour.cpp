#include <catch.hpp>
#include "pipeline\light-management\hashed\HashedLightManager.h"
#include "pipeline\light-management\hashed\Exceptions.h"


SCENARIO("HashedLightManager::constructLightOctree should throw an exception when invalid arguments are provided",
         "[LightOctreeFull][HashedLightManager]") {
  GIVEN("A new HashedLightManager") {
    nTiled::world::World world;
    double node_size = 0.5;

    nTiled::pipeline::hashed::HashedLightManager manager =
      nTiled::pipeline::hashed::HashedLightManager(world, 
                                                   node_size);

    WHEN("constructLightoctree is called with an empty light vector") {
      THEN("An exception is thrown") {
        REQUIRE_THROWS_AS(manager.constructEmptyLightOctree(),
                          nTiled::pipeline::hashed::HashedShadingNoLightException);

      }
    }
  }
}



SCENARIO("HashedLightManager::constructEmptyLightOctree should create a valid empty LightOctree when provided with a non empty world",
         "[LightOctreeFull][HashedLightManager]") {
  GIVEN("A set of worlds with varying number of lights") {
    double node_size = 1.0;
    std::vector<nTiled::world::World*> worlds = {};
    std::vector<nTiled::pipeline::hashed::HashedLightManager> managers = {};

    nTiled::world::World* w;

    std::string name = "just_testing_things";
    glm::vec3 intensity = glm::vec3(1.0);
    std::map<std::string, nTiled::world::Object*> empty_map =
      std::map<std::string, nTiled::world::Object*>();
    double radius = 20.0;

    for (unsigned int i = 1; i <= 20; ++i) {
      w = new nTiled::world::World();

      for (unsigned int x = 0; x < i; ++x) {
        for (unsigned int y = 0; y < i; ++y) {
          for (unsigned int z = 0; z < i; ++z) {
            glm::vec4 position = glm::vec4(x * 10.0,
                                           y * 12.0,
                                           z * 14.0,
                                           1.0);
            w->constructPointLight(name, 
                                  position,
                                  intensity,
                                  radius,
                                  true,
                                  empty_map);
          }
        }
      }

      worlds.push_back(w);
      managers.push_back(nTiled::pipeline::hashed::HashedLightManager(*w, node_size));
    }

    WHEN("constructLightOctree is called") {
      for (nTiled::pipeline::hashed::HashedLightManager& man : managers)
        man.constructEmptyLightOctree();

      THEN("Each light should fit into the newly constructed LightOctree") {
        for (unsigned int i = 0; i < 20; ++i) {
          nTiled::pipeline::hashed::HashedLightManager& mana = managers.at(i);
          nTiled::world::World& wor = *worlds.at(i);
          const nTiled::pipeline::hashed::LightOctree& lo = *(mana.getLightOctree());
          glm::vec3 orig_lo = lo.getOrigin();
          double width = lo.getWidth();

          for (nTiled::world::PointLight* p : wor.p_lights) {
            REQUIRE(p->position.x - p->radius >= orig_lo.x);
            REQUIRE(p->position.y - p->radius >= orig_lo.y);
            REQUIRE(p->position.z - p->radius >= orig_lo.z);

            REQUIRE(p->position.x + p->radius <= orig_lo.x + width);
            REQUIRE(p->position.y + p->radius <= orig_lo.y + width);
            REQUIRE(p->position.z + p->radius <= orig_lo.z + width);
          }
        }
      }
    }
  }
}