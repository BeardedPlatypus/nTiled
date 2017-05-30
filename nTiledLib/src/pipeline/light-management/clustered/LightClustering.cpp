#include "pipeline\light-management\clustered\LightClustering.h"

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

namespace nTiled {
namespace pipeline {
namespace clustered {

LightClustering::LightClustering(glm::uvec2 dimensions,
                                 glm::uvec2 tile_size) :
    tile_size(tile_size),
    image_dimensions(dimensions) {
  // construct cluster_map_raw  
  // -------------------------
  // calc number of tiles x
  unsigned int n_x = this->image_dimensions.x / this->tile_size.x;
  if (this->tile_size.x * n_x < this->image_dimensions.x) {
    n_x++;
  }

  // calc number of tiles y
  unsigned int n_y = this->image_dimensions.y / this->tile_size.y;
  if (this->tile_size.y * n_y < this->image_dimensions.y) {
    n_y++;
  }

  this->n_tiles = glm::uvec2(n_x, n_y);

  this->cluster_map_raw = 
    std::vector<std::vector<std::pair<GLushort, std::vector<GLuint>>>>(n_tiles.x * n_tiles.y);

  for (unsigned int i = 0; i < this->n_tiles.x * n_tiles.y; i++) {
    this->cluster_map_raw[i] = std::vector<std::pair<GLushort, std::vector<GLuint>>>();
  }
}


void LightClustering::initFrame(const std::vector<GLushort>& unique_clusters,
                                const std::vector<GLushort>& n_clusters_tile) {
  unsigned int offset = 0;
  for (unsigned int i = 0; i < this->n_tiles.x * this->n_tiles.y; i++) {
    // empty the cluster_map_raw from previous frame
    this->cluster_map_raw[i].clear();

    // add new k_values
    for (unsigned int j = 0; j < n_clusters_tile[i]; j++) {
      this->cluster_map_raw[i].push_back(
        std::pair<GLushort, std::vector<GLuint>>(unique_clusters[offset + j],
                                                 std::vector<GLuint>()));
    }
    offset += n_clusters_tile[i];
  }
}


void LightClustering::incrementLight(glm::uvec3 frustrum_begin,
                                     glm::uvec3 frustrum_end,
                                     GLuint light_index) {
  unsigned int tile_index;
  unsigned int k_i;

  // Loop over each tile
  for (unsigned int y_i = frustrum_begin.y; y_i <= frustrum_end.y; y_i++) {
    for (unsigned int x_i = frustrum_begin.x; x_i <= frustrum_end.x; x_i++) {
      tile_index = y_i * n_tiles.x + x_i;
      k_i = 0;

      // loop over the k_values until either all are checked or 
      // the k_value overlaps with the light frustrum
      while (k_i < this->cluster_map_raw[tile_index].size() &&
             this->cluster_map_raw[tile_index][k_i].first < frustrum_begin.z) {
        k_i++;
      }
      // loop over the k_values until either all are checked or
      // the k_value does not overlap anymore with the light frustrum
      while (k_i < this->cluster_map_raw[tile_index].size() &&
             this->cluster_map_raw[tile_index][k_i].first <= frustrum_end.z) {
        this->cluster_map_raw[tile_index][k_i].second.push_back(light_index);
        k_i++;
      }
    }
  }
}


void LightClustering::finaliseClusters() {
  this->cluster_to_light_index_map.clear();
  this->light_index_list.clear();

  unsigned int current_offset = 0;
  unsigned int n_indices;

  // loop over tiles
  for (unsigned int i = 0; i < this->n_tiles.x * this->n_tiles.y; i++) {
    // loop over k_values
    for (unsigned int k_i = 0; k_i < this->cluster_map_raw[i].size(); k_i++) {
      // add light indices of this k_val to the light index list
      for (GLuint light_index : this->cluster_map_raw[i][k_i].second) {
        light_index_list.push_back(light_index);
      }

      n_indices = this->cluster_map_raw[i][k_i].second.size();
      this->cluster_to_light_index_map.push_back(glm::uvec2(current_offset,
                                                            n_indices));
      current_offset += n_indices;
    }
  }
}

} // clustered
} // pipeline
} // nTiled