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

class KeyComputeShader : public ComputeShader {
public:
  /*!
   Construct a new KeyComputeShader, that takes at input a depth map texture 
   reference.
   */
  KeyComputeShader(GLuint depth_texture,
                   const state::View& view,
                   glm::uvec2 tile_size);
  ~KeyComputeShader();

  /*!
   Compute the Key texture, which subdivides depth space into a discrete set 
   of bins per tile.
   */
  virtual void execute();
  
  /*!
   Get the opengl pointer to the k_texture to which this ComputeShader outputs
   */
  virtual GLuint getKTexture() const { return this->k_texture; }

  /*!
   Visualise the k texture of this KeyComputeShader
   */
  virtual void debugVisualise();
  
protected:
  // Attributes
  // --------------------------------------------------------------------------
  /*! Number of tiles in the x direction */
  unsigned int n_tiles_x;
  /*! Number of tiles in the y direction */
  unsigned int n_tiles_y;

  // Compute shader
  // --------------------------------------------------------------------------
  /*!
   Compile and load the compute shader of this KeyComputeShader
   */
  void loadComputeShader();

  /*! Compute shader reference */
  GLuint key_compute_shader;

  // Program attributes
  //---------------------------------------------------------------------------
  /*! Depth texture this KeyComputeShader acts upon */
  const GLuint depth_texture;
  /*! K texture this KeyComputeShader writes too after each execute */
  GLuint k_texture;

  // Debug componets
  //---------------------------------------------------------------------------
  /*! Debug Shader for visualising k texture */
  virtual void loadDebugVisualiseShaders();

  GLuint key_visualise_shader;
  GLuint uniform_max_k;

  PipelineObject* display_quad;

  glm::uvec2 tile_size;
  glm::uvec2 n_tiles;
  glm::uvec2 viewport;
};

}
}
}
