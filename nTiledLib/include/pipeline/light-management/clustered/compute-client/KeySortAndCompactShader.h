/*! @file KeySortAndCompactShader.h 
 *  @brief KeySortAndCompactShader.h contains the definition of the 
 *         KeySortAndCompactShader.
 */
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

/*! @brief KeySortAndCompactShader is a ComputeShader responsible for 
 *         sorting and compacting the keys computed in the KeySortShader.
 */
class KeySortAndCompactShader : public ComputeShader {
public:
  // --------------------------------------------------------------------------
  // Constructor | Destructor
  // --------------------------------------------------------------------------
  /*! @brief Construct a new KeySortAndCompactShader with the given parameters.
   *
   * Construct a new KeySortAndcompactShader which will use the imagetexture 
   * specified at k_tex, to consturt a set of unique clusters in k_tex, as 
   * well as an index map.
   *
   * @param k_tex openGL pointer to the k_tex imagetexture
   * @param view The View in which context this KeySortAndCompactShader will be
   *             executed.
   * @param tile_size The tile size in pixels of each cluster
   */
  KeySortAndCompactShader(GLuint k_tex,
                          const state::View& view,
                          glm::uvec2 tile_size);

  // --------------------------------------------------------------------------
  // Execute
  // --------------------------------------------------------------------------
  /*! @brief Compute the unique k values, k_index_map and total number of 
   *         unique  k_values per tile, based on the k_tex this 
   *         KeySortAndCompactShader was constructed with.
   */
  virtual void execute() override;

  // --------------------------------------------------------------------------
  // Attribute access methods
  // --------------------------------------------------------------------------
  /*! @brief Get the pointer to the index texture.
   */
  virtual GLuint getIndexTexture() const { 
    return this->cluster_index_texture; 
  }

  /*! @brief Get the total number of indices per tile
   */
  virtual std::vector<GLushort> getNIndicesTiles() const { 
    return this->n_indices_tiles; 
  }

  /*! @brief Get the unique k_values per tile
   *
   * The values are structured as follows
   * |(tile (0, 0): k_1, k_2, ..., k_n | (tile (0, 1): ... | etc
   * where k_n corresponds with the total number of unique clusters in that tile
   */
  virtual std::vector<GLushort> getKValuesTiles() const {
    return this->k_values_tiles;
  }

protected:
  /*! @brief Pointer to the opengl compute program of this 
   *         KeySortAndCompactShader */
  GLuint key_sort_shader;

  // input
  // --------------------------------------------------------------------------
  /*! @brief openGL pointer to the input k_tex */
  const GLuint k_texture;

  // output
  // --------------------------------------------------------------------------
  /*! @brief openGL pointer to the cluster index texture */
  GLuint cluster_index_texture;
  /*! @brief openGL pointer to the unique_clusters_texture */
  GLuint unique_clusters_texture;
  /*! @brief openGL pointer to the n_clusters imagetexture */
  GLuint n_clusters_texture;

  /*! @brief vector containing the number of indices per tile after executing
   *         this compute shader.
   */
  std::vector<GLushort> n_indices_tiles;
  /*! @brief vector containing the k values per tile. */
  std::vector<GLushort> k_values_tiles;

  // --------------------------------------------------------------------------
  // Management functions and values
  // --------------------------------------------------------------------------
  /*! @brief Load and compile the compute shader into video memory.
   */
  void loadComputeShader();

  /*! @brief Specify the texture format and dimensions.
   */
  void specifyTexture(GLuint tex_name,
                      GLenum format,
                      glm::uvec2 dimensions);

  /*!  @briefcluster tile size (xy dimensions) */
  const glm::uvec2 tile_size;
  /*!  @briefNumber of tiles */
  const glm::uvec2 n_tiles;
  /*!  @briefviewport size */
  const glm::uvec2 viewport;

  // --------------------------------------------------------------------------
  //  DebugMethods
  // --------------------------------------------------------------------------
  /*! @brief Debug method to print values to console. */
  void printToConsole(std::string message,
                      GLushort* values, 
                      unsigned int total_size);
};

}
}
}
