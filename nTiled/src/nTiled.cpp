// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include "main\Controller.h"
#include <iostream>

// ----------------------------------------------------------------------------
//  Defines
// ----------------------------------------------------------------------------
//#define SCENE_PATH std::string("C:/Users/Monthy/Documents/projects/thesis/thesis-data-suite/video/scene_def/spaceship-indoor/70#1x_1y_1z/forward_hashed.json")
//#define SCENE_PATH std::string("C:/Users/Monthy/Documents/projects/thesis/thesis-data-suite/video/scene_def/pipers-alley/58#1x_1y_1z/deferred_attenuated.json")
//#define SCENE_PATH std::string("C:/Users/Monthy/Documents/projects/thesis/thesis-data-suite/video/scene_def/ziggurat-city/65#1x_1y_1z/forward_attenuated.json")
//#define SCENE_PATH std::string("C:/Users/Monthy/Documents/projects/thesis/thesis-data-suite/scenes/pipers-alley/test-scene/forward_attenuated_scene.json")
//#define SCENE_PATH std::string("C:/Users/Monthy/Documents/projects/thesis/thesis-data-suite/scenes/pipers-alley/test-scene/forward_hashed_scene.json")
//#define SCENE_PATH std::string("C:/Users/Monthy/Desktop/pipers-alley-hashed/pipers_alley_hashed.json")
//#define SCENE_PATH std::string("C:/Users/Monthy/Documents/projects/thesis/thesis-data-suite/video/scene_def/ziggurat-city/65#1x_1y_1z/forward_attenuated.json")
//#define SCENE_PATH std::string("C:/Users/Monthy/Desktop/ziggurat-city-hashed/ziggurat_city_hashed.json")
//#define SCENE_PATH std::string("C:/Users/Monthy/Documents/projects/thesis/thesis-data-suite/scenes/plane-test/scene-def/forward_attenuated.json")
//#define SCENE_PATH std::string("C:/Users/Monthy/Documents/projects/thesis/thesis-data-suite/scenes/plane-test/scene-def/forward_hashed.json")
//#define SCENE_PATH std::string("../example/example_scene_hashed_def1.json")
//#define SCENE_PATH std::string("../example/example_scene_hashed_def2.json")
//#define SCENE_PATH std::string("../example/example_scene_hashed.json")
//#define SCENE_PATH std::string("../example/example_scene_hashed_def_.json")
//#define SCENE_PATH std::string("C:/Users/Monthy/Documents/projects/thesis/thesis-data-suite/node-size-det/config_0.json")
#define SCENE_PATH std::string("C:\\Users\\Monthy\\Documents\\projects\\thesis\\thesis-data-suite\\validate_node-size\\scenes\\forward\\spaceship-indoor\\560#2x_2y_2z\\conf_0.25.json")
//#define SCENE_PATH std::string("C:/Users/Monthy/Documents/projects/thesis/thesis-data-suite/data/raw/hashed_exec_lights/deferred/pipers-alley/58#1x_1y_1z/0.507/config_0.json")
//#define SCENE_PATH std::string("C:/Users/Monthy/Documents/projects/thesis/thesis-data-suite/data/raw/hashed_exec_lights/deferred/ziggurat-city/65#1x_1y_1z/0.507/config_0.json")
//#define SCENE_PATH std::string("C:/Users/Monthy/Documents/projects/thesis/thesis-data-suite/scenes/pipers-alley/validation-scenes/arch/scene.json")
//#define SCENE_PATH std::string("C:/Users/Monthy/Documents/projects/thesis/thesis-data-suite/scenes/ziggurat-city/scene-json/scene.json")
//#define SCENE_PATH std::string("../example/example_scene_clustered.json")
//#define SCENE_PATH std::string("C:/Users/Monthy/Documents/projects/thesis/thesis-data-suite/data/raw/clustered_exec_lights/deferred/spaceship-indoor/70#1x_1y_1z/config_0.json")
//#define SCENE_PATH std::string("C:/Users/Monthy/Documents/projects/thesis/thesis-data-suite/data/raw/ts-resolution/deferred/spaceship-indoor/16/320/config_0.json")
//#define SCENE_PATH std::string("C:/Users/Monthy/Documents/projects/thesis/calc_test/config_0.json")
//#define SCENE_PATH std::string("C:/Users/Monthy/Documents/projects/thesis/thesis-data-suite/data/raw/light-calc/resolution/ts/deferred/pipers-alley/8/320/config.json")
//#define SCENE_PATH std::string("C:/Users/Monthy/Documents/projects/thesis/thesis-data-suite/data/raw/light-visualisation/ziggurat-city/tiled/config.json")
//#define SCENE_PATH std::string("C:/Users/Monthy/Documents/projects/thesis/thesis-data-suite/data/raw/light-visualisation-vid/spaceship-indoor/tiled/config.json")
//#define SCENE_PATH std::string("C:/Users/Monthy/Documents/deferred_tiled.json")
//#define SCENE_PATH std::string("C:/Users/Monthy/Documents/projects/thesis/thesis-data-suite/data/raw/lc-summed/lights/hs/deferred/pipers-alley/3.31111/1044#3x_3y_2z/config.json")
//#define SCENE_PATH std::string("C:/Users/Monthy/Documents/projects/thesis/thesis-data-suite/data/raw/light-visualisation-sd/spaceship-indoor/1/config.json")
//#define OUTPUT_PATH_BASE = std::string("C:/Users/Monthy/Documents/projects/thesis/hierarchical-shading/test/")

//#define SCENE_PATH std::string("C:/Users/Monthy/Desktop/lets-burn-this-fucker-down/deferred_clustered.json")
//#define SCENE_PATH std::string("C:/Users/Monthy/Documents/projects/thesis/test_logger/config_0.json")
//#define SCENE_PATH std::string("C:/Users/Monthy/Desktop/ZCgbuffer/deferred_attenuated.json")

//#define SCENE_PATH std::string("../example/example_scene_fa.json")
//#define SCENE_PATH std::string("../example/example_scene_ft.json")
//#define SCENE_PATH std::string("../example/example_scene_da.json")
//#define SCENE_PATH std::string("../example/example_scene_dt.json")

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