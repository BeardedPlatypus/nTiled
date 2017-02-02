// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include "main\Controller.h"
#include <iostream>

#include "pipeline\light-management\hashed\HashedLightManager.h"
#include "pipeline\light-management\hashed\light-octree\LightOctreeBuilder.h"
#include "pipeline\light-management\hashed\light-octree\LightOctreeExporter.h"
#include "pipeline\light-management\hashed\light-octree\single-light-tree\SingleLightTreeBuilder.h"

#include "math\util.h"
// ----------------------------------------------------------------------------
//  Defines
// ----------------------------------------------------------------------------
#define SCENE_PATH std::string("../example/example_scene_hashed2.json")
//#define SCENE_PATH std::string("C:/Users/Monthy/Documents/projects/thesis/thesis-data-suite/scenes/pipers-alley/validation-scenes/arch/scene.json")
//#define SCENE_PATH std::string("C:/Users/Monthy/Documents/projects/thesis/thesis-data-suite/scenes/ziggurat-city/scene-json/scene.json")

//#define OUTPUT_PATH_BASE = std::string("C:/Users/Monthy/Documents/projects/thesis/hierarchical-shading/test/")

// ----------------------------------------------------------------------------
//  Main
// ----------------------------------------------------------------------------
int main(int argc, char** argv) {
  if (argc > 2) {
    std::cerr << "Usage: " << argv[0] << "<path_to_scene_def.json" << std::endl;
    return -1;
  }

  std::string scene_def_path;
  if (argc > 1) {
    scene_def_path = std::string(argv[1]);
    std::cout << "Using scene: " << scene_def_path << std::endl;
  } else {
    scene_def_path = SCENE_PATH;
  }

  nTiled::Controller controller = nTiled::Controller(scene_def_path);
  controller.initialise();
  controller.render();

  /*
  float size = 2.0 + 0.02 ;

  unsigned int k = nTiled::math::getNextPow2(int(ceil(size / 0.1)));
  unsigned int octree_depth = (unsigned int)(log2(k)) + 1;
  nTiled::pipeline::hashed::SingleLightTreeBuilder builder = nTiled::pipeline::hashed::SingleLightTreeBuilder(0.25,
                                                                                                              glm::vec4(0.0, 0.0, 0.0, 1.0),
                                                                                                              octree_depth);

  nTiled::world::World world = nTiled::world::World();
  world.constructPointLight("light.0",
                            glm::vec4(1.05, 1.05, 1.05, 1.0),
                            glm::vec3(1.0, 1.0, 1.0),
                            1.0,
                            true,
                            std::map<std::string, nTiled::world::Object*>());

  nTiled::pipeline::hashed::SingleLightTree* tree = builder.buildSingleLightTree(*world.p_lights.at(0), 0);

  std::string path = "C:/Users/Monthy/Documents/projects/thesis/hierarchical-shading/test/single_light_tree_display.json";
  tree->exportToJson(path,
                    octree_depth,
                    glm::vec3(0.0, 0.0, 0.0),
                    0.1);
                    */

   
  /*
  nTiled::pipeline::hashed::SingleLightTreeBuilder builder = nTiled::pipeline::hashed::SingleLightTreeBuilder(1.0,
                                                                                                              glm::vec4(-1.9, -0.8, -1.1, 1.0),
                                                                                                              3); 
  //                                                                                               glm::vec4(-1.8, -1.8, -1.8, 1.0)); */
  //nTiled::pipeline::hierarchical::SingleLightTree* slt = builder.buildSingleLightTree(light, 0);
  
  //nTiled::world::World world = nTiled::world::World();

  /*
  world.constructPointLight("light.0",
                            glm::vec4(2.05, 2.05, 2.05, 1.0),
                            glm::vec3(1.0, 1.0, 1.0),
                            2.0,
                            true,
                            std::map<std::string, nTiled::world::Object*>());
              */
  /*
  world.constructPointLight("light.0",
                            glm::vec4(-0.8, 0.3, 0.0, 1.0),
                            glm::vec3(1.0, 1.0, 1.0),
                            1.0,
                            true,
                            std::map<std::string, nTiled::world::Object*>());

  world.constructPointLight("light.1",
                            glm::vec4(0.4, 0.3, 0.69282, 1.0),
                            glm::vec3(1.0, 1.0, 1.0),
                            1.0,
                            true,
                            std::map<std::string, nTiled::world::Object*>());

  world.constructPointLight("light.2",
                            glm::vec4(0.4, 0.3, -0.69282, 1.0),
                            glm::vec3(1.0, 1.0, 1.0),
                            1.0,
                            true,
                            std::map<std::string, nTiled::world::Object*>()); 

  /*
  nTiled::pipeline::hashed::HashedLightManager light_manager =
    nTiled::pipeline::hashed::HashedLightManager(world,
                                                 0.25,
                                                 3,
                                                 2,
                                                 10);
  light_manager.constructLightOctree();
  */

  /* 
  std::string path_octree = "C:/Users/Monthy/Documents/projects/thesis/hierarchical-shading/test/octree_display_3_lights.json";
  std::string path_lights = "C:/Users/Monthy/Documents/projects/thesis/hierarchical-shading/test/lights_display_3_lights.json";
  //std::string path_slt = "C:/Users/Monthy/Documents/projects/thesis/hierarchical-shading/test/slt_test.json";

  /*
  slt->exportToJson(path_slt,
                    3,
                    glm::vec3(-1.9, -0.8, -1.1),
                    2.0); */
  /*
  nTiled::pipeline::hashed::LightOctreeBuilder builder = nTiled::pipeline::hashed::LightOctreeBuilder();
  nTiled::pipeline::hashed::LightOctree* octree = builder.buildLightOctree(world.p_lights, 0.25);

  nTiled::pipeline::hashed::exportLightOctree(path_octree, *octree);
  nTiled::pipeline::hashed::exportLights(path_lights, world.p_lights);
  /*
  std::string path_linkless = "C:/Users/Monthy/Documents/projects/thesis/hierarchical-shading/test/linkless_test.json";
  std::string path_indices = "C:/Users/Monthy/Documents/projects/thesis/hierarchical-shading/test/indices_test.json";

  light_manager.exportToJson(path_lights,
                             path_octree,
                             path_linkless,
                             path_indices);
                             */
  return 0;
}