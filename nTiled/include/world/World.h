#pragma once

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <string>

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "world\Object.h"

namespace nTiled {
namespace world {

class World {
public:
  // --------------------------------------------------------------------------
  //  Constructor
  // --------------------------------------------------------------------------
  /*!
   * Construct a new empty World.
   *
   * Return:
   *     a new empty world.
   */
   // --------------------------------------------------------------------------
   //  Destructor
   // --------------------------------------------------------------------------
  World();
  /*!
   * Destruct this world. Remove all allocated meshes and objects.
   */
  ~World();

  // --------------------------------------------------------------------------
  //  Construction methods
  // --------------------------------------------------------------------------
  Mesh* constructMesh(const std::vector<glm::vec4>& vertices,
                      const std::vector<glm::vec3>& normals,
                      const std::vector<glm::vec3>& uvs,
                      const std::vector<glm::tvec3<glm::u32>> elements);

  Object* constructObject(const std::string& name,
                          Mesh* p_mesh,
                          glm::mat4 transformation_matrix,
                          pipeline::ShaderKey shader,
                          std::map<std::string, std::string> texture_map,
                          bool is_rendered);

  // --------------------------------------------------------------------------
  //  Class members
  // --------------------------------------------------------------------------
  /*! vector of pointers to every mesh of this World */
  std::vector<Mesh*> p_mesh_catalog;
  /*! vector of pointers to every object of this World */
  std::vector<Object*> p_objects;
};

}
}

