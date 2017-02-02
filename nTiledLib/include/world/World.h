/*! @file World.h
 *  @brief World.h contains the World definition
 */
#pragma once

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <string>

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "world\Object.h"
#include "world\PointLight.h"


namespace nTiled {
namespace world {

/*! @brief World contains all objects, meshes and lights of a single run of 
 *         nTiled.
 */
class World {
public:
  // --------------------------------------------------------------------------
  //  Constructor
  // --------------------------------------------------------------------------
  /*! @brief Construct a new empty World.
   *
   * @return a new empty world.
   */
  World();

   // --------------------------------------------------------------------------
   //  Destructor
   // --------------------------------------------------------------------------
  /*! @brief Destruct this world. Remove all allocated meshes and objects.
   */
  ~World();

  // --------------------------------------------------------------------------
  //  Construction methods
  // --------------------------------------------------------------------------
  /*! @brief Construct a new Mesh with the given parameters and add it to this 
   *          World.
   * 
   * @param vertices The vertices of this new Mesh
   * @param normals The normals of this new Mesh
   * @param uvs The uv coordinates of this new Mesh
   * @param elements the primitives of this new Mesh
   */
  Mesh* constructMesh(const std::vector<glm::vec4>& vertices,
                      const std::vector<glm::vec3>& normals,
                      const std::vector<glm::vec3>& uvs,
                      const std::vector<glm::tvec3<glm::u32>> elements);

  /*! @brief Construct A new Object with the given parameters and add it to
   *         this World.
   * 
   * @param name The name of this new Object
   * @param p_mesh a pointer to the Mesh of this new Object
   * @param transformation_matrix The transformation matrix of this new 
   *                               object.
   * @param shader The shader with which this new Object should be rendered.
   * @param texture_map The textures used to render this new Object.
   * @param is_rendered Whether this new Object should be rendered.
   */
  Object* constructObject(const std::string& name,
                          Mesh* p_mesh,
                          glm::mat4 transformation_matrix,
                          pipeline::ShaderKey shader,
                          std::map<std::string, std::string> texture_map,
                          bool is_rendered);

  /*! @brief Construct a new PointLight with the given parameters and add it 
   *         to this World.
   * 
   * @param name The name of this new PointLight
   * @param position The position in world coordinates of this new PointLight
   * @param intensity The intensity of this new PointLight
   * @param radius The radius of this new PointLight
   * @param is_emitting Whether this new PointLight is emitting light
   * @param debug_light_objects The debug_light_objects with which this new
   *                            PointLight is rendered in debug mode
   */
  PointLight* constructPointLight(const std::string& name,
                                  glm::vec4 position,
                                  glm::vec3 intensity,
                                  float radius,
                                  bool is_emitting,
                                  std::map<std::string, Object*> debug_light_objects);

  // --------------------------------------------------------------------------
  //  Class members
  // --------------------------------------------------------------------------
  /*! @brief std::vector of pointers to every mesh of this World */
  std::vector<Mesh*> p_mesh_catalog;
  /*! @brief std::vector of pointers to every object of this World */
  std::vector<Object*> p_objects;
  /*! @brief std::vector of pointers to every PointLight of this World */
  std::vector<PointLight*> p_lights;
};

}
}

