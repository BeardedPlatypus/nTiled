#include "world\Mesh.h"

namespace nTiled {
namespace world {

// ----------------------------------------------------------------------------
//  Mesh
// ----------------------------------------------------------------------------
Mesh::Mesh(std::vector<glm::vec4> vertices,
           std::vector<glm::vec3> normals,
           std::vector<glm::vec3> uvs,
           std::vector<glm::tvec3<glm::u32>> elements) : 
  vertices(vertices),
  normals(normals),
  uvs(uvs),
  elements(elements) { 
}

Mesh::Mesh() : Mesh(std::vector<glm::vec4>(),
                    std::vector<glm::vec3>(),
                    std::vector<glm::vec3>(),
                    std::vector<glm::tvec3<glm::u32>>()) {
}

}
}
