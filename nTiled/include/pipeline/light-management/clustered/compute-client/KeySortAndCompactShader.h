#pragma once

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <glad\glad.h>
#include <glm\glm.hpp>

#include <vector>

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "pipeline\light-management\clustered\compute-client\ComputeShader.h"
#include "state\StateView.h"

namespace nTiled {
namespace pipeline {
namespace clustered {

class KeySortAndCompactShader : public ComputeShader {
public:
  // --------------------------------------------------------------------------
  // Constructor | Destructor
  // --------------------------------------------------------------------------
  /*!
   Construct a new KeySortAndCompactShader 

   Construct a new KeySortAndcompactShader which will use the imagetexture 
   specified at k_tex, to consturt a set of unique clusters in k_tex, as 
   well as an index map.

   Param:
       k_tex (GLuint): openGL pointer to the k_tex imagetexture
       tile_size (glm::uvec
   */
  KeySortAndCompactShader(GLuint k_tex,
                          const state::View& view,
                          glm::uvec2 tile_size);
  ~KeySortAndCompactShader();

  // --------------------------------------------------------------------------
  // Execute
  // --------------------------------------------------------------------------
  /*! 
   Compute the unique k values, k_index_map and total number of unique 
   k_values per tile, based on the k_tex this KeySortAndCompactShader was 
   constructed with.
   */
  virtual void execute() override;

  // --------------------------------------------------------------------------
  // Attribute access methods
  // --------------------------------------------------------------------------
  /*! Get the pointer to the index texture */
  virtual GLuint getIndexTexture() const { 
    return this->cluster_index_texture; 
  }

  /*! Get the total number of indices per tile*/
  virtual std::vector<GLushort> getNIndicesTiles() const { 
    return this->n_indices_tiles; 
  }

  /*! 
    Get the unique k_values per tile

    The values are structured as follows
    |(tile (0, 0): k_1, k_2, ..., k_n | (tile (0, 1): ... | etc
    where k_n corresponds with the total number of unique clusters in that tile
    */
  virtual std::vector<GLushort> getKValuesTiles() const {
    return this->k_values_tiles;
  }

protected:
  /*! Pointer to the opengl compute program of this KeySortAndCompactShader */
  GLuint key_sort_shader;

  // input
  // --------------------------------------------------------------------------
  /*! openGL pointer to the input k_tex */
  const GLuint k_texture;

  // output
  // --------------------------------------------------------------------------
  /*! openGL pointer to the cluster index texture */
  GLuint cluster_index_texture;
  /*! openGL pointer to the unique_clusters_texture */
  GLuint unique_clusters_texture;
  /*! openGL pointer to the n_clusters imagetexture */
  GLuint n_clusters_texture;

  std::vector<GLushort> n_indices_tiles;
  std::vector<GLushort> k_values_tiles;

  // --------------------------------------------------------------------------
  // Management functions and values
  // --------------------------------------------------------------------------
  void loadComputeShader();
  void specifyTexture(GLuint tex_name,
                      GLenum format,
                      glm::uvec2 dimensions);

  /*! cluster tile size (xy dimensions) */
  const glm::uvec2 tile_size;
  /*! Number of tiles */
  const glm::uvec2 n_tiles;
  /*! viewport size */
  const glm::uvec2 viewport;
};

}
}
}
