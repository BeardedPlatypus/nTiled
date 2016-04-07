#include "state\StateTexture.h"

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "pipeline\shader-util\LoadTextures.h"

namespace nTiled {
namespace state {

// ----------------------------------------------------------------------------
//  Constructors
// ----------------------------------------------------------------------------
TextureCatalog::TextureCatalog(std::map<std::string, std::string> file_map,
                               std::map<std::string, GLuint> memory_map) :
  file_map(file_map),
  memory_map(memory_map) { }

TextureCatalog::TextureCatalog(std::map<std::string, std::string> file_map) :
    TextureCatalog(file_map, std::map<std::string, GLuint>()) { }


// ----------------------------------------------------------------------------
//  Accessors
// ----------------------------------------------------------------------------
GLuint TextureCatalog::getTexture(std::string texture_id) {
  auto memory_iter = this->memory_map.find(texture_id);

  if (memory_iter != this->memory_map.end()) {
    return memory_iter->second;
  } else {
    auto file_iter = this->file_map.find(texture_id);
    if (file_iter != this->file_map.end()) {
      GLuint texture_p = pipeline::loadTexturePNG(file_iter->second);

      this->memory_map.insert(std::pair<std::string, GLuint>(texture_id,
                                                             texture_p));

      return texture_p;
    } else {
      throw std::runtime_error(
        std::string("Texture with id: ") + texture_id + 
        std::string(" does not exist."));
    }
  }
}

} // state
} // nTiled