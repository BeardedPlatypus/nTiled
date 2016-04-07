#pragma once

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <string>
#include <map>

#include <glad\glad.h>

namespace nTiled {
namespace state {

class TextureCatalog {
 public:
  // Constructor
  TextureCatalog(std::map<std::string, std::string> file_map,
                 std::map<std::string, GLuint> memory_map);
  TextureCatalog(std::map<std::string, std::string> file_map);

  // Texture access method
  GLuint getTexture(std::string texture_id);

 private:
  // Texture maps
  std::map<std::string, std::string> file_map;
  std::map<std::string, GLuint> memory_map;
};

} // state
} // nTiled
