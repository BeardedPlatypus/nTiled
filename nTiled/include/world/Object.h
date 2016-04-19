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
struct Object {
public:
  // --------------------------------------------------------------------------
  //  Constructor
  // --------------------------------------------------------------------------
  /*! 
   * Construct a new Object with the given parameters
   * 
   * Param: 
   *     name (const string&): the name of this new Object
   *     mesh (const Mesh&): the mesh of this new Object
   *     transformation_matrix (glm::mat4): The transformation of model to 
   *                                        world coordinates of this Object
   *     is_rendered (bool): whether this object should be rendered
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
  /*! Name of this object */
  const std::string name;

  // --------------------------------------------------------------------------
  //  Geometry
  /*! Reference to the mesh of this Object*/
  const world::Mesh& mesh;
  /*! Transformation from model to world coordinates of this Object */
  glm::mat4 transformation_matrix;

  // --------------------------------------------------------------------------
  //  Shading
  /*! The Shader specification of this Object */
  pipeline::ShaderKey shader_key;

  /*! Object texture name to Unique Texture Id map of this Object */
  std::map<std::string, std::string> texture_map;

  bool is_rendered;
};


} // world 
} // nTiled