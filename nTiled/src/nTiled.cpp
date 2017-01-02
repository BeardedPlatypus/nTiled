// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include "main\Controller.h"
#include <iostream>

#include "pipeline\light-management\hierarchical\single-light-tree\SLTBuilder.h"
#include "pipeline\light-management\hierarchical\LightOctreeBuilder.h"

#include "pipeline\light-management\hierarchical\LightOctreeExporter.h"

#include "pipeline\light-management\hierarchical\HashedLightManager.h"

// ----------------------------------------------------------------------------
//  Defines
// ----------------------------------------------------------------------------
//#define SCENE_PATH std::string("../example/example_scene.json")
//#define SCENE_PATH std::string("C:/Users/Monthy/Documents/projects/thesis/thesis-data-suite/scenes/pipers-alley/validation-scenes/arch/scene.json")
#define SCENE_PATH std::string("C:/Users/Monthy/Documents/projects/thesis/thesis-data-suite/scenes/ziggurat-city/scene-json/scene.json")

#define OUTPUT_PATH_BASE = std::string("C:/Users/Monthy/Documents/projects/thesis/hierarchical-shading/test/")

// ----------------------------------------------------------------------------
//  Main
// ----------------------------------------------------------------------------
int main(int argc, char** argv) {
  /*
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
  */

  //nTiled::pipeline::hierarchical::SLTBuilder builder =  nTiled::pipeline::hierarchical::SLTBuilder(0.4,
  //                                                                                               glm::vec4(-1.8, -1.8, -1.8, 1.0));
  //nTiled::pipeline::hierarchical::SingleLightTree* slt = builder.buildSingleLightTree(light, 0);

  nTiled::world::World world = nTiled::world::World();
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

  nTiled::pipeline::hierarchical::HashedLightManager light_manager =
    nTiled::pipeline::hierarchical::HashedLightManager(world,
                                                       0.25,
                                                       2,
                                                       1.5,
                                                       5);
  light_manager.constructLightOctree();

 
  std::string path_octree = "C:/Users/Monthy/Documents/projects/thesis/hierarchical-shading/test/octree_test.json";
  std::string path_lights = "C:/Users/Monthy/Documents/projects/thesis/hierarchical-shading/test/lights_test.json";
  std::string path_linkless = "C:/Users/Monthy/Documents/projects/thesis/hierarchical-shading/test/linkless_test.json";
  std::string path_indices = "C:/Users/Monthy/Documents/projects/thesis/hierarchical-shading/test/indices_test.json";

  light_manager.exportToJson(path_lights,
                             path_octree,
                             path_linkless,
                             path_indices);
  return 0;
}