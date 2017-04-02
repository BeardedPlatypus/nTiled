#include <catch.hpp>
#include "pipeline\light-management\hashed\HashedLightManager.h"
#include "pipeline\light-management\hashed\Exceptions.h"


SCENARIO("HashedLightManager::constructLinklessOctree should create a valid LinklessOctree when provided with a non empty world",
         "[LightOctreeFull][HashedLightManager][constructLinklessOctree]") {
  GIVEN("A set of worlds with varying number of lights") {
    double node_size = 3.0;
    std::vector<nTiled::world::World*> worlds = {};
    std::vector<nTiled::pipeline::hashed::HashedLightManager> managers = {};

    nTiled::world::World* w;

    std::string name = "just_testing_things";
    glm::vec3 intensity = glm::vec3(1.0);
    std::map<std::string, nTiled::world::Object*> empty_map =
      std::map<std::string, nTiled::world::Object*>();
    double radius = 20.0;

    for (unsigned int i = 1; i <= 5; ++i) {
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
      nTiled::pipeline::hashed::HashedConfig conf =
        nTiled::pipeline::hashed::HashedConfig(node_size,
                                               2,
                                               2.0,
                                               10);
      managers.push_back(nTiled::pipeline::hashed::HashedLightManager(*w, conf));
    }

    WHEN("constructLinklessOctree is called") {
      for (nTiled::pipeline::hashed::HashedLightManager& man : managers) {
        man.constructLinklessOctree();
      }

      THEN("Each light should fit into the newly constructed LinklessOctree") {
        for (unsigned int i = 0; i < 5; ++i) {
          nTiled::pipeline::hashed::HashedLightManager& mana = managers.at(i);
          const nTiled::pipeline::hashed::LinklessOctree& lo = *(mana.getLinklessOctree());
          unsigned int dim = lo.getTotalNNodes();
          glm::vec3 orig = lo.getOrigin();
          double width = lo.getWidth();

          glm::vec3 offset = glm::vec3(orig.x - 0.05 * width,
                                       orig.y - 0.05 * width,
                                       orig.z - 0.05 * width);
          double step_size = node_size * 0.75;

          std::vector<GLuint> indices;
          std::vector<nTiled::pipeline::hashed::SingleLightTree*> slts =
            mana.getSLTs();

          for (unsigned int x = 0; x < 2 * dim; ++x) {
            for (unsigned int y = 0; y < 2 * dim; ++y) {
              for (unsigned int z = 0; z < 2 * dim; ++z) {
                glm::vec3 p = offset + glm::vec3(step_size * x,
                                                 step_size * y,
                                                 step_size * z);
                indices = lo.retrieveLights(p);

                for (unsigned int j = 0; j < slts.size(); ++j) {
                  glm::vec3 slt_origin = slts.at(j)->getOrigin();
                  double slt_width = slts.at(j)->getWidth();
                  if (((p.x >= slt_origin.x) &&
                       (p.y >= slt_origin.y) &&
                       (p.z >= slt_origin.z) &&
                       (p.x <= slt_origin.x + slt_width) &&
                       (p.y <= slt_origin.y + slt_width) &&
                       (p.z <= slt_origin.z + slt_width)) &&
                      (slts.at(j)->isInLight(p))) {

                    if (std::find(indices.begin(),
                                  indices.end(),
                                  j) == indices.end()) {
                      indices = lo.retrieveLights(p);
                      bool durp = slts.at(j)->isInLight(p);
                      bool has_failed = true;
                    }

                    REQUIRE(std::find(indices.begin(),
                                      indices.end(),
                                      j) != indices.end());
                  } else {
                    if (std::find(indices.begin(),
                                  indices.end(),
                                  j) != indices.end()) {
                      indices = lo.retrieveLights(p);
                      bool durp = slts.at(j)->isInLight(p);
                      bool has_failed = true;
                    }

                    REQUIRE(std::find(indices.begin(),
                                      indices.end(),
                                      j) == indices.end());
                  }
                }
              }
            }
          }
        }
      }
    }
  }
}
