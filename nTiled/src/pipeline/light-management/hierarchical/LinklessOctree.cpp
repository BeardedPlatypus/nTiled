#include "pipeline\light-management\hierarchical\LinklessOctree.h"

#include <cmath>

namespace nTiled {
namespace pipeline {
namespace hierarchical {

LinklessOctree::LinklessOctree(
  std::vector<SpatialHashFunction<glm::uvec2>*>* leaf_hash_maps,
  std::vector<bool>* has_leaf_hash_map,
  std::vector<SpatialHashFunction<glm::u8vec2>*>* node_hash_maps) :
    leaf_hash_maps(leaf_hash_maps),
    has_leaf_hash_map(has_leaf_hash_map),
    node_hash_maps(node_hash_maps) {
  // Copy spatial hash function data to glsl ready values
  // --------------------------------------------------------------------------
  // depth of this LinklessOctree
  unsigned int depth = this->getDepth();

  // construct node data arrays
  this->octree_hash_tables = new GLushort*[depth];
  this->octree_offset_tables = new GLushort*[depth];

  // construct leaf data arrays
  this->data_hash_tables = new GLuint*[depth];
  this->data_offset_tables = new GLushort*[depth];

  // Copy data into new arrays
  // --------------------------------------------------------------------------
  unsigned int data_index;

  for (unsigned int i = 0; i < depth; i++) {
    // copy spatial hash functions into glsl ready values
    // todo optimise this

    // copy octree data and offset table
    // ------------------------------------------------------------------------
    data_index = 0;
    this->octree_hash_tables[i] = 
      new GLushort[this->node_hash_maps->at(i)->getHashTable().size()];
    for (glm::u8vec2 val : this->node_hash_maps->at(i)->getHashTable()) {
      this->octree_hash_tables[i][data_index++] = val.x;
      this->octree_hash_tables[i][data_index++] = val.y;
    }

    data_index = 0;
    this->octree_offset_tables[i] = 
      new GLushort[this->node_hash_maps->at(i)->getOffsetTable().size()];
    for (GLushort val : this->node_hash_maps->at(i)->getOffsetTable()) {
      this->octree_offset_tables[i][data_index++] = val;
    }

    if (this->has_leaf_hash_map->at(i)) {
      data_index = 0;
      this->data_hash_tables[i] =
        new GLuint[this->leaf_hash_maps->at(i)->getHashTable().size()];
      for (glm::uvec2 val : this->leaf_hash_maps->at(i)->getHashTable()) {
        this->data_hash_tables[i][data_index++] = val.x;
        this->data_hash_tables[i][data_index++] = val.y;
      }

      data_index = 0;
      this->data_offset_tables[i] =
        new GLushort[this->leaf_hash_maps->at(i)->getOffsetTable().size()];
      for (GLushort val : this->leaf_hash_maps->at(i)->getOffsetTable()) {
        this->data_offset_tables[i][data_index++] = val;
      }
    }
  }
}


LinklessOctree::~LinklessOctree() {
  unsigned int depth = this->getDepth();

  for (unsigned int i = 0; i < depth; i++) {
    delete[] this->octree_hash_tables[i];
    delete[] this->octree_offset_tables[i];

    delete[] this->data_hash_tables[i];
    delete[] this->data_offset_tables[i];
  }

  delete[] this->octree_hash_tables;
  delete[] this->octree_offset_tables;
  delete[] this->data_hash_tables;
  delete[] this->data_offset_tables;

  for (SpatialHashFunction<glm::uvec2>* p_h_leaf : *(this->leaf_hash_maps)) {
    delete p_h_leaf;
  }

  for (SpatialHashFunction<glm::u8vec2>* p_h_node : *(this->node_hash_maps)) {
    delete p_h_node;
  }

  delete this->leaf_hash_maps;
  delete this->has_leaf_hash_map;
  delete this->node_hash_maps;

}


void loadSpatialTable(GLenum texture_index,
                      GLuint p_texture,
                      GLint internal_format,
                      GLsizei dimension,
                      GLenum pixel_data_format,
                      GLenum pixel_data_type,
                      const GLvoid * data,
                      GLuint shader,
                      std::string glsl_sampler_name) {
    glActiveTexture(texture_index);
    glBindTexture(GL_TEXTURE_3D, p_texture);

    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);

    glTexImage3D(GL_TEXTURE_3D,     // target
                 0,                 // mipmap level
                 internal_format,          // internal format: 2x8 bits unsigned integer, 2
                                    // bits per node, 8 nodes packed per value
                 dimension,          // width
                 dimension,          // height
                 dimension,          // depth
                 0,                 // border
                 pixel_data_format,     // pixel data format
                 pixel_data_type, // pixel data type
                 data);

  GLuint p_texture_data = glGetUniformLocation(shader, glsl_sampler_name.c_str());
  glUniform1i(p_texture_data,
              texture_index);
}


void LinklessOctree::loadToShader(GLuint shader) {
  unsigned int depth = this->getDepth();

  // generate octree spatial hash functions
  GLuint* ps_octree_node_tables = new GLuint[depth]; // bvec2
  GLuint* ps_octree_offset_tables = new GLuint[depth];

  glGenTextures(depth, ps_octree_node_tables);
  glGenTextures(depth, ps_octree_offset_tables);

  // generate data spatial hash functions
  GLuint* ps_data_node_tables = new GLuint[depth];
  GLuint* ps_data_offset_tables = new GLuint[depth];

  glGenTextures(depth, ps_data_node_tables);
  glGenTextures(depth, ps_data_offset_tables);

  std::string sampler_name;

  // create octree textures per level
  for (unsigned int i = 0; i < depth; i++) {
    // load octree_node_tables[i]
    loadSpatialTable(GL_TEXTURE0 + 4 * i + 0,
                     ps_octree_node_tables[i],
                     GL_RG8UI,
                     this->node_hash_maps->at(i)->getM(),
                     GL_RG_INTEGER,
                     GL_UNSIGNED_SHORT,
                     this->octree_hash_tables[i],
                     shader,
                     "node_hash_tables[" + std::to_string(i) + "]");

    // load octree_offset_tables[i]
    loadSpatialTable(GL_TEXTURE0 + 4 * i + 1,
                     ps_octree_offset_tables[i],
                     GL_R8UI,
                     this->node_hash_maps->at(i)->getR(),
                     GL_RED_INTEGER,
                     GL_UNSIGNED_SHORT,
                     this->octree_offset_tables[i],
                     shader,
                     "node_offset_tables[" + std::to_string(i) + "]");
   
    // load data nodes
    if (this->has_leaf_hash_map->at(i)) {
      // load leaf_node_tables[i]
      loadSpatialTable(GL_TEXTURE0 + 4 * i + 2,
                       ps_data_node_tables[i],
                       GL_RG32UI,
                       this->leaf_hash_maps->at(i)->getM(),
                       GL_RED_INTEGER,
                       GL_UNSIGNED_INT,
                       this->data_hash_tables[i],
                       shader,
                       "leaf_hash_tables[" + std::to_string(i) + "]");

      // load leaf_offset_tables[i]
      loadSpatialTable(GL_TEXTURE0 + 4 * i + 3,
                       ps_data_offset_tables[i],
                       GL_R8UI,
                       this->leaf_hash_maps->at(i)->getR(),
                       GL_RED_INTEGER,
                       GL_UNSIGNED_SHORT,
                       this->data_offset_tables[i],
                       shader,
                       "leaf_offset_tables[" + std::to_string(i) + "]");
    }
  }
}


}
}
}
