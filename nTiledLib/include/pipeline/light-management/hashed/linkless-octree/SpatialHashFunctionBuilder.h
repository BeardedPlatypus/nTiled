#pragma once

// ---------------------------------------------------------------------------
//  Libraries
// ---------------------------------------------------------------------------
#include <vector>
#include <glad\glad.h>
#include <glm\glm.hpp>
#include <random>


// ---------------------------------------------------------------------------
//  nTiled headers
// ---------------------------------------------------------------------------
#include "SpatialHashFunction.h"
#include "Table.h"


namespace nTiled {
namespace pipeline {
namespace hashed {

/*! @brief The SpatialHashFunctionBuilder is responsible for constructing 
 *         new SpatialHashFunctions. 
 */
template <class R>
class SpatialHashFunctionBuilder {
public:

  // ----------------------------------------------------------------------------
  //  Supporting construction structs
  // ----------------------------------------------------------------------------
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
    bool operator() (const ConstructionElement& i, const ConstructionElement& j) { return (i.elements.size() > j.elements.size()); }
  };
    // --------------------------------------------------------------------------
    //  Constructor
    // --------------------------------------------------------------------------
    /*! @brief Construct a new SpatialHashFunctionBuilder
     * 
     * @returns A new SpatialHashFunctionBuilder
     */
    SpatialHashFunctionBuilder();

    /*! @brief Construct a new SpatialHashFunctionBuilder using the given seed
     *
     * @param seed The seed being used in the random number generator utilised
     *             this new SpatialHashFunctionBuilder
     * 
     * @returns A new SpatialHashFunctionBuilder
     */
    SpatialHashFunctionBuilder(unsigned int seed);
  
    // --------------------------------------------------------------------------
    //  SpatialHashFunction construction method
    // --------------------------------------------------------------------------
    /*! @brief Construct a new Spatialhashfunction containing the given entries.
     *         Return the pointer to this new SpatialDataHashFunction.
     *
     * @param entries A vector containing the set of ppoints and associated data
     * @param max_attempts The maximum number of attempts to be tried building 
     *                     the new SpatialhashFunction.
     * @param ratio The ratio with which the size of the offset table Phi is 
     *              increased, each time a build attempt fails.
     *              
     * @returns A pointer to the newly constructed SpatialHashFunction containing
     *          all given entries, and no other data elements.
     *          | FORALL e IN entries: (new this)->obtainData(e.first) == e.second
     */
  SpatialHashFunction<R>* constructHashFunction(
    const std::vector<std::pair<glm::uvec3, R>>& entries,
    unsigned int max_attempts,
    float ratio);

  // --------------------------------------------------------------------------
  //  Construction supporting methods
  // --------------------------------------------------------------------------

  /*! @brief Build the hash table H and offset table Phi of a new 
   *         SpatialHashFunction and return these as a pair of pointers.
   * 
   * @param m_dim The size of a single dimension of hash table H
   * @param r_dim The size of a single dimension of offset table Phi
   * @param entries The entries which should be stored in H
   *
   * @returns a pair of vectors describing the hash table H and offset table 
   *          Phi
   *          | H->size() == m_dim * m_dim * m_dim
   *          | Phi->size() == r_dim * r_dim * r_dim
   *          | FORALL e IN entries : H[h(e.first)] == e.second
   */
  bool buildTables(const std::vector<ConstructionElement>& entry_vector,
                   Table<R>& hash_table,
                   Table<glm::u8vec3>& offset_table);

  /*! @brief map entries to result_entry_set, preparing the data to be used in
   *         build tables.
   *
   * @param entries The entry set from which result_entry_set is constructed
   * @param m_dim The dimension of the hashtable H
   * @param r_dim The dimension of the offset table Phi
   * @param result_entry_set The resulting vector in which the mapped data is 
   *                         placed
   *
   * @returns True if a perfect hash function can be constructed with the given
   *          entries and dimensions, false otherwise.
   */
  bool mapEntryVector(const std::vector<std::pair<glm::uvec3, R>>& entries,
                      unsigned int m_dim,
                      unsigned int r_dim,
                      std::vector<ConstructionElement>& result_entry_set) const;

  /*! @brief Calculate whether m and r are acceptable parameters for the 
   *         construction of a SpatialHashFunction.
   * 
   * @param m The size of a single dimension for the hash table H
   * @param r The size of a single dimension for the offset table Phi
   *
   * @returns True if the parameters are acceptable, False otherwise.
   */
  bool isAcceptableParameters(unsigned int m,
                              unsigned int r);

  /*! @brief Retrieve the neighbouring values within the offset table Phi
   *         around point p, and store them in candidate_vector.
   *
   * @param p The 3d point around which the values should be retrieved.
   * @param r_dim The size of a single dimension of the offset table Phi
   * @param offset_defined_table Vector describing whether a corresponding 
   *                             value in the offset table Phi is defined.
   * @param offset_table Offset table Phi
   * @param candidate_vector The output vector in which the candidates are 
   *                         stored.
   *
   * @post FORALL p_i: next_to(p_i, p) -> Phi[p_i] IN candidate_vector
   */
  void retrieveCandidates(glm::uvec3 p,
                          const Table<glm::u8vec3>& offset_table,
                          std::vector<glm::u8vec3>& candidate_vector);

  /*! @brief Check whether the candidate offset value is a valid offset
   *         given the elements and the currently defined hash table.
   * 
   * @param candidate The candidate offset value
   * @param m_dim The size of a single dimension of the hash table H
   * @param elements A vector containing all elements associated with a single
   *                 entry in the offset table.
   * @param hash_def_table_def Vector describing whether a corresponding value
   *                           in the hash table H is defined.
   *
   * @returns True if candidate is a valid offset value, False otherwise
   */
  bool isValidCandidate(glm::u8vec3 candidate,
                        const std::vector<EntryElement>& elements,
                        const Table<R>& hash_table);


private:
  /*! @brief mt19937 random generator of this SpatialHashFunctionBuilder. */
  std::mt19937 gen;
  /*! @brief Uniform unsigned short distribution. */
  std::uniform_int_distribution<unsigned short> distribution;
};




}
}
}
