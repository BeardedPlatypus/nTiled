/*! @file KeyComputeShader.h
 *  @brief KeyComputeShader.h contains the definitions of the 
 *         KeyComputeShader used to calculate the keys of pixels within the
 *         Clustered Shading algorithm. 
 */
#pragma once

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <glad\glad.h>
#include <glm\glm.hpp>

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "pipeline\light-management\clustered\compute-client\ComputeShader.h"
#include "state\StateView.h"

// Debug
#include "pipeline\PipelineObject.h"

namespace nTiled {
namespace pipeline {
namespace clustered {

/*! @brief KeyComputeShader is a ComputeShader responsible for calculating 
 *         the k value per pixels and returning a texture containing all 
 *         these values.
 */
class KeyComputeShader : public ComputeShader {
public:
  /*! @brief Construct a new KeyComputeShader with the given parameters.
   *
   * @param depth_texture The depth texture which is used to calculate
   *                      the k value of each pixel
   * @param view Reference to the View this KeyComputeShader observes.
   * @param tile_size The tile size in pixels that is used in this Clustered
   *                  Shading algorithm.
   */
  KeyComputeShader(GLuint depth_texture,
                   const state::View& view,
                   glm::uvec2 tile_size);

  /*! @brief Destruct this KeyComputeShader. */
  ~KeyComputeShader();

  /*! @brief Compute the Key texture, which subdivides depth space into 
   *         a discrete set of bins per tile.
   */
  virtual void execute() override;
  
  /*! @brief Get the opengl pointer to the k_texture to which this 
   *         ComputeShader outputs
   *
   * @return openGL pointer to the k_texture of this KeyComputeShader.
   */
  virtual GLuint getKTexture() const { return this->k_texture; }

  /*! @brief Visualise the k texture of this KeyComputeShader
   */
  virtual void debugVisualise();
  
protected:
  // Attributes
  // --------------------------------------------------------------------------
  /*! @brief Number of tiles */
  glm::uvec2 n_tiles;

  // Compute shader
  // --------------------------------------------------------------------------
  /*! @brief Compile and load the compute shader of this KeyComputeShader
   */
  void loadComputeShader();

  /*! @brief openGL pointer to the KeyComputeShader in video memory. */
  GLuint key_compute_shader;

  // Program attributes
  //---------------------------------------------------------------------------
  /*! @brief Depth texture this KeyComputeShader acts upon */
  const GLuint depth_texture;
  /*! @brief K texture this KeyComputeShader writes too after each execute */
  GLuint k_texture;

  // Debug componets
  //---------------------------------------------------------------------------
  /*! @brief Debug Shader for visualising k texture */
  virtual void loadDebugVisualiseShaders();

  /*! @brief openGL pointer to the KeyVisualise shader of this 
   *         KeyComputeShader. */
  GLuint key_visualise_shader;
  /*! @brief openGL pointer to the uniform value which holds the max k value */
  GLuint uniform_max_k;

  /*! @brief pointer to the PipelineObject holding a screenspace quad. */
  PipelineObject* display_quad;

  /*! @brief The tile size in pixels of this KeyComputeShader. */
  glm::uvec2 tile_size;
  /*! @brief The size of the viewport in pixels of this KeyComputeShader. */
  glm::uvec2 viewport;
};

} // clustered
} // pipeline
} // nTiled
