#include <iostream>

#include "main\DataController.h"


// ----------------------------------------------------------------------------
//  Main
// ----------------------------------------------------------------------------
int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << "<path_to_scene_def.json" << std::endl;
    return -1;
  }

  nTiled::main::DataController controller =
    nTiled::main::DataController(argv[1]);


  controller.init();
  controller.execute();
  controller.exportData();

  return 0;
}
