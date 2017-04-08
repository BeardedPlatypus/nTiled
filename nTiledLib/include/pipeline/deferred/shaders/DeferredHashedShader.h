#pragma once


// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "DeferredShader.h"
#include "pipeline\light-management\hashed\HashedLightManagerBuilder.h"


namespace nTiled {
namespace pipeline {

class DeferredHashedShader : public DeferredShader {
public:
  /*! @brief Construct a new DeferredHashedShader with the given parameters
   * 
   * @param shader_id The DeferredShaderId of this new DeferredShader
   * @param path_geometry_pass_vertex_shader Path to the geometry pass 
   *                                         vertex shader file.
   * @param path_geometry_pass_fragment_shader Path to the  geometry pass 
   *                                           fragment shader file.
   * @param path_light_pass_vertex_shader Path to the light pass vertex 
   *                                      shader file.
   * @param path_light_pass_fragment_shader Path to the light pass 
   *                                        fragment shader file.
   * @param world Reference to the world containing the objects and lights this 
   *              Shader observes
   * @param view Reference to the View and Camera this shader uses to shade.
   * @param p_output_buffer openGL pointer to the output buffer to which
   *                        this DeferredTiledShader should render.
   * @param light_manager_builder HashedLightManagerBuilder which constructs 
   *                              the HashedLightManager of this 
   *                              DeferredHashedShader
   * @param config The configuration data for the HashedLightManager
   */
  DeferredHashedShader(DeferredShaderId shader_id,
                       const std::string& path_geometry_pass_vertex_shader,
                       const std::string& path_geometry_pass_fragment_shader,
                       const std::string& path_light_pass_vertex_shader,
                       const std::string& path_light_pass_fragment_shader,
                       const world::World& world,
                       const state::View& view,
                       GLint p_output_buffer,
                       const hashed::HashedLightManagerBuilder& light_manager_builder,
                       const hashed::HashedConfig& config);

  virtual void renderGeometryPass() override;
  virtual void renderLightPass() override;

protected:
  virtual void loadShaders(const std::string& path_geometry_vert_shader,
                           const std::string& path_geometry_frag_shader,
                           const std::string& path_light_vert_shader,
                           const std::string& path_light_frag_shader) override;

private:
  /*! @brief Pointer to the HashedLightManager of this DeferredHashedShader. */
  hashed::HashedLightManager* p_light_manager;


};

}
}