#include "pipeline\light-management\hashed\HashedLightManager.h"

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <glm/gtc/type_ptr.hpp>

#include <rapidjson\writer.h>
#include <rapidjson\stringbuffer.h>
#include <fstream>


// ----------------------------------------------------------------------------
//  nTiled Headers
// ----------------------------------------------------------------------------
#include "pipeline\light-management\hashed\light-octree\LightOctreeBuilder.h"
#include "pipeline\light-management\hashed\light-octree\LightOctreeExporter.h"

#include "pipeline\light-management\hashed\linkless-octree\ExportFunctions.h"


namespace nTiled {
namespace pipeline {
namespace hashed {

HashedLightManager::HashedLightManager(const world::World& world,
                                       float minimum_node_size,
                                       unsigned int starting_depth,
                                       float r_increase_ratio,
                                       unsigned int max_attempts) :
  world(world),
  minimum_node_size(minimum_node_size),
  starting_depth(starting_depth),
  r_increase_ratio(r_increase_ratio),
  max_attempts(max_attempts) {
}


HashedLightManager::~HashedLightManager() {
  delete this->p_light_octree;
  delete this->p_linkless_octree;
}


void HashedLightManager::loadToShader(GLuint shader) {
  // load octree origin
  glm::vec4 octree_origin_4 = this->p_light_octree->getOrigin();
  glm::vec3 octree_origin = glm::vec3(octree_origin_4.x,
                                      octree_origin_4.y,
                                      octree_origin_4.z) / octree_origin_4.w;

  GLint p_octree_origin = glGetUniformLocation(shader, "octree_origin");
  glUniform3fv(p_octree_origin, 1, glm::value_ptr(octree_origin));

  // lode node size base den
  GLfloat node_size_base_den = 1.0f / ((1 << (this->p_light_octree->getDepth() - 1 - this->starting_depth)) * this->minimum_node_size);

  GLint p_node_size_base_den = glGetUniformLocation(shader,
                                                    "node_size_base_den");
  glUniform1f(p_node_size_base_den, node_size_base_den);

  // Construct LightIndex buffer
  // ---------------------------
  GLuint ssbo_handles[1];

  glGenBuffers(1, ssbo_handles);
  this->light_index_buffer = ssbo_handles[0];

  glBindBuffer(GL_SHADER_STORAGE_BUFFER, light_index_buffer);
  glBufferData(GL_SHADER_STORAGE_BUFFER,
               sizeof(GLuint) * this->light_index_list.size(),
               this->light_index_list.data(),
               GL_DYNAMIC_DRAW);

  // Link LightIndex buffer
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, light_index_buffer);

  // remove link
  glBindBuffer(GL_UNIFORM_BUFFER, 0);


  this->p_linkless_octree->loadToShader(shader);

}


void HashedLightManager::updateOctreeOrigin(GLuint shader, const glm::mat4& lookAt) {
  GLint p_octree_origin = glGetUniformLocation(shader, "octree_origin");
  glm::vec4 octree_origin = (this->p_light_octree->getOrigin());
  //glm::vec4 octree_origin_camera = lookAt * octree_origin;
  glm::vec3 octree_origin_3 = glm::vec3(octree_origin.x,
                                        octree_origin.y,
                                        octree_origin.z) / octree_origin.w;


  glUniform3fv(p_octree_origin, 1, glm::value_ptr(octree_origin_3));
}


void HashedLightManager::constructLightOctree() {
  LightOctreeBuilder builder = LightOctreeBuilder();
  this->p_light_octree = builder.buildLightOctree(this->world.p_lights,
                                                  this->minimum_node_size);
  // Current data node
  unsigned short current = 0;

  // Construction data array
  // nodes[current] contains data this layer
  // nodes[(current + 1) & 1] contains data next layer
  std::vector<std::pair<glm::uvec3, lo::Node*>> nodes[2] = {
    this->p_light_octree->getNodesAtDepth(this->starting_depth),
    std::vector<std::pair<glm::uvec3, lo::Node*>>() };

  std::string path_initial_nodes = "C:/Users/Monthy/Documents/projects/thesis/hierarchical-shading/test/initial_nodes.json";
  exportInitialLayers(path_initial_nodes, nodes[0]);

  // Hash Node data single layer
  std::vector<std::pair<glm::uvec3, glm::u8vec2>> hash_nodes =
    std::vector<std::pair<glm::uvec3, glm::u8vec2>>();

  // Leaf Node data single layer
  std::vector<std::pair<glm::uvec3, glm::uvec2>> leaf_nodes =
    std::vector<std::pair<glm::uvec3, glm::uvec2>>();

  // Light index list, containing indices to lights
  this->light_index_list.clear();

  // Vector pointer to if leaf data hash function exists
  std::vector<SpatialHashFunction<glm::u8vec2>*>* p_node_hash_functions =
    new std::vector<SpatialHashFunction<glm::u8vec2>*>();

  // Vector pointer to leaf data hash maps
  std::vector<SpatialHashFunction<glm::uvec2>*>* p_leaf_hash_functions =
    new std::vector<SpatialHashFunction<glm::uvec2>*>();

  std::vector<bool>* p_has_leaf_hash_functions =
    new std::vector<bool>();

  while (!nodes[current].empty()) {
    // get data of this level
    for (std::pair<glm::uvec3, lo::Node*> node : nodes[current]) {
      node.second->extractData(node.first,
                               hash_nodes,
                               leaf_nodes,
                               this->light_index_list,
                               nodes[(current + 1) & 1]);
    }
    // construct spatial functions of this map
    p_node_hash_functions->push_back(new SpatialHashFunction<glm::u8vec2>(hash_nodes,
                                                                          this->max_attempts,
                                                                          this->r_increase_ratio));

    if (leaf_nodes.empty()) {
      p_has_leaf_hash_functions->push_back(false);
      p_leaf_hash_functions->push_back(nullptr);
    } else {
      p_has_leaf_hash_functions->push_back(true);
      p_leaf_hash_functions->push_back(new SpatialHashFunction<glm::uvec2>(leaf_nodes,
                                                                           this->max_attempts,
                                                                           this->r_increase_ratio));
    }
    nodes[current].clear();
    hash_nodes.clear();
    leaf_nodes.clear();

    current = (current + 1) & 1;
  }

  // check if the depth values are right
  //unsigned int n_levels = this->p_light_octree->getDepth() - this->starting_depth;
  unsigned int initial_nodes_n = 1 << (this->starting_depth + 1);

  this->p_linkless_octree = new LinklessOctree(this->p_light_octree->getMinimumLeafNodeSize(),
                                               this->starting_depth,
                                               initial_nodes_n,
                                               this->p_light_octree->getOrigin(),
                                               p_leaf_hash_functions,
                                               p_has_leaf_hash_functions,
                                               p_node_hash_functions);
}


void HashedLightManager::exportToJson(const std::string& path_lights,
                                      const std::string& path_light_octree,
                                      const std::string& path_linkless_octree,
                                      const std::string& path_light_indices) const {
  // export lights to path_lights
  exportLights(path_lights, this->world.p_lights);
  // export light octree to path_light_octree
  exportLightOctree(path_light_octree, *this->p_light_octree);
  // export linkless light octree to path_linkless_octree
  this->p_linkless_octree->exportToJson(path_linkless_octree);

  // export light indices
  rapidjson::StringBuffer s;
  rapidjson::Writer<rapidjson::StringBuffer> writer(s);

  writer.StartObject();
  writer.Key("light-indices");
  writer.StartArray();
  for (GLuint val : this->light_index_list) {
    writer.Uint(val);
  }
  writer.EndArray();
  writer.EndObject();

  std::ofstream output_stream;
  output_stream.open(path_light_indices);
  output_stream << s.GetString();
  output_stream.close();
}


HashedLightManagerBuilder::HashedLightManagerBuilder() {}

HashedLightManager* HashedLightManagerBuilder::constructNewHashedLightManager(
  const world::World& world,
  float minimum_node_size,
  unsigned int starting_depth,
  float r_increase_ratio,
  unsigned int max_attempts) const {
  return new HashedLightManager(world,
                                minimum_node_size,
                                starting_depth,
                                r_increase_ratio,
                                max_attempts);
}


}
}
}
