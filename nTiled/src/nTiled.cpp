// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include "main\Controller.h"
#include <iostream>

#include "pipeline\light-management\hierarchical\single-light-tree\SLTBuilder.h"
#include "pipeline\light-management\hierarchical\LightOctreeBuilder.h"

#include "pipeline\light-management\hierarchical\LightOctreeExporter.h"

// ----------------------------------------------------------------------------
//  Defines
// ----------------------------------------------------------------------------
//#define SCENE_PATH std::string("../example/example_scene.json")
//#define SCENE_PATH std::string("C:/Users/Monthy/Documents/projects/thesis/thesis-data-suite/scenes/pipers-alley/validation-scenes/arch/scene.json")
#define SCENE_PATH std::string("C:/Users/Monthy/Documents/projects/thesis/thesis-data-suite/scenes/ziggurat-city/scene-json/scene.json")
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
  nTiled::world::PointLight light0 = nTiled::world::PointLight("light",
                                                               glm::vec4(1.0, 1.0, 1.0, 1.0),
                                                               glm::vec3(1.0, 1.0, 1.0),
                                                               1.0,
                                                               true,
                                                               std::map<std::string, nTiled::world::Object*>());
  nTiled::world::PointLight light1 = nTiled::world::PointLight("light",
                                                               glm::vec4(5.0, 5.0, 5.0, 1.0),
                                                               glm::vec3(1.0, 1.0, 1.0),
                                                               1.5,
                                                               true,
                                                               std::map<std::string, nTiled::world::Object*>());
  
  //nTiled::pipeline::hierarchical::SLTBuilder builder =  nTiled::pipeline::hierarchical::SLTBuilder(0.4,
  //                                                                                               glm::vec4(-1.8, -1.8, -1.8, 1.0));
  //nTiled::pipeline::hierarchical::SingleLightTree* slt = builder.buildSingleLightTree(light, 0);

  auto builder = nTiled::pipeline::hierarchical::LightOctreeBuilder();
  std::vector<nTiled::world::PointLight*> lights = std::vector<nTiled::world::PointLight*>();
  lights.push_back(&light0);
  lights.push_back(&light1);

  auto octree = builder.buildLightOctree(lights, 0.75);
 
  std::string path_octree = "C:/Users/Monthy/Documents/octree_test.json";
  std::string path_lights = "C:/Users/Monthy/Documents/lights_test.json";

  nTiled::pipeline::hierarchical::exportToJson(path_lights, path_octree, lights, *octree);
  //nTiled::pipeline::hierarchical::exportLightOctree(path, *octree);


  //slt->exportToJson(path);

  //delete slt;
  */
  return 0;
}