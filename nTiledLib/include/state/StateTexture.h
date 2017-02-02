/*! @file StateTexture.h
 *  @brief StateTexture.h contains the definition of the TextureCatalog
 */
#pragma once

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <string>
#include <map>

#include <glad\glad.h>


namespace nTiled {
namespace state {

/*! @brief TextureCatalog contains all information related to textures in a
 *         nTiled execution.
 */
class TextureCatalog {
public:
  // Constructor
  /*! @brief Construct a new TextureCatalog with the given file_map and 
   *         memory map.
   * 
   * @param file_map map containing texture ids to texture file paths
   * @param memory_map map containing texture ids to memory locations
   */
  TextureCatalog(std::map<std::string, std::string> file_map,
                 std::map<std::string, GLuint> memory_map);

  /*! @brief construct a new TextureCatalog with the given file_map 
   * 
   * @param file_map map containing texture ids to texture file paths
   */
  TextureCatalog(std::map<std::string, std::string> file_map);

  // Texture access method
  /*! @brief Get the memory location of the texture with the given texture_id
   * 
   * @param texture_id The unique string of the texture which should be 
   *        retrieved.
   */
  GLuint getTexture(std::string texture_id);

private:
  // Texture maps
  /*! @brief map containing texture ids to texture file paths. */
  std::map<std::string, std::string> file_map;
  /*! @brief map containing texture ids to memory locations. */
  std::map<std::string, GLuint> memory_map;
};

} // state
} // nTiled
