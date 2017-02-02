/*! @file ObjectConstructor.h 
 *  @brief ObjectConstructor.h contains the interface that should be used 
 *         by all ObjectConstructors within nTiled.
 */
#pragma once

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "world\World.h"


namespace nTiled {
namespace world {

/*! @brief PrimitiveConstructor is the abstract base class of any Object
 *         Constructors of nTiled
 */
class PrimitiveConstructor {
public:
  /*! @brief virtual destructor */
  virtual ~PrimitiveConstructor() {}

  /*! @brief Construct a pointer to a new Object with the given Parameters
   *  
   * @param name The name of this new Object
   * @param transformation_matrix The transformation matrix of this new Object
   * @param shader_id The shader with which this new Object should be rendered.
   * @param texture_map The textures which are used to render this new Object.
   * @param is_rendered Whether this new Object is rendered.
   */
  virtual Object* add(const std::string& name,
                      glm::mat4 transformation_matrix,
                      pipeline::ShaderKey shader_id,
                      std::map<std::string, std::string> texture_map,
                      bool is_rendered) = 0;
};

} // world
} // nTiled
