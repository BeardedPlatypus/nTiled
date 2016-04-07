#pragma once

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "world\World.h"

namespace nTiled {
namespace world {
// ------------------------------------------------------------------------
//  Abstract class
class PrimitiveConstructor {
public:
  virtual ~PrimitiveConstructor() {}
  virtual Object* add(const std::string& name,
                      glm::mat4 transformationMatrix,
                      pipeline::ShaderKey shader_id,
                      std::map<std::string, std::string> texture_map,
                      bool is_rendered) = 0;
};

} // world
} // nTiled
