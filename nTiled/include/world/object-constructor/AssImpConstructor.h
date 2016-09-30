/*! @file AssIpmConstructor.h
 *  @brief AssImpConstructor.h contains the definition of AssImpConstructor
 *         which uses the AssImp library to construct Objects.
 */
#pragma once

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "world\object-constructor\ObjectConstructor.h"


namespace nTiled {
namespace world {

/*! @brief The AssImpConstructor is responsible for loading a single Mesh
 *         through the AssImp library and constructing objects with this Mesh
 */
class AssImpConstructor : public PrimitiveConstructor {
public:
  /*! @brief Construct a new Object constructor that modifies the specified world
   *         by adding objects with the specified mesh.
   * 
   * @param path Path to the Mesh definition file.
   * @param world World to which this AssImpConstructor should add the 
   *        constructed objects.
   */
  AssImpConstructor(const std::string& path,
                    World& world);

  /*! @brief Add a new object with this AssImpConstructor's Mesh to this 
   *         AssImpConstructor's world
   * 
   * @param name Name of the new Object
   * @param shader The shader of the new Object
   * @param texture_map Map containing the texture names
   * @param transformation_matrix Transformation of this Object
   * @param is_rendered Whether this object is rendered.
   */
  Object* add(const std::string& name,
              glm::mat4 transformation_matrix,
              pipeline::ShaderKey shader_id,
              std::map<std::string, std::string> texture_map,
              bool is_rendered);

private:
  /*! @brief The world this AssImpConstructor modifies */
  World& world;
  /*! @brief Pointer to the mesh with which new objects are added */
  Mesh* obj_mesh;
};


} // world
} // nTiled
