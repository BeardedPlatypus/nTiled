#pragma once

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "world\object-constructor\ObjectConstructor.h"


namespace nTiled {
namespace world {

class AssImpConstructor : public PrimitiveConstructor {
public:
  /*!
   * Construct a new Object constructor that modifies the specified world
   * by adding objects with the specified mesh.
   */
  AssImpConstructor(const std::string& path,
                    World& world);

  /*!
   * Add a new object with this AssImpConstructor's Mesh to this 
   * AssImpConstructor's world
   * 
   * Param: 
   *     name (const string&): name of the new Object
   *     shader (pipeline::ShaderKey): the shader of the new Object
   *     texture_map (map<string, string>): map containing the texture names
   *     transformation_matrix (glm::mat4): transformation of this Object
   *     is_rendered (bool): whether this object is rendered.
   */
  Object* add(const std::string& name,
              glm::mat4 transformation_matrix,
              pipeline::ShaderKey shader_id,
              std::map<std::string, std::string> texture_map,
              bool is_rendered);

private:
  /*! The world this AssImpConstructor modifies */
  World& world;
  /*! Pointer to the mesh with which new objects are added */
  Mesh* obj_mesh;
};


} // world
} // nTiled
