#pragma once

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <vector>
#include <glm\glm.hpp>

namespace nTiled {
namespace world {

/*!
 * Mesh struct holds all relevant data of a single mesh
 */
struct Mesh {
public:
  /*!
   * Construct a new Mesh object with the given parameters
   * 
   * Param:
   *     vertices (vector<vec4>): the vertex positions in homogeneous 
   *                              coordinates, belonging to this Mesh.
   *     normals (vector<vec3>): normal directions of this Mesh.
   *     uvs (vector<vec3): uvw coordinates of this Mesh.
   *     elements (vector<tvec3<u32>>): the elements making up the triangles
   *                                    of this mesh.
   */
  Mesh(std::vector<glm::vec4> vertices,
       std::vector<glm::vec3> normals,
       std::vector<glm::vec3> uvs,
       std::vector<glm::tvec3<glm::u32>> elements);
  /*!
   * Construct a new empty Mesh object.
   */
  Mesh();

  /*! Vertex locations of this Mesh in modelspace */
  std::vector<glm::vec4> vertices;
  /*! Normal directions of this Mesh in modelspace */
  std::vector<glm::vec3> normals;
  /*! UVW coordinates of this Mesh*/
  std::vector<glm::vec3> uvs;
  /*! Primitive elements of this Mesh */
  std::vector<glm::tvec3<glm::u32>> elements;
};

} // world
} // nTiled