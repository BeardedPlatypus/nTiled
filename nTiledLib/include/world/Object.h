/*! @file Object.h
 *  @brief Object.h contains the definition of the Object
 */
#pragma once

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <map>

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "world\Mesh.h"
#include "pipeline\ShaderKey.h"


namespace nTiled {
namespace world {

/*! @brief Object holds all relevant information of a single object
 */
struct Object {
public:
  // --------------------------------------------------------------------------
  //  Constructor
  // --------------------------------------------------------------------------
  /*! @brief Construct a new Object with the given parameters
   * 
   * @param name the name of this new Object
   * @param mesh the mesh of this new Object
   * @param transformation_matrix The transformation of model to 
   *                              world coordinates of this Object
   * @param shader The shader used to render this object
   * @param texture_map The textures used to render this object
   * @param is_rendered whether this object should be rendered
   */
  Object(const std::string& name,
         const Mesh& mesh,
         glm::mat4 transformation_matrix,
         pipeline::ShaderKey shader,
         std::map<std::string, std::string> texture_map,
         bool is_rendered);

  // --------------------------------------------------------------------------
  //  Member Variables
  // --------------------------------------------------------------------------
  /*! @brief Name of this object */
  const std::string name;

  // --------------------------------------------------------------------------
  //  Geometry
  /*! @brief Reference to the mesh of this Object*/
  const world::Mesh& mesh;
  /*! @brief Transformation from model to world coordinates of this Object */
  glm::mat4 transformation_matrix;

  // --------------------------------------------------------------------------
  //  Shading
  /*! @brief The Shader specification of this Object */
  pipeline::ShaderKey shader_key;

  /*! @brief Object texture name to Unique Texture Id map of this Object */
  std::map<std::string, std::string> texture_map;

  /*! @brief Whether this object should be rendered */
  bool is_rendered;
};


} // world 
} // nTiled