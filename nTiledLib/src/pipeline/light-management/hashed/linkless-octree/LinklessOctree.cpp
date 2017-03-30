#include "pipeline\light-management\hashed\linkless-octree\LinklessOctree.h"

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <cmath>

#include <rapidjson\writer.h>
#include <rapidjson\stringbuffer.h>
#include <fstream>

#include <iostream>

namespace nTiled {
namespace pipeline {
namespace hashed {

LinklessOctree::LinklessOctree(
  float minimum_node_size,
  unsigned int starting_depth,
  unsigned int initial_n_nodes_dim,
  glm::vec4 octree_origin,
  std::vector<SpatialHashFunction<glm::uvec2>*>* leaf_hash_maps,
  std::vector<bool>* has_leaf_hash_map,
  std::vector<SpatialHashFunction<glm::u8vec2>*>* node_hash_maps) :
  minimum_node_size(minimum_node_size),
  starting_depth(starting_depth),
  initial_n_nodes_dim(initial_n_nodes_dim),
  octree_origin(octree_origin),
  leaf_hash_maps(leaf_hash_maps),
  has_leaf_hash_map(has_leaf_hash_map),
  node_hash_maps(node_hash_maps) {
  // Copy spatial hash function data to glsl ready values
  // --------------------------------------------------------------------------
  // depth of this LinklessOctree
  unsigned int n_levels = this->getNLevels();

  // construct node data arrays
  this->octree_hash_tables = new GLubyte*[n_levels];
  this->octree_offset_tables = new GLubyte*[n_levels];

  // construct leaf data arrays
  this->data_hash_tables = new GLuint*[n_levels];
  this->data_offset_tables = new GLubyte*[n_levels];

  // Copy data into new arrays
  // --------------------------------------------------------------------------
  unsigned int data_index;

  for (unsigned int i = 0; i < n_levels; i++) {
    // copy spatial hash functions into glsl ready values
    // todo optimise this

    // copy octree data and offset table
    // ------------------------------------------------------------------------
    data_index = 0;
    this->octree_hash_tables[i] =
      new GLubyte[this->node_hash_maps->at(i)->getHashTable().size() * 2];
    for (glm::u8vec2 val : this->node_hash_maps->at(i)->getHashTable()) {
      this->octree_hash_tables[i][data_index++] = val.x;
      this->octree_hash_tables[i][data_index++] = val.y;
    }

    data_index = 0;
    this->octree_offset_tables[i] =
      new GLubyte[this->node_hash_maps->at(i)->getOffsetTable().size() * 3];

    for (glm::u8vec3 val  : this->node_hash_maps->at(i)->getOffsetTable()) {
      this->octree_offset_tables[i][data_index++] = val.x;
      this->octree_offset_tables[i][data_index++] = val.y;
      this->octree_offset_tables[i][data_index++] = val.z;
    }

    if (this->has_leaf_hash_map->at(i)) {
      data_index = 0;
      this->data_hash_tables[i] =
        new GLuint[this->leaf_hash_maps->at(i)->getHashTable().size() * 2];
      for (glm::uvec2 val : this->leaf_hash_maps->at(i)->getHashTable()) {
        this->data_hash_tables[i][data_index++] = val.x;
        this->data_hash_tables[i][data_index++] = val.y;
      }

      data_index = 0;
      this->data_offset_tables[i] =
        new GLubyte[this->leaf_hash_maps->at(i)->getOffsetTable().size() * 3];
      for (glm::u8vec3 val : this->leaf_hash_maps->at(i)->getOffsetTable()) {
        this->data_offset_tables[i][data_index++] = val.x;
        this->data_offset_tables[i][data_index++] = val.y;
        this->data_offset_tables[i][data_index++] = val.z;
      }
    }
  }
}


LinklessOctree::~LinklessOctree() {
  unsigned int n_levels = this->getNLevels();

  for (unsigned int i = 0; i < n_levels; i++) {
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


template <class R>
void loadSpatialTable(GLuint index,
                      GLuint p_texture,
                      GLint internal_format,
                      GLsizei dimension,
                      GLenum pixel_data_format,
                      GLenum pixel_data_type,
                      R* data,//const GLvoid * data,
                      GLuint shader,
                      std::string glsl_sampler_name) {

  GLuint p_texture_data = glGetUniformLocation(shader, glsl_sampler_name.c_str());
  glUniform1i(p_texture_data, index);

  glActiveTexture(GL_TEXTURE0 + index);
  glBindTexture(GL_TEXTURE_3D, p_texture);

  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);

  glTexImage3D(GL_TEXTURE_3D,     // target
               0,                 // mipmap level
               internal_format,   // internal format: 2x8 bits unsigned integer, 2
                                  // bits per node, 8 nodes packed per value
               dimension,         // width
               dimension,         // height
               dimension,         // depth
               0,                 // border
               pixel_data_format, // pixel data format
               pixel_data_type,   // pixel data type
               data);

}


template void loadSpatialTable(GLuint,       // index
                               GLuint,       // p_texture
                               GLint,        // internal_format
                               GLsizei,      // dimension
                               GLenum,       // pixel_data_format
                               GLenum,       // pixel_data_type
                               GLubyte*,     // data
                               GLuint,       // shader
                               std::string); // sampler name

template void loadSpatialTable(GLuint,       // index
                               GLuint,       // p_texture
                               GLint,        // internal_format
                               GLsizei,      // dimension
                               GLenum,       // pixel_data_format
                               GLenum,       // pixel_data_type
                               GLuint*,      // data
                               GLuint,       // shader
                               std::string); // sampler name

void LinklessOctree::loadToShader(GLuint shader) {
  /*
  unsigned int n_levels = this->getNLevels();

  // generate octree spatial hash functions
  GLuint* ps_octree_node_tables = new GLuint[n_levels];
  GLuint* ps_octree_offset_tables = new GLuint[n_levels];

  glGenTextures(n_levels, ps_octree_node_tables);
  glGenTextures(n_levels, ps_octree_offset_tables);

  // generate data spatial hash functions
  GLuint* ps_data_node_tables = new GLuint[n_levels];
  GLuint* ps_data_offset_tables = new GLuint[n_levels];

  glGenTextures(n_levels, ps_data_node_tables);
  glGenTextures(n_levels, ps_data_offset_tables);

  // create octree textures per level
  for (unsigned int i = 0; i < n_levels; i++) {
    // load octree_node_tables[i]
    loadSpatialTable(0 + 4 * i,
                     ps_octree_node_tables[i],
                     GL_RG8UI,
                     this->node_hash_maps->at(i)->getM(),
                     GL_RG_INTEGER,
                     GL_UNSIGNED_BYTE,
                     this->octree_hash_tables[i],
                     shader,
                     "node_hash_tables[" + std::to_string(i) + "]");
    // load octree_offset_tables[i]
    loadSpatialTable(1 + 4 * i,
                     ps_octree_offset_tables[i],
                     GL_RGB8UI,
                     this->node_hash_maps->at(i)->getR(),
                     GL_RGB_INTEGER,
                     GL_UNSIGNED_BYTE,
                     this->octree_offset_tables[i],
                     shader,
                     "node_offset_tables[" + std::to_string(i) + "]");

    // load data nodes
    if (this->has_leaf_hash_map->at(i)) {
      // load leaf_node_tables[i]
      loadSpatialTable(2 + 4 * i,
                       ps_data_node_tables[i],
                       GL_RG32UI,
                       this->leaf_hash_maps->at(i)->getM(),
                       GL_RG_INTEGER,
                       GL_UNSIGNED_INT,
                       this->data_hash_tables[i],
                       shader,
                       "leaf_hash_tables[" + std::to_string(i) + "]");

      // load leaf_offset_tables[i]
      loadSpatialTable(3 + 4 * i,
                       ps_data_offset_tables[i],
                       GL_RGB8UI,
                       this->leaf_hash_maps->at(i)->getR(),
                       GL_RGB_INTEGER,
                       GL_UNSIGNED_BYTE,
                       this->data_offset_tables[i],
                       shader,
                       "leaf_offset_tables[" + std::to_string(i) + "]");
    }
  } */
}


void LinklessOctree::exportToJson(const std::string& path) const {
  rapidjson::StringBuffer s;
  rapidjson::Writer<rapidjson::StringBuffer> writer(s);

  unsigned int n_levels = this->getNLevels();

  writer.StartObject();

  writer.Key("initial_node_size");
  writer.Double(this->getInitialNodeSize());

  writer.Key("initial_n_nodes");
  writer.Uint(this->getInitialNNodesDim());

  writer.Key("starting_depth");
  writer.Uint(this->starting_depth);

  writer.Key("n_levels");
  writer.Uint(n_levels);

  writer.Key("octree_origin");
  writer.StartObject();
  writer.Key("x");
  writer.Double(this->getOrigin().x / this->getOrigin().w);
  writer.Key("y");
  writer.Double(this->getOrigin().y / this->getOrigin().w);
  writer.Key("z");
  writer.Double(this->getOrigin().z / this->getOrigin().w);
  writer.EndObject();

  writer.Key("octree_layers");
  writer.StartArray();

  SpatialHashFunction<glm::u8vec2>* octree_func;
  SpatialHashFunction<glm::uvec2>* data_func;

  unsigned int m;
  unsigned int r;

  for (unsigned int i = 0; i < n_levels; i++) {
    octree_func = this->node_hash_maps->at(i);
    data_func = this->leaf_hash_maps->at(i);

    writer.StartObject();
    writer.Key("level_i");
    writer.Uint(i);

    // Octree spatial hash function
    writer.Key("octree");
    writer.StartObject();
    m = octree_func->getM();
    r = octree_func->getR();

    writer.Key("m");
    writer.Uint(m);

    writer.Key("r");
    writer.Uint(r);

    writer.Key("octree_table");
    writer.StartArray();
    for (unsigned int k = 0; k < m * m * m * 2; k++) {
      writer.Uint(this->octree_hash_tables[i][k]);
    }
    writer.EndArray();

    writer.Key("offset_table");
    writer.StartArray();
    for (glm::u8vec3 val : octree_func->getOffsetTable()) {
      writer.StartObject();
        writer.Key("x");
        writer.Uint(val.x);
        writer.Key("y");
        writer.Uint(val.y);
        writer.Key("z");
        writer.Uint(val.z);
      writer.EndObject();
    }
    writer.EndArray();
    writer.EndObject();

    // Data spatial hash function
    writer.Key("data");
    writer.StartObject();
    writer.Key("has_data");
    writer.Bool(this->has_leaf_hash_map->at(i));

    if (this->has_leaf_hash_map->at(i)) {
      m = data_func->getM();
      r = data_func->getR();

      writer.Key("m");
      writer.Uint(m);

      writer.Key("r");
      writer.Uint(r);

      writer.Key("data_table");
      writer.StartArray();
      for (glm::uvec2 val : data_func->getHashTable()) {
        writer.StartObject();
          writer.Key("offset");
          writer.Uint(val.x);
          writer.Key("size");
          writer.Uint(val.y);
        writer.EndObject();
      }
      writer.EndArray();

      writer.Key("offset_table");
      writer.StartArray();
      for (glm::u8vec3 val : data_func->getOffsetTable()) {
        writer.StartObject();
        writer.Key("x");
        writer.Uint(val.x);
        writer.Key("y");
        writer.Uint(val.y);
        writer.Key("z");
        writer.Uint(val.z);
      writer.EndObject();
      }
      writer.EndArray();
    }
    writer.EndObject();
    writer.EndObject();
  }

  writer.EndArray();
  writer.EndObject();

  std::ofstream output_stream;
  output_stream.open(path);
  output_stream << s.GetString();
  output_stream.close();
}

}
}
}
