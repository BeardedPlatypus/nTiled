#pragma once

// ---------------------------------------------------------------------------
//  Libraries
// ---------------------------------------------------------------------------
#include <vector>
#include <glad\glad.h>
#include <glm\glm.hpp>
#include <random>

namespace nTiled {
namespace pipeline {
namespace hashed {

/*! @brief SpatialHashFunction<R> describes a single 3d hash function holding 
 *         data of type R. This implementation is based on the paper
 *         Perfect Spatial Hashing.
 * 
 *  The hash function is build with both a hashtable H and an offset table
 *  Phi. Hash table H contains data of type R, while the offset table is
 *  defined as elements of glm::u8vec3.
 * 
 *  The spatial Hash function is defined as:
 *    h(p) = h_0(p) + Phi(h_1(p))
 */
template <class R>
class SpatialHashFunction {
  /*! @brief An entry element describes a single element within the 
   *         The spatial hash function. It contains the actual point, 
   *         The hash_0 and the data associated with the given point
   */
  struct EntryElement {
    /*! @brief construct a new EntryElement with the specified 
     *         parameters.
     * 
     * @param point a point within the spatial data structure with which 
     *              this EntryElement.data is associated.
     * @param hash_0 the hash_0 value associated with the point.
     * @param data R data assoiated with the point of this EntryElement.
     */
    EntryElement(glm::uvec3 point, glm::uvec3 hash_0, R data);

    /*! @brief The point with which this EntryElement is associated. */
    glm::uvec3 point;
    /*! @brief The h_0(p) value that is associated with this point. */
    glm::uvec3 hash_0;
    /*! @brief The data of type R associated with this point. */
    R data;
  };

  /*! @brief A ConstructionElement describes all data associated with
   *         an single hash_1 value. It contains the hash_1 value and
   *         all associated elements.
   */
  struct ConstructionElement {
    /*! @brief Construct a new empty ConstructionElement.
     */
    ConstructionElement();

    /*! @brief Construct a new empty ConstructionElement of hash_1
     * 
     * @param hash_1 The hash_1 value associated with this new 
     *               ConstructionElement
     */
    ConstructionElement(glm::uvec3 hash_1);

    /*! @brief The hash_1 value of this ConstructionElement*/
    glm::uvec3 hash_1;
    /*! @brief A vector containing all entry elements associated with this 
     *         hash_1 value.
     */
    std::vector<EntryElement> elements;
  };

  /*! @brief Struct to compare two ConstructionElement based on the size of
   *         their elements. Returns the greater one.
   */
  struct ConstructionElementCompare {
    bool operator() (ConstructionElement i, ConstructionElement j) { return (i.elements.size() > j.elements.size()); }
  } compare_struct;

public:
  // --------------------------------------------------------------------------
  //  Constructor | Destructor
  /*! @brief Construct a new SpatialHashFunction containig the given entries
   *
   * @param entries A vector containing the a set of points and associated data
   * @param max_attempts The maximum number attempts a hash function should be
   *                     tried to be build.
   * @param ratio The ratio with which the offset table is increased, each time
   *              a build attempt fails.
   * @param gen a std::mt19937 generator to build the random values used during
   *            construction.
   * @param distribution A uniform integer distribution used to build the random
   *                     values used during construction.
   */
  SpatialHashFunction(const std::vector<std::pair<glm::uvec3, R>>& entries,
                      unsigned int max_attempts,
                      float ratio,
                      std::mt19937& gen,
                      std::uniform_int_distribution<unsigned short>& distribution);
  /*! @brief Destruct this SpatialHashFunction
   */
  ~SpatialHashFunction();

  // --------------------------------------------------------------------------
  //  Getters

  /*! @brief Get the hash table of this SpatialHashFunction
   *  
   * @return The hash table of this SpatialHashFunction
   */
  const std::vector<R>& getHashTable() const { return *(this->p_hash_table); }

  /*! @brief Get the offset table of this sPatialHashfunction
   *
   * @return The offset table of this SpatialHashFunction
   */
  const std::vector<glm::u8vec3>& getOffsetTable() const { return *(this->p_offset_table); }

  /*! @Brief Get the size m in one dimension of this SpatialHashFunction's 
   *         HashTable
   *
   * @return The size m in one dimension of this SpatialHashFunction's HashTable
   */
  inline unsigned int getM() const { return this->m_dim; }

  /*! @brief Get the size r in one dimension of this SpatialHashFunction's 
   *         offset table
   *
   * @return The size r in one dimension of this SpatialHashFunction's
   *         offset table.
   */
  inline unsigned int getR() const { return this->r_dim; }

private:
  // -------------------------------------------------------------------------
  //  buildTable
  /*! @brief Build the tables associated with this SpatialHashFunction 
   *         given the specified parameters.
   * 
   * @param m The size of a single dimension of H of this SpatialhashFunction
   * @param r The size of a single dimension of Phi of this SpatialHashFunction
   * @param entries The entries which should be stored in H
   * @param gen The mt19937 generator used to build random values during the 
   *            construction
   * @param distribution a uniform integer distribution to build the random values
   *        during the construction.
   * 
   * @returns True if building of tables succeeded, false otherwise.
   *
   * @post (new this)->m == m
   * @post (new this)->r == r
   */
  bool buildTables(unsigned int m,
                   unsigned int r,
                   std::vector<std::pair<glm::uvec3, R>> entries,
                   std::mt19937& gen,
                   std::uniform_int_distribution<unsigned short>& distribution);

  // --------------------------------------------------------------------------
  //  supporting functions
  /*! @brief Calculate whether m and r are acceptable parameters for the 
   *         construction of this SpatialHashFunction.
   *
   * @param m The size of a single dimension of H
   * @param r The size of a single dimension of Phi
   *
   * @returns True if the parameters are acceptable, false otherwise.
   */
  bool isAcceptableParameters(unsigned int m, unsigned int r);

  /*! @brief Convert point p to an index within a single array given 
   *          dimension dim.
   * 
   * @param p A 3d point which should be converted to a single 
   *          index within an array.
   * @param dim The dimension in which the index resides.
   * 
   * @returns The index within the one dimensional array that corresponds with
   *          p
   */
  inline unsigned int toIndex(glm::uvec3 p, unsigned int dim) {
    return p.x + p.y * dim + p.z * dim * dim;
  }

  /*! @brief Retrieve the neighbouring values within the offset table Phi
   *        around point p and store them in in candidate_vector.
   * 
   * @param p The point around which the values should be retrieved.
   * @param dim The size of the offset table Phi in a single dimension.
   * @param offset_def A table describing whether a corresponding value in
   *                   in offset_val is defined.
   * @param offset_val The current offset_table Phi
   * @param candidate_vector The output vector in which the candidates are 
   *                         stored.
   */
  void retrieveCandidates(glm::uvec3 p,
                          unsigned int dim,
                          const std::vector<bool>& offset_def,
                          const std::vector<glm::u8vec3>& offset_val,
                          std::vector<glm::u8vec3>& candidate_vector);

  /*! @brief Check whether the proposed candidate is a valid candidate, 
   *         given the specified parameters.
   * 
   * @param candidate The ccandidate offset value.
   * @param m The size of the dimension of the hash table H
   * @param elements a vector containing all the elements associated with 
   *                 a single entry in the offset table Phi.
   * @param hash_table_def A table describing whether a corresponding position
   *                       in hash table H is already defined.
   */
  bool isValidCandidate(glm::u8vec3 candidate,
                        unsigned int m,
                        const std::vector<EntryElement>& elements,
                        const std::vector<bool>& hash_table_def);

  // --------------------------------------------------------------------------
  //  properties

  // Hash table H
  // ------------
  /*! @biref The size m of the hash table in one dimension. */
  unsigned int m_dim;
  /*! @brief Pointer to the hash table of this SpatialHashFunction. */
  std::vector<R>* p_hash_table;

  // Offset table Phi
  // ----------------
  /*! @brief The size r of the offset table in one dimension. */
  unsigned int r_dim;
  /*! @brief Pointer to the offset table of this SpatialHashFunction. */
  std::vector<glm::u8vec3>* p_offset_table;
};

}
}
}
