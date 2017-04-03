#include "pipeline\light-management\hashed\linkless-octree\LinklessOctree.h"

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <glm/gtc/type_ptr.hpp>
#include <string>

namespace nTiled {
namespace pipeline {
namespace hashed {

// ----------------------------------------------------------------------------
//  Constructor | Destructor
// ----------------------------------------------------------------------------
LinklessOctree::LinklessOctree(
    unsigned int depth,
    unsigned int n_levels,
    double minimum_node_size,
    glm::vec3 origin,
    std::vector<SpatialHashFunction<glm::u8vec2>*>* p_octree_hash_maps,
    std::vector<bool>* p_data_hash_map_exists,
    std::vector<SpatialHashFunction<glm::uvec2>*>* p_data_hash_maps,
    std::vector<GLuint>* p_light_indices) : 
    depth(depth),
    n_levels(n_levels),
    minimum_node_size(minimum_node_size),
    origin(origin),
    p_octree_hash_maps(p_octree_hash_maps),
    p_data_hash_map_exists(p_data_hash_map_exists),
    p_data_hash_maps(p_data_hash_maps),
    p_light_indices(p_light_indices) {
  // construct arrays of openGL data
  this->p_octree_hash_maps_data_opengl = new GLubyte*[this->getNLevels()];
  this->p_octree_hash_maps_offset_opengl = new GLubyte*[this->getNLevels()];

  this->p_data_hash_maps_data_opengl = new GLuint*[this->getNLevels()];
  this->p_data_hash_maps_offset_opengl = new GLubyte*[this->getNLevels()];

  // copy data into openGL arrays
  unsigned int data_index;

  for (unsigned int i = 0; i < n_levels; i++) {
    // ------------------------------------------------------------------------
    //  p_octree_hash_maps_data_opengl
    data_index = 0;

    this->p_octree_hash_maps_data_opengl[i] =
      new GLubyte[this->p_octree_hash_maps->at(i)->getHashTable().size() * 2];

    for (glm::u8vec2 val : this->p_octree_hash_maps->at(i)->getHashTable()) {
      this->p_octree_hash_maps_data_opengl[i][data_index++] = val.x;
      this->p_octree_hash_maps_data_opengl[i][data_index++] = val.y;
    }

    // ------------------------------------------------------------------------
    //  p_octree_hash_maps_offset_opengl
    data_index = 0;

    this->p_octree_hash_maps_offset_opengl[i] =
      new GLubyte[this->p_octree_hash_maps->at(i)->getOffsetTable().size() * 3];

    for (glm::u8vec3 val : this->p_octree_hash_maps->at(i)->getOffsetTable()) {
      this->p_octree_hash_maps_offset_opengl[i][data_index++] = val.x;
      this->p_octree_hash_maps_offset_opengl[i][data_index++] = val.y;
      this->p_octree_hash_maps_offset_opengl[i][data_index++] = val.z;
    }

    if (this->p_data_hash_map_exists->at(i)) {
      // ----------------------------------------------------------------------
      // p_data_hash_maps_data_opengl
      data_index = 0;

      this->p_data_hash_maps_data_opengl[i] =
        new GLuint[this->p_data_hash_maps->at(i)->getHashTable().size() * 2];

      for (glm::uvec2 val : this->p_data_hash_maps->at(i)->getHashTable()) {
        this->p_data_hash_maps_data_opengl[i][data_index++] = val.x;
        this->p_data_hash_maps_data_opengl[i][data_index++] = val.y;
      }

      // ----------------------------------------------------------------------
      // p_data_hash_maps_offset_opengl
      data_index = 0;

      this->p_data_hash_maps_offset_opengl[i] =
        new GLubyte[this->p_data_hash_maps->at(i)->getOffsetTable().size() * 3];

      for (glm::u8vec3 val : this->p_data_hash_maps->at(i)->getOffsetTable()) {
        this->p_data_hash_maps_offset_opengl[i][data_index++] = val.x;
        this->p_data_hash_maps_offset_opengl[i][data_index++] = val.y;
        this->p_data_hash_maps_offset_opengl[i][data_index++] = val.z;
      }
    }
  }

}


LinklessOctree::~LinklessOctree() {
  // Remove openGL arrays
  // --------------------------------------------------------------------------
  for (unsigned int i = 0; i < this->getNLevels(); ++i) {
    delete[] this->p_octree_hash_maps_data_opengl[i];
    delete[] this->p_octree_hash_maps_offset_opengl[i];

    if (this->p_data_hash_map_exists->at(i)) {
      delete[] this->p_data_hash_maps_data_opengl[i];
      delete[] this->p_data_hash_maps_offset_opengl[i];
    }
  }

  delete[] this->p_octree_hash_maps_data_opengl;
  delete[] this->p_octree_hash_maps_offset_opengl;

  delete[] this->p_data_hash_maps_data_opengl;
  delete[] this->p_data_hash_maps_offset_opengl;

  //  Remove CPU datastructures
  // --------------------------------------------------------------------------
  for (SpatialHashFunction<glm::u8vec2>* p_hfunc : *p_octree_hash_maps)
    delete p_hfunc;
  delete p_octree_hash_maps;

  for (unsigned int i = 0; i < this->p_data_hash_map_exists->size(); ++i)
    if (p_data_hash_map_exists->at(i))
      delete this->p_data_hash_maps->at(i);

  delete this->p_data_hash_map_exists;
  delete this->p_data_hash_maps;

  delete this->p_light_indices;

}


// ----------------------------------------------------------------------------
//  Get methods
// ----------------------------------------------------------------------------
std::vector<GLuint> LinklessOctree::retrieveLights(glm::vec3 point) const {
  glm::vec3 orig = this->getOrigin();
  double width = this->getWidth();
  if (point.x < orig.x ||
      point.y < orig.y ||
      point.z < orig.z ||
      point.x > orig.x + width ||
      point.y > orig.y + width ||
      point.z > orig.z + width) {
    return std::vector<GLuint>();
  }

  glm::u8vec2 octree_data;
  glm::uvec2 light_data = glm::uvec2(0);

  double next_node_size = ((this->getTotalNNodes() / this->getInitialNNodes())
                           * this->getMinimumNodeSize());

  glm::vec3 p_octree_coord = point - orig;
  unsigned int n_nodes = this->getInitialNNodes();

  glm::uvec3 coord_cur;
  glm::uvec3 coord_next = glm::uvec3(floor(p_octree_coord.x / next_node_size),
                                     floor(p_octree_coord.y / next_node_size),
                                     floor(p_octree_coord.z / next_node_size));

  glm::uvec3 index_dif;
  unsigned int index_int;

  for (unsigned int layer_i = 0; layer_i < this->getNLevels(); ++layer_i) {
    coord_cur = coord_next;
    next_node_size *= 0.5;
    coord_next = glm::uvec3(floor(p_octree_coord.x / next_node_size),
                            floor(p_octree_coord.y / next_node_size),
                            floor(p_octree_coord.z / next_node_size));
    index_dif = coord_next - (coord_cur + coord_cur);
    index_int = index_dif.x + index_dif.y * 2 + index_dif.z * 4;

    octree_data = this->p_octree_hash_maps->at(layer_i)->getData(coord_cur);
    if (math::extractBit(octree_data.x, index_int)) {
      if (math::extractBit(octree_data.y, index_int)) {
        light_data = this->p_data_hash_maps->at(layer_i)->getData(coord_next);
      }
      break;
    }
  }

  std::vector<GLuint> node_indices = {};
  for (unsigned int i = light_data.x; i < light_data.x + light_data.y; ++i) {
    node_indices.push_back(this->p_light_indices->at(i));
  }

  return node_indices;
}


// ----------------------------------------------------------------------------
//  openGL methods
// ----------------------------------------------------------------------------
//  loadSpatialTable
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

// ----------------------------------------------------------------------------
//  loadToShader

void LinklessOctree::loadToShader(GLuint shader) {
  // --------------------------------------------------------------------------
  //  construct Initial openGL values

  unsigned int n_levels = this->getNLevels();

  // generate octree spatial hash functions
  this->ps_gfx_octree_node_tables = new GLuint[n_levels];
  this->ps_gfx_octree_offset_tables = new GLuint[n_levels];

  glGenTextures(n_levels, this->ps_gfx_octree_node_tables);
  glGenTextures(n_levels, this->ps_gfx_octree_offset_tables);

  // generate data spatial hash functions
  this->ps_gfx_data_node_tables = new GLuint[n_levels];
  this->ps_gfx_data_offset_tables = new GLuint[n_levels];

  glGenTextures(n_levels, this->ps_gfx_data_node_tables);
  glGenTextures(n_levels, this->ps_gfx_data_offset_tables);

  // create octree textures per level
  for (unsigned int i = 0; i < n_levels; i++) {
    // ------------------------------------------------------------------------
    // load octree_node_tables[i]
    loadSpatialTable<GLubyte>(0 + 4 * i,
                              this->ps_gfx_octree_node_tables[i],
                              GL_RG8UI,
                              this->p_octree_hash_maps->at(i)->getM(),
                              GL_RG_INTEGER,
                              GL_UNSIGNED_BYTE,
                              this->p_octree_hash_maps_data_opengl[i],
                              shader,
                              "octree_data_tables[" + std::to_string(i) + "]");

    // ------------------------------------------------------------------------
    // load octree_offset_tables[i]
    loadSpatialTable<GLubyte>(1 + 4 * i,
                              this->ps_gfx_octree_offset_tables[i],
                              GL_RGB8UI,
                              this->p_octree_hash_maps->at(i)->getR(),
                              GL_RGB_INTEGER,
                              GL_UNSIGNED_BYTE,
                              this->p_octree_hash_maps_offset_opengl[i],
                              shader,
                              "octree_offset_tables[" + std::to_string(i) + "]");

    // load data nodes
    if (this->p_data_hash_map_exists->at(i)) {
      // ----------------------------------------------------------------------
      // load leaf_node_tables[i]
      loadSpatialTable<GLuint>(2 + 4 * i,
                               this->ps_gfx_data_node_tables[i],
                               GL_RG32UI,
                               this->p_data_hash_maps->at(i)->getM(),
                               GL_RG_INTEGER,
                               GL_UNSIGNED_INT,
                               this->p_data_hash_maps_data_opengl[i],
                               shader,
                               "light_data_tables[" + std::to_string(i) + "]");

      // ----------------------------------------------------------------------
      // Load leaf_offset_tables[i]
      loadSpatialTable<GLubyte>(3 + 4 * i,
                                this->ps_gfx_data_offset_tables[i],
                                GL_RGB8UI,
                                this->p_data_hash_maps->at(i)->getR(),
                                GL_RGB_INTEGER,
                                GL_UNSIGNED_BYTE,
                                this->p_data_hash_maps_offset_opengl[i],
                                shader,
                                "light_offset_tables[" + std::to_string(i) + "]");
    }
  }

  // --------------------------------------------------------------------------
  //  Load light indices
  glGenBuffers(1, &this->p_gfx_light_indices);

  glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->p_gfx_light_indices);
  glBufferData(GL_SHADER_STORAGE_BUFFER,
               sizeof(GLuint) * this->p_light_indices->size(),
               this->p_light_indices->data(),
               GL_DYNAMIC_DRAW); // probably not necessary to dynamic draw

  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, this->p_gfx_light_indices);

  glBindBuffer(GL_UNIFORM_BUFFER, 0);

  // --------------------------------------------------------------------------
  //  Load matrix variables
  GLuint p_node_size_den = glGetUniformLocation(shader, "node_size_den");
  float node_size_den = 1.0f / ((this->getTotalNNodes() / this->getInitialNNodes())
                                * this->getMinimumNodeSize());
  glUniform1f(p_node_size_den, node_size_den);

  GLuint p_octree_origin = glGetUniformLocation(shader, "octree_origin");
  glm::vec3 octree_origin = this->getOrigin();
  glUniform3fv(p_octree_origin, 1, glm::value_ptr(octree_origin));


  GLuint p_octree_width = glGetUniformLocation(shader, "octree_width");
  float octree_width = float(this->getWidth());
  glUniform1f(p_octree_width, octree_width);
}


}
}
}