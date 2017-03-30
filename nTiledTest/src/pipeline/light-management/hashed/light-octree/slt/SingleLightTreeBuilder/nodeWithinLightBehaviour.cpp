#include <catch.hpp>

#include "pipeline\light-management\hashed\light-octree\slt\SingleLightTreeBuilder.h"

// ----------------------------------------------------------------------------
//  lightWithinNodes Scenarios
// ----------------------------------------------------------------------------
// TODO add more specific cases in case this does not work
SCENARIO("lightWithNodes should return whether a light (partly) covers a node.",
         "[LightOctreeFull][SLTFull][SLTBuilder]") {
  GIVEN("A SLTBuilder and centers defined in each octant") {
    const double size = 0.5;
    const glm::vec3 origin = glm::vec3(-2.0, -3.0, -4.0);

    nTiled::pipeline::hashed::SingleLightTreeBuilder builder =
      nTiled::pipeline::hashed::SingleLightTreeBuilder(size, origin);

    glm::vec4 point_nnn = glm::vec4(-1.5, -2.5, -3.5, 1.0);
    glm::vec4 point_pnn = glm::vec4(1.5, -2.5, -3.5, 1.0);
    glm::vec4 point_npn = glm::vec4(-1.5, 2.5, -3.5, 1.0);
    glm::vec4 point_ppn = glm::vec4(1.5, 2.5, -3.5, 1.0);
    glm::vec4 point_nnp = glm::vec4(-1.5, -2.5, 3.5, 1.0);
    glm::vec4 point_pnp = glm::vec4(1.5, -2.5, 3.5, 1.0);
    glm::vec4 point_npp = glm::vec4(-1.5, 2.5, 3.5, 1.0);
    glm::vec4 point_ppp = glm::vec4(1.5, 2.5, 3.5, 1.0);

    std::vector<glm::vec4> points = {
      point_nnn,
      point_pnn,
      point_npn,
      point_ppn,
      point_nnp,
      point_pnp,
      point_npp,
      point_ppp,
    };

    glm::vec3 q_nnn = glm::vec3(-1.0, -1.0, -1.0);
    glm::vec3 q_pnn = glm::vec3( 1.0, -1.0, -1.0);
    glm::vec3 q_npn = glm::vec3(-1.0,  1.0, -1.0);
    glm::vec3 q_ppn = glm::vec3( 1.0,  1.0, -1.0);
    glm::vec3 q_nnp = glm::vec3(-1.0, -1.0,  1.0);
    glm::vec3 q_pnp = glm::vec3( 1.0, -1.0,  1.0);
    glm::vec3 q_npp = glm::vec3(-1.0,  1.0,  1.0);
    glm::vec3 q_ppp = glm::vec3( 1.0,  1.0,  1.0);

    WHEN("A light is provided that does not cover a node") {
      float slt_size = 2.0f;


      double radius = 0.85;
      std::string name = "just_testing_things";
      glm::vec3 intensity = glm::vec3(1.0f);
      std::map<std::string, nTiled::world::Object*> empty_map =
        std::map<std::string, nTiled::world::Object*>();

      // ----------------------------------------------------------------------
      std::vector<float> x_q_nnn = { -2.0, 0.0, 2.0 };
      std::vector<float> y_q_nnn = { -2.0, 0.0, 2.0 };
      std::vector<float> z_q_nnn = { -2.0, 0.0, 2.0 };

      std::vector<nTiled::world::PointLight> lights_q_nnn = {};
      for (unsigned int x_i = 0; x_i < 2; x_i++) {
        for (unsigned int y_i = 0; y_i < 2; y_i++) {
          for (unsigned int z_i = 0; z_i < 2; z_i++) {
            if (!(x_i == 1 && y_i == 1 && z_i == 1)) {
              lights_q_nnn.push_back(nTiled::world::PointLight(name,
                                                               glm::vec4(x_q_nnn.at(x_i),
                                                                         y_q_nnn.at(y_i),
                                                                         z_q_nnn.at(z_i),
                                                                         1.0f),
                                                               intensity,
                                                               radius,
                                                               true,
                                                               empty_map));

            }
          }
        }
      }

      // ----------------------------------------------------------------------
      std::vector<float> x_q_pnn = { 0.0, 2.0, 4.0 };
      std::vector<float> y_q_pnn = { -2.0, 0.0, 2.0 };
      std::vector<float> z_q_pnn = { -2.0, 0.0, 2.0 };

      std::vector<nTiled::world::PointLight> lights_q_pnn = {};
      for (unsigned int x_i = 0; x_i < 2; x_i++) {
        for (unsigned int y_i = 0; y_i < 2; y_i++) {
          for (unsigned int z_i = 0; z_i < 2; z_i++) {
            if (!(x_i == 1 && y_i == 1 && z_i == 1)) {
              lights_q_pnn.push_back(nTiled::world::PointLight(name,
                                                               glm::vec4(x_q_pnn.at(x_i),
                                                                         y_q_pnn.at(y_i),
                                                                         z_q_pnn.at(z_i),
                                                                         1.0f),
                                                               intensity,
                                                               radius,
                                                               true,
                                                               empty_map));

            }
          }
        }
      }

      // ----------------------------------------------------------------------
      std::vector<float> x_q_npn = { -2.0, 0.0, 2.0 };
      std::vector<float> y_q_npn = { 0.0, 2.0, 4.0 };
      std::vector<float> z_q_npn = { -2.0, 0.0, 2.0 };

      std::vector<nTiled::world::PointLight> lights_q_npn = {};
      for (unsigned int x_i = 0; x_i < 2; x_i++) {
        for (unsigned int y_i = 0; y_i < 2; y_i++) {
          for (unsigned int z_i = 0; z_i < 2; z_i++) {
            if (!(x_i == 1 && y_i == 1 && z_i == 1)) {
              lights_q_npn.push_back(nTiled::world::PointLight(name,
                                                               glm::vec4(x_q_npn.at(x_i),
                                                                         y_q_npn.at(y_i),
                                                                         z_q_npn.at(z_i),
                                                                         1.0f),
                                                               intensity,
                                                               radius,
                                                               true,
                                                               empty_map));

            }
          }
        }
      }

      // ----------------------------------------------------------------------
      std::vector<float> x_q_ppn = { 0.0, 2.0, 4.0 };
      std::vector<float> y_q_ppn = { 0.0, 2.0, 4.0 };
      std::vector<float> z_q_ppn = { -2.0, 0.0, 2.0 };

      std::vector<nTiled::world::PointLight> lights_q_ppn = {};
      for (unsigned int x_i = 0; x_i < 2; x_i++) {
        for (unsigned int y_i = 0; y_i < 2; y_i++) {
          for (unsigned int z_i = 0; z_i < 2; z_i++) {
            if (!(x_i == 1 && y_i == 1 && z_i == 1)) {
              lights_q_ppn.push_back(nTiled::world::PointLight(name,
                                                               glm::vec4(x_q_ppn.at(x_i),
                                                                         y_q_ppn.at(y_i),
                                                                         z_q_ppn.at(z_i),
                                                                         1.0f),
                                                               intensity,
                                                               radius,
                                                               true,
                                                               empty_map));

            }
          }
        }
      }

      // ----------------------------------------------------------------------
      std::vector<float> x_q_nnp = { -2.0, 0.0, 2.0 };
      std::vector<float> y_q_nnp = { -2.0, 0.0, 2.0 };
      std::vector<float> z_q_nnp = { 0.0, 2.0, 4.0 };

      std::vector<nTiled::world::PointLight> lights_q_nnp = {};
      for (unsigned int x_i = 0; x_i < 2; x_i++) {
        for (unsigned int y_i = 0; y_i < 2; y_i++) {
          for (unsigned int z_i = 0; z_i < 2; z_i++) {
            if (!(x_i == 1 && y_i == 1 && z_i == 1)) {
              lights_q_nnp.push_back(nTiled::world::PointLight(name,
                                                               glm::vec4(x_q_nnp.at(x_i),
                                                                         y_q_nnp.at(y_i),
                                                                         z_q_nnp.at(z_i),
                                                                         1.0f),
                                                               intensity,
                                                               radius,
                                                               true,
                                                               empty_map));

            }
          }
        }
      }

      // ----------------------------------------------------------------------
      std::vector<float> x_q_pnp = { 0.0, 2.0, 4.0 };
      std::vector<float> y_q_pnp = { -2.0, 0.0, 2.0 };
      std::vector<float> z_q_pnp = { 0.0, 2.0, 4.0 };

      std::vector<nTiled::world::PointLight> lights_q_pnp = {};
      for (unsigned int x_i = 0; x_i < 2; x_i++) {
        for (unsigned int y_i = 0; y_i < 2; y_i++) {
          for (unsigned int z_i = 0; z_i < 2; z_i++) {
            if (!(x_i == 1 && y_i == 1 && z_i == 1)) {
              lights_q_pnp.push_back(nTiled::world::PointLight(name,
                                                               glm::vec4(x_q_pnp.at(x_i),
                                                                         y_q_pnp.at(y_i),
                                                                         z_q_pnp.at(z_i),
                                                                         1.0f),
                                                               intensity,
                                                               radius,
                                                               true,
                                                               empty_map));

            }
          }
        }
      }

      // ----------------------------------------------------------------------
      std::vector<float> x_q_npp = { -2.0, 0.0, 2.0 };
      std::vector<float> y_q_npp = { 0.0, 2.0, 4.0 };
      std::vector<float> z_q_npp = { 0.0, 2.0, 4.0 };

      std::vector<nTiled::world::PointLight> lights_q_npp = {};
      for (unsigned int x_i = 0; x_i < 2; x_i++) {
        for (unsigned int y_i = 0; y_i < 2; y_i++) {
          for (unsigned int z_i = 0; z_i < 2; z_i++) {
            if (!(x_i == 1 && y_i == 1 && z_i == 1)) {
              lights_q_npp.push_back(nTiled::world::PointLight(name,
                                                               glm::vec4(x_q_npp.at(x_i),
                                                                         y_q_npp.at(y_i),
                                                                         z_q_npp.at(z_i),
                                                                         1.0f),
                                                               intensity,
                                                               radius,
                                                               true,
                                                               empty_map));

            }
          }
        }
      }

      // ----------------------------------------------------------------------
      std::vector<float> x_q_ppp = { 0.0, 2.0, 4.0 };
      std::vector<float> y_q_ppp = { 0.0, 2.0, 4.0 };
      std::vector<float> z_q_ppp = { 0.0, 2.0, 4.0 };

      std::vector<nTiled::world::PointLight> lights_q_ppp = {};
      for (unsigned int x_i = 0; x_i < 2; x_i++) {
        for (unsigned int y_i = 0; y_i < 2; y_i++) {
          for (unsigned int z_i = 0; z_i < 2; z_i++) {
            if (!(x_i == 1 && y_i == 1 && z_i == 1)) {
              lights_q_ppp.push_back(nTiled::world::PointLight(name,
                                                               glm::vec4(x_q_ppp.at(x_i),
                                                                         y_q_ppp.at(y_i),
                                                                         z_q_ppp.at(z_i),
                                                                         1.0f),
                                                               intensity,
                                                               radius,
                                                               true,
                                                               empty_map));

            }
          }
        }
      }


      THEN("nodeWithinLight should return false.") {
        for (const nTiled::world::PointLight& l : lights_q_nnn) {
          REQUIRE_FALSE(builder.nodeWithinLight(l, q_nnn, slt_size));
        }

        for (const nTiled::world::PointLight& l : lights_q_pnn) {
          REQUIRE_FALSE(builder.nodeWithinLight(l, q_pnn, slt_size));
        }

        for (const nTiled::world::PointLight& l : lights_q_npn) {
          REQUIRE_FALSE(builder.nodeWithinLight(l, q_npn, slt_size));
        }

        for (const nTiled::world::PointLight& l : lights_q_ppn) {
          REQUIRE_FALSE(builder.nodeWithinLight(l, q_ppn, slt_size));
        }

        for (const nTiled::world::PointLight& l : lights_q_nnp) {
          REQUIRE_FALSE(builder.nodeWithinLight(l, q_nnp, slt_size));
        }

        for (const nTiled::world::PointLight& l : lights_q_pnp) {
          REQUIRE_FALSE(builder.nodeWithinLight(l, q_pnp, slt_size));
        }

        for (const nTiled::world::PointLight& l : lights_q_npp) {
          REQUIRE_FALSE(builder.nodeWithinLight(l, q_npp, slt_size));
        }

        for (const nTiled::world::PointLight& l : lights_q_ppp) {
          REQUIRE_FALSE(builder.nodeWithinLight(l, q_ppp, slt_size));
        }
      }
    }

    WHEN("A light is provided that does covers a node partly") {
      float slt_size = 2.0f;

      float radius = 3.0;
      float radius_2 = 1.0;
      std::string name = "just_testing_things";
      glm::vec3 intensity = glm::vec3(1.0f);
      std::map<std::string, nTiled::world::Object*> empty_map =
        std::map<std::string, nTiled::world::Object*>();

      // ----------------------------------------------------------------------
      std::vector<float> x_q_nnn = { -2.0, 0.0, 2.0 };
      std::vector<float> y_q_nnn = { -2.0, 0.0, 2.0 };
      std::vector<float> z_q_nnn = { -2.0, 0.0, 2.0 };

      std::vector<nTiled::world::PointLight> lights_q_nnn = {};
      for (unsigned int x_i = 0; x_i < 2; x_i++) {
        for (unsigned int y_i = 0; y_i < 2; y_i++) {
          for (unsigned int z_i = 0; z_i < 2; z_i++) {
            if (x_i == 1 && y_i == 1 && z_i == 1) {
              lights_q_nnn.push_back(nTiled::world::PointLight(name,
                                                               glm::vec4(x_q_nnn.at(x_i),
                                                                         y_q_nnn.at(y_i),
                                                                         z_q_nnn.at(z_i),
                                                                         1.0f),
                                                               intensity,
                                                               radius_2,
                                                               true,
                                                               empty_map));
            } else {
              lights_q_nnn.push_back(nTiled::world::PointLight(name,
                                                               glm::vec4(x_q_nnn.at(x_i),
                                                                         y_q_nnn.at(y_i),
                                                                         z_q_nnn.at(z_i),
                                                                         1.0f),
                                                               intensity,
                                                               radius,
                                                               true,
                                                               empty_map));
            }
          }
        }
      }

      // ----------------------------------------------------------------------
      std::vector<float> x_q_pnn = { 0.0, 2.0, 4.0 };
      std::vector<float> y_q_pnn = { -2.0, 0.0, 2.0 };
      std::vector<float> z_q_pnn = { -2.0, 0.0, 2.0 };

      std::vector<nTiled::world::PointLight> lights_q_pnn = {};
      for (unsigned int x_i = 0; x_i < 2; x_i++) {
        for (unsigned int y_i = 0; y_i < 2; y_i++) {
          for (unsigned int z_i = 0; z_i < 2; z_i++) {
            if (x_i == 1 && y_i == 1 && z_i == 1) {
              lights_q_pnn.push_back(nTiled::world::PointLight(name,
                                                               glm::vec4(x_q_pnn.at(x_i),
                                                                         y_q_pnn.at(y_i),
                                                                         z_q_pnn.at(z_i),
                                                                         1.0f),
                                                               intensity,
                                                               radius_2,
                                                               true,
                                                               empty_map));

            } else {
              lights_q_nnn.push_back(nTiled::world::PointLight(name,
                                                               glm::vec4(x_q_nnn.at(x_i),
                                                                         y_q_nnn.at(y_i),
                                                                         z_q_nnn.at(z_i),
                                                                         1.0f),
                                                               intensity,
                                                               radius,
                                                               true,
                                                               empty_map));
            }
          }
        }
      }

      // ----------------------------------------------------------------------
      std::vector<float> x_q_npn = { -2.0, 0.0, 2.0 };
      std::vector<float> y_q_npn = { 0.0, 2.0, 4.0 };
      std::vector<float> z_q_npn = { -2.0, 0.0, 2.0 };

      std::vector<nTiled::world::PointLight> lights_q_npn = {};
      for (unsigned int x_i = 0; x_i < 2; x_i++) {
        for (unsigned int y_i = 0; y_i < 2; y_i++) {
          for (unsigned int z_i = 0; z_i < 2; z_i++) {
            if (x_i == 1 && y_i == 1 && z_i == 1) {
              lights_q_npn.push_back(nTiled::world::PointLight(name,
                                                               glm::vec4(x_q_npn.at(x_i),
                                                                         y_q_npn.at(y_i),
                                                                         z_q_npn.at(z_i),
                                                                         1.0f),
                                                               intensity,
                                                               radius_2,
                                                               true,
                                                               empty_map));

            } else {
              lights_q_nnn.push_back(nTiled::world::PointLight(name,
                                                               glm::vec4(x_q_nnn.at(x_i),
                                                                         y_q_nnn.at(y_i),
                                                                         z_q_nnn.at(z_i),
                                                                         1.0f),
                                                               intensity,
                                                               radius,
                                                               true,
                                                               empty_map));
            }
          }
        }
      }

      // ----------------------------------------------------------------------
      std::vector<float> x_q_ppn = { 0.0, 2.0, 4.0 };
      std::vector<float> y_q_ppn = { 0.0, 2.0, 4.0 };
      std::vector<float> z_q_ppn = { -2.0, 0.0, 2.0 };

      std::vector<nTiled::world::PointLight> lights_q_ppn = {};
      for (unsigned int x_i = 0; x_i < 2; x_i++) {
        for (unsigned int y_i = 0; y_i < 2; y_i++) {
          for (unsigned int z_i = 0; z_i < 2; z_i++) {
            if (x_i == 1 && y_i == 1 && z_i == 1) {
              lights_q_ppn.push_back(nTiled::world::PointLight(name,
                                                               glm::vec4(x_q_ppn.at(x_i),
                                                                         y_q_ppn.at(y_i),
                                                                         z_q_ppn.at(z_i),
                                                                         1.0f),
                                                               intensity,
                                                               radius_2,
                                                               true,
                                                               empty_map));

            } else {
              lights_q_nnn.push_back(nTiled::world::PointLight(name,
                                                               glm::vec4(x_q_nnn.at(x_i),
                                                                         y_q_nnn.at(y_i),
                                                                         z_q_nnn.at(z_i),
                                                                         1.0f),
                                                               intensity,
                                                               radius,
                                                               true,
                                                               empty_map));
            }
          }
        }
      }

      // ----------------------------------------------------------------------
      std::vector<float> x_q_nnp = { -2.0, 0.0, 2.0 };
      std::vector<float> y_q_nnp = { -2.0, 0.0, 2.0 };
      std::vector<float> z_q_nnp = { 0.0, 2.0, 4.0 };

      std::vector<nTiled::world::PointLight> lights_q_nnp = {};
      for (unsigned int x_i = 0; x_i < 2; x_i++) {
        for (unsigned int y_i = 0; y_i < 2; y_i++) {
          for (unsigned int z_i = 0; z_i < 2; z_i++) {
            if (x_i == 1 && y_i == 1 && z_i == 1) {
              lights_q_nnp.push_back(nTiled::world::PointLight(name,
                                                               glm::vec4(x_q_nnp.at(x_i),
                                                                         y_q_nnp.at(y_i),
                                                                         z_q_nnp.at(z_i),
                                                                         1.0f),
                                                               intensity,
                                                               radius_2,
                                                               true,
                                                               empty_map));

            } else {
              lights_q_nnn.push_back(nTiled::world::PointLight(name,
                                                               glm::vec4(x_q_nnn.at(x_i),
                                                                         y_q_nnn.at(y_i),
                                                                         z_q_nnn.at(z_i),
                                                                         1.0f),
                                                               intensity,
                                                               radius,
                                                               true,
                                                               empty_map));
            }
          }
        }
      }

      // ----------------------------------------------------------------------
      std::vector<float> x_q_pnp = { 0.0, 2.0, 4.0 };
      std::vector<float> y_q_pnp = { -2.0, 0.0, 2.0 };
      std::vector<float> z_q_pnp = { 0.0, 2.0, 4.0 };

      std::vector<nTiled::world::PointLight> lights_q_pnp = {};
      for (unsigned int x_i = 0; x_i < 2; x_i++) {
        for (unsigned int y_i = 0; y_i < 2; y_i++) {
          for (unsigned int z_i = 0; z_i < 2; z_i++) {
            if (x_i == 1 && y_i == 1 && z_i == 1) {
              lights_q_pnp.push_back(nTiled::world::PointLight(name,
                                                               glm::vec4(x_q_pnp.at(x_i),
                                                                         y_q_pnp.at(y_i),
                                                                         z_q_pnp.at(z_i),
                                                                         1.0f),
                                                               intensity,
                                                               radius_2,
                                                               true,
                                                               empty_map));

            } else {
              lights_q_nnn.push_back(nTiled::world::PointLight(name,
                                                               glm::vec4(x_q_nnn.at(x_i),
                                                                         y_q_nnn.at(y_i),
                                                                         z_q_nnn.at(z_i),
                                                                         1.0f),
                                                               intensity,
                                                               radius,
                                                               true,
                                                               empty_map));
            }
          }
        }
      }

      // ----------------------------------------------------------------------
      std::vector<float> x_q_npp = { -2.0, 0.0, 2.0 };
      std::vector<float> y_q_npp = { 0.0, 2.0, 4.0 };
      std::vector<float> z_q_npp = { 0.0, 2.0, 4.0 };

      std::vector<nTiled::world::PointLight> lights_q_npp = {};
      for (unsigned int x_i = 0; x_i < 2; x_i++) {
        for (unsigned int y_i = 0; y_i < 2; y_i++) {
          for (unsigned int z_i = 0; z_i < 2; z_i++) {
            if (x_i == 1 && y_i == 1 && z_i == 1) {
              lights_q_npp.push_back(nTiled::world::PointLight(name,
                                                               glm::vec4(x_q_npp.at(x_i),
                                                                         y_q_npp.at(y_i),
                                                                         z_q_npp.at(z_i),
                                                                         1.0f),
                                                               intensity,
                                                               radius_2,
                                                               true,
                                                               empty_map));

            } else {
              lights_q_nnn.push_back(nTiled::world::PointLight(name,
                                                               glm::vec4(x_q_nnn.at(x_i),
                                                                         y_q_nnn.at(y_i),
                                                                         z_q_nnn.at(z_i),
                                                                         1.0f),
                                                               intensity,
                                                               radius,
                                                               true,
                                                               empty_map));
            }
          }
        }
      }

      // ----------------------------------------------------------------------
      std::vector<float> x_q_ppp = { 0.0, 2.0, 4.0 };
      std::vector<float> y_q_ppp = { 0.0, 2.0, 4.0 };
      std::vector<float> z_q_ppp = { 0.0, 2.0, 4.0 };

      std::vector<nTiled::world::PointLight> lights_q_ppp = {};
      for (unsigned int x_i = 0; x_i < 2; x_i++) {
        for (unsigned int y_i = 0; y_i < 2; y_i++) {
          for (unsigned int z_i = 0; z_i < 2; z_i++) {
            if (x_i == 1 && y_i == 1 && z_i == 1) {
              lights_q_ppp.push_back(nTiled::world::PointLight(name,
                                                               glm::vec4(x_q_ppp.at(x_i),
                                                                         y_q_ppp.at(y_i),
                                                                         z_q_ppp.at(z_i),
                                                                         1.0f),
                                                               intensity,
                                                               radius_2,
                                                               true,
                                                               empty_map));

            } else {
              lights_q_nnn.push_back(nTiled::world::PointLight(name,
                                                               glm::vec4(x_q_nnn.at(x_i),
                                                                         y_q_nnn.at(y_i),
                                                                         z_q_nnn.at(z_i),
                                                                         1.0f),
                                                               intensity,
                                                               radius,
                                                               true,
                                                               empty_map));
            }
          }
        }
      }


      THEN("nodeWithinLight should return true.") {
        for (const nTiled::world::PointLight& l : lights_q_nnn) {
          REQUIRE(builder.nodeWithinLight(l, q_nnn, slt_size));
        }

        for (const nTiled::world::PointLight& l : lights_q_pnn) {
          REQUIRE(builder.nodeWithinLight(l, q_pnn, slt_size));
        }

        for (const nTiled::world::PointLight& l : lights_q_npn) {
          REQUIRE(builder.nodeWithinLight(l, q_npn, slt_size));
        }

        for (const nTiled::world::PointLight& l : lights_q_ppn) {
          REQUIRE(builder.nodeWithinLight(l, q_ppn, slt_size));
        }

        for (const nTiled::world::PointLight& l : lights_q_nnp) {
          REQUIRE(builder.nodeWithinLight(l, q_nnp, slt_size));
        }

        for (const nTiled::world::PointLight& l : lights_q_pnp) {
          REQUIRE(builder.nodeWithinLight(l, q_pnp, slt_size));
        }

        for (const nTiled::world::PointLight& l : lights_q_npp) {
          REQUIRE(builder.nodeWithinLight(l, q_npp, slt_size));
        }

        for (const nTiled::world::PointLight& l : lights_q_ppp) {
          REQUIRE(builder.nodeWithinLight(l, q_ppp, slt_size));
        }
      }
    }


    WHEN("A light is provided that completely covers a node") {
      float slt_size = 2.0f;

      double radius = 10.85;
      std::string name = "just_testing_things";
      glm::vec3 intensity = glm::vec3(1.0f);
      std::map<std::string, nTiled::world::Object*> empty_map =
        std::map<std::string, nTiled::world::Object*>();

      // ----------------------------------------------------------------------
      std::vector<float> x_q_nnn = { -2.0, 0.0, 2.0 };
      std::vector<float> y_q_nnn = { -2.0, 0.0, 2.0 };
      std::vector<float> z_q_nnn = { -2.0, 0.0, 2.0 };

      std::vector<nTiled::world::PointLight> lights_q_nnn = {};
      for (unsigned int x_i = 0; x_i < 2; x_i++) {
        for (unsigned int y_i = 0; y_i < 2; y_i++) {
          for (unsigned int z_i = 0; z_i < 2; z_i++) {
            lights_q_nnn.push_back(nTiled::world::PointLight(name,
                                                             glm::vec4(x_q_nnn.at(x_i),
                                                                       y_q_nnn.at(y_i),
                                                                       z_q_nnn.at(z_i),
                                                                       1.0f),
                                                             intensity,
                                                             radius,
                                                             true,
                                                             empty_map));

          }
        }
      }

      // ----------------------------------------------------------------------
      std::vector<float> x_q_pnn = { 0.0, 2.0, 4.0 };
      std::vector<float> y_q_pnn = { -2.0, 0.0, 2.0 };
      std::vector<float> z_q_pnn = { -2.0, 0.0, 2.0 };

      std::vector<nTiled::world::PointLight> lights_q_pnn = {};
      for (unsigned int x_i = 0; x_i < 2; x_i++) {
        for (unsigned int y_i = 0; y_i < 2; y_i++) {
          for (unsigned int z_i = 0; z_i < 2; z_i++) {
            lights_q_pnn.push_back(nTiled::world::PointLight(name,
                                                             glm::vec4(x_q_pnn.at(x_i),
                                                                       y_q_pnn.at(y_i),
                                                                       z_q_pnn.at(z_i),
                                                                       1.0f),
                                                             intensity,
                                                             radius,
                                                             true,
                                                             empty_map));

          }
        }
      }

      // ----------------------------------------------------------------------
      std::vector<float> x_q_npn = { -2.0, 0.0, 2.0 };
      std::vector<float> y_q_npn = { 0.0, 2.0, 4.0 };
      std::vector<float> z_q_npn = { -2.0, 0.0, 2.0 };

      std::vector<nTiled::world::PointLight> lights_q_npn = {};
      for (unsigned int x_i = 0; x_i < 2; x_i++) {
        for (unsigned int y_i = 0; y_i < 2; y_i++) {
          for (unsigned int z_i = 0; z_i < 2; z_i++) {
            lights_q_npn.push_back(nTiled::world::PointLight(name,
                                                             glm::vec4(x_q_npn.at(x_i),
                                                                       y_q_npn.at(y_i),
                                                                       z_q_npn.at(z_i),
                                                                       1.0f),
                                                             intensity,
                                                             radius,
                                                             true,
                                                             empty_map));

          }
        }
      }

      // ----------------------------------------------------------------------
      std::vector<float> x_q_ppn = { 0.0, 2.0, 4.0 };
      std::vector<float> y_q_ppn = { 0.0, 2.0, 4.0 };
      std::vector<float> z_q_ppn = { -2.0, 0.0, 2.0 };

      std::vector<nTiled::world::PointLight> lights_q_ppn = {};
      for (unsigned int x_i = 0; x_i < 2; x_i++) {
        for (unsigned int y_i = 0; y_i < 2; y_i++) {
          for (unsigned int z_i = 0; z_i < 2; z_i++) {
            lights_q_ppn.push_back(nTiled::world::PointLight(name,
                                                             glm::vec4(x_q_ppn.at(x_i),
                                                                       y_q_ppn.at(y_i),
                                                                       z_q_ppn.at(z_i),
                                                                       1.0f),
                                                             intensity,
                                                             radius,
                                                             true,
                                                             empty_map));

          }
        }
      }

      // ----------------------------------------------------------------------
      std::vector<float> x_q_nnp = { -2.0, 0.0, 2.0 };
      std::vector<float> y_q_nnp = { -2.0, 0.0, 2.0 };
      std::vector<float> z_q_nnp = { 0.0, 2.0, 4.0 };

      std::vector<nTiled::world::PointLight> lights_q_nnp = {};
      for (unsigned int x_i = 0; x_i < 2; x_i++) {
        for (unsigned int y_i = 0; y_i < 2; y_i++) {
          for (unsigned int z_i = 0; z_i < 2; z_i++) {
            lights_q_nnp.push_back(nTiled::world::PointLight(name,
                                                             glm::vec4(x_q_nnp.at(x_i),
                                                                       y_q_nnp.at(y_i),
                                                                       z_q_nnp.at(z_i),
                                                                       1.0f),
                                                             intensity,
                                                             radius,
                                                             true,
                                                             empty_map));

          }
        }
      }

      // ----------------------------------------------------------------------
      std::vector<float> x_q_pnp = { 0.0, 2.0, 4.0 };
      std::vector<float> y_q_pnp = { -2.0, 0.0, 2.0 };
      std::vector<float> z_q_pnp = { 0.0, 2.0, 4.0 };

      std::vector<nTiled::world::PointLight> lights_q_pnp = {};
      for (unsigned int x_i = 0; x_i < 2; x_i++) {
        for (unsigned int y_i = 0; y_i < 2; y_i++) {
          for (unsigned int z_i = 0; z_i < 2; z_i++) {
            lights_q_pnp.push_back(nTiled::world::PointLight(name,
                                                             glm::vec4(x_q_pnp.at(x_i),
                                                                       y_q_pnp.at(y_i),
                                                                       z_q_pnp.at(z_i),
                                                                       1.0f),
                                                             intensity,
                                                             radius,
                                                             true,
                                                             empty_map));

          }
        }
      }

      // ----------------------------------------------------------------------
      std::vector<float> x_q_npp = { -2.0, 0.0, 2.0 };
      std::vector<float> y_q_npp = { 0.0, 2.0, 4.0 };
      std::vector<float> z_q_npp = { 0.0, 2.0, 4.0 };

      std::vector<nTiled::world::PointLight> lights_q_npp = {};
      for (unsigned int x_i = 0; x_i < 2; x_i++) {
        for (unsigned int y_i = 0; y_i < 2; y_i++) {
          for (unsigned int z_i = 0; z_i < 2; z_i++) {
            lights_q_npp.push_back(nTiled::world::PointLight(name,
                                                             glm::vec4(x_q_npp.at(x_i),
                                                                       y_q_npp.at(y_i),
                                                                       z_q_npp.at(z_i),
                                                                       1.0f),
                                                             intensity,
                                                             radius,
                                                             true,
                                                             empty_map));

          }
        }
      }

      // ----------------------------------------------------------------------
      std::vector<float> x_q_ppp = { 0.0, 2.0, 4.0 };
      std::vector<float> y_q_ppp = { 0.0, 2.0, 4.0 };
      std::vector<float> z_q_ppp = { 0.0, 2.0, 4.0 };

      std::vector<nTiled::world::PointLight> lights_q_ppp = {};
      for (unsigned int x_i = 0; x_i < 2; x_i++) {
        for (unsigned int y_i = 0; y_i < 2; y_i++) {
          for (unsigned int z_i = 0; z_i < 2; z_i++) {
            lights_q_ppp.push_back(nTiled::world::PointLight(name,
                                                             glm::vec4(x_q_ppp.at(x_i),
                                                                       y_q_ppp.at(y_i),
                                                                       z_q_ppp.at(z_i),
                                                                       1.0f),
                                                             intensity,
                                                             radius,
                                                             true,
                                                             empty_map));

          }
        }
      }


      THEN("nodeWithinLight should return true.") {
        for (const nTiled::world::PointLight& l : lights_q_nnn) {
          REQUIRE(builder.nodeWithinLight(l, q_nnn, slt_size));
        }

        for (const nTiled::world::PointLight& l : lights_q_pnn) {
          REQUIRE(builder.nodeWithinLight(l, q_pnn, slt_size));
        }

        for (const nTiled::world::PointLight& l : lights_q_npn) {
          REQUIRE(builder.nodeWithinLight(l, q_npn, slt_size));
        }

        for (const nTiled::world::PointLight& l : lights_q_ppn) {
          REQUIRE(builder.nodeWithinLight(l, q_ppn, slt_size));
        }

        for (const nTiled::world::PointLight& l : lights_q_nnp) {
          REQUIRE(builder.nodeWithinLight(l, q_nnp, slt_size));
        }

        for (const nTiled::world::PointLight& l : lights_q_pnp) {
          REQUIRE(builder.nodeWithinLight(l, q_pnp, slt_size));
        }

        for (const nTiled::world::PointLight& l : lights_q_npp) {
          REQUIRE(builder.nodeWithinLight(l, q_npp, slt_size));
        }

        for (const nTiled::world::PointLight& l : lights_q_ppp) {
          REQUIRE(builder.nodeWithinLight(l, q_ppp, slt_size));
        }
      }
    }
  }
}
