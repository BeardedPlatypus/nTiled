#include "world\World.h"

namespace nTiled {
namespace world {

// ----------------------------------------------------------------------------
//  Constructor
// ----------------------------------------------------------------------------
World::World() : p_mesh_catalog(std::vector<Mesh*>()),
                 p_objects(std::vector<Object*>()) { }

// ----------------------------------------------------------------------------
//  Destructor
// ----------------------------------------------------------------------------
World::~World() {
  for (Object* p_obj : this->p_objects) {
    delete p_obj;
  }

  for (Mesh* p_mesh : this->p_mesh_catalog) {
    delete p_mesh;
  }
}

// ----------------------------------------------------------------------------
//  Construction methods
// ----------------------------------------------------------------------------
Mesh* World::constructMesh(const std::vector<glm::vec4>& vertices,
                           const std::vector<glm::vec3>& normals,
                           const std::vector<glm::vec3>& uvs,
                           const std::vector<glm::tvec3<glm::u32>> elements) {
  Mesh* p_mesh = new Mesh(vertices,
                          normals,
                          uvs,
                          elements);
  this->p_mesh_catalog.push_back(p_mesh);
  return p_mesh;
}

Object* World::constructObject(const std::string& name,
                               Mesh* p_mesh,
                               glm::mat4 transformation_matrix,
                               pipeline::ShaderKey shader,
                               std::map<std::string, std::string> texture_map,
                               bool is_rendered) {
  Object* p_obj = new Object(name,
                             *p_mesh,
                             transformation_matrix,
                             shader,
                             texture_map,
                             is_rendered);
  this->p_objects.push_back(p_obj);
  return p_obj;
}



} // world
} // nTiled