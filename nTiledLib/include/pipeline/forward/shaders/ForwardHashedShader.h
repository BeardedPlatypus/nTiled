#pragma once

// ----------------------------------------------------------------------------
//  nTiled Headers
// ----------------------------------------------------------------------------
#include "ForwardShader.h"
#include "pipeline\light-management\hashed\HashedLightManager.h"
#include "pipeline\light-management\hashed\HashedLightManagerBuilder.h"


namespace nTiled {
namespace pipeline {

class ForwardHashedShader : public ForwardShader {
public:
  /*! @brief Construct a new ForwardHashedShader with the given parameters
   * 
   * @param shader_id The ForwardShaderId of this new ForwardShader
   * @param path_vertex_shader Path to the vertex shader file.
   * @param path_fragment_shader Path to the fragment shader file.
   * @param world Reference to the world containing the objects and lights 
   *              this new ForwardShader observes
   * @param view Reference to the View and Camera this ForwardShader uses 
   *             to shade.
   * @param p_output_buffer openGL pointer to the output buffer to which
   *                        this ForwardShader should render.
   * @param light_manager_builder HashedLightManagerBuilder which constructs 
   *                              the HashedLightManager of this 
   *                              ForwardHashedShader
   * @param config The configuration data for the HashedLightManager
   */
  ForwardHashedShader(ForwardShaderId shader_id,
                      const std::string& path_vertex_shader,
                      const std::string& path_fragment_shader,
                      const world::World& world,
                      const state::View& view,
                      GLint p_output_buffer,
                      const hashed::HashedLightManagerBuilder& light_manager_builder,
                      const hashed::HashedConfig& config);

  virtual void render() override;

protected:
  virtual void loadShaders(const std::string& path_vert_shader,
                           const std::string& path_frag_shader) override;

private:
  /*! @brief The HashedLightManager of this ForwardHashedShader. */
  hashed::HashedLightManager* p_light_manager;
};


}
}