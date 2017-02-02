/*! @file Mesh.h
 *  @brief Mesh.h contains the definition of the Mesh struct.
 */
#pragma once

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <vector>
#include <glm\glm.hpp>

namespace nTiled {
namespace world {

/*! @brief Mesh struct holds all relevant data of a single mesh
 */
struct Mesh {
public:
  /*! @brief Construct a new Mesh object with the given parameters
   * 
   * @param vertices The vertex positions in homogeneous coordinates, belonging
   *                 to this Mesh.
   * @param normals Normal directions of this Mesh.
   * @param uvs UVW coordinates of this Mesh.
   * @param elements the elements making up the triangles of this mesh.
   */
  Mesh(std::vector<glm::vec4> vertices,
       std::vector<glm::vec3> normals,
       std::vector<glm::vec3> uvs,
       std::vector<glm::tvec3<glm::u32>> elements);
  /*! @brief Construct a new empty Mesh object.
   */
  Mesh();

  /*! @brief Vertex locations of this Mesh in modelspace */
  std::vector<glm::vec4> vertices;
  /*! @brief Normal directions of this Mesh in modelspace */
  std::vector<glm::vec3> normals;
  /*! @brief UVW coordinates of this Mesh*/
  std::vector<glm::vec3> uvs;
  /*! @brief Primitive elements of this Mesh */
  std::vector<glm::tvec3<glm::u32>> elements;
};

} // world
} // nTiled