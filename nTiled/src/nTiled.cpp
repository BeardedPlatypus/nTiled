// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include "main\Controller.h"
#include <iostream>

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

  return 0;
}