#include "pipeline\light-management\clustered\ClusteredLightManager.h"

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <cmath>

// ----------------------------------------------------------------------------
//  nTiled headers 
// ----------------------------------------------------------------------------
#include "math\util.h"

namespace nTiled {
namespace pipeline {

ClusteredLightManager::ClusteredLightManager(const state::View& view,
                                             const world::World& world,
                                             glm::uvec2 tile_size,
                                             GLuint depth_texture) :
    view(view),
    world(world),
    tile_size(tile_size),
    light_clustering(clustered::LightClustering(view.viewport,
                                                tile_size)),
    projector(*(new BoxProjector())),
    key_compute_shader(clustered::KeyComputeShader(
      depth_texture,
      view,
      tile_size)),
    key_sort_compact_shader(clustered::KeySortAndCompactShader(
      this->key_compute_shader.getKTexture(),
      view, 
      tile_size)) {  
  //   k inv denominator
  float theta = 0.5 * math::to_radians(view.camera.getFoV());
  float tile_width_percentage = (float)tile_size.x / (float)view.viewport.x;
  this->k_inv_denominator = 1.0f / (log(1 + (2 * tan(theta) *
                                    tile_width_percentage)));
}

ClusteredLightManager::~ClusteredLightManager() {
  delete &(this->projector);
}

void ClusteredLightManager::constructClusteringFrame() {
  // Execute compute shaders
  this->key_compute_shader.execute();
  this->key_sort_compact_shader.execute();

  // Extract values
  std::vector<GLushort> n_clusters_tiles = 
    this->key_sort_compact_shader.getNIndicesTiles();
  std::vector<GLushort> k_values_tiles =
    this->key_sort_compact_shader.getKValuesTiles();

  // Calculate summed indices
  this->summed_indices.clear();
  GLuint summed_index = 0;
  for (GLushort n_k_vals : n_clusters_tiles) {
    this->summed_indices.push_back(summed_index);
    summed_index += n_k_vals;
  }

  // Clear Clustering
  this->light_clustering.initFrame(k_values_tiles,
                                   n_clusters_tiles);

  GLuint index = 0;
  glm::uvec4 affected_tiles;

  glm::uvec3 frustrum_begin;
  glm::uvec3 frustrum_end;
  for (const world::PointLight* light : this->world.p_lights) {
    // calculate tiles effected by light if false then no tiles are affected
    // and it can be skipped
    if (this->projector.computeProjection(*light,
                                          this->view.camera,
                                          this->view.viewport,
                                          this->tile_size,
                                          affected_tiles)) {
      frustrum_begin.x = affected_tiles.x;
      frustrum_begin.y = affected_tiles.y;

      frustrum_end.x = affected_tiles.z;
      frustrum_end.y = affected_tiles.z;

      // calculate z_value of light in camera space
      glm::uvec4 light_camera_pos = view.camera.getLookAt() * light->position;
      frustrum_begin.z = GLuint(floor(log(-(light_camera_pos.z + light->radius) / view.camera.getDepthrange().x) * this->k_inv_denominator));
      frustrum_end.z = GLuint(floor(log(-(light_camera_pos.z - light->radius) / view.camera.getDepthrange().x) * this->k_inv_denominator));

      this->light_clustering.incrementLight(frustrum_begin,
                                            frustrum_end,
                                            index);
    }
    index++;
  }

  // Finilse clustering
  this->light_clustering.finaliseClusters();
}

const std::vector<GLuint>& ClusteredLightManager::getSummedIndicesData() const {
  return this->summed_indices;
}

GLuint ClusteredLightManager::getKIndexMapPointer() const {
  return this->key_sort_compact_shader.getIndexTexture();
}

const std::vector<glm::uvec2>& ClusteredLightManager::getLightClusterData() const {
  return this->light_clustering.cluster_to_light_index_map;
}

const std::vector<GLuint>& ClusteredLightManager::getLightIndexData() const {
  return this->light_clustering.light_index_list;
}


}
}