#include "world\Object.h"

namespace nTiled {
namespace world {
Object::Object(const std::string& name,
               const Mesh& mesh,
               glm::mat4 transformation_matrix,
               pipeline::ShaderKey shader,
               std::map<std::string, std::string> texture_map,
               bool is_rendered) :
  name(name),
  mesh(mesh),
  transformation_matrix(transformation_matrix),
  is_rendered(is_rendered) { }

}
}