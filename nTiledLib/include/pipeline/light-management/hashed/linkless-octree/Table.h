#pragma once

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <glm\glm.hpp>
#include <vector>


namespace nTiled {
namespace pipeline {
namespace hashed {

/*! @brief Table implements a simple Table used to construct 
 *         SpatialHashFunctions.
 */
template <class R>
class Table {
public:
  // --------------------------------------------------------------------------
  //  Constructor
  // --------------------------------------------------------------------------
  /*! @Brief Construct a new Table with the given dimension.
   * 
   * @param t_dim the dimension in one direction of this new Table
   * 
   * @return A new Table of size t_dim
   * @throws SpatialHashFunctionConstructionInvalidArgException
   *         IF t_dim = 0
   */
  Table(unsigned int t_dim);

  // --------------------------------------------------------------------------
  //  Methods
  // --------------------------------------------------------------------------
  /*! @brief return if the point at p is defined.
   * 
   * @param p The point which should be checked
   *
   * @return True if the point at p in this Table is defined, false otherwise.
   */
  bool isDefined(glm::uvec3 p) const;

  /*! @brief Get the data associated with point p in this Table
   *
   * @param p The point from which the data should be retrieved.
   * 
   * @return The data associated with point p within this Table
   * @throws SpatialHashFunctionConstructionIllegalAccessTableException 
   *         IF not this->isDefined(p)
   */
  R getPoint(glm::uvec3 p) const;

  /*! @brief Get the data vector that houses this tables data.
   *
   * @return The data vector that houses this tables data.
   */
  const std::vector<R>& getDataVector() const { return this->data; }

  /*! @brief Set the data associated with point p in this Table to d
   *
   * @param p The point where the data should be set
   * @param d The data set at point P
   *
   * @throws SpatialHashFunctionConstructionIllegalWriteTableException
   *         if this->isDefined(p)
   */
  void setPoint(glm::uvec3 p, R d);

  /*! @brief Get the size of a dimension of this Table
   * 
   * @return the size of a single dimension of this Table
   */
  inline unsigned int getDim() const { return this->t_dim; }

private:
  /*! @brief Vector containing whether points in data are defined. */
  std::vector<bool> is_def;
  /*! @brief Vector containing the data in this Table*/
  std::vector<R> data;
  /*! @brief The size of a dimension of this table. */
  unsigned int t_dim;
};


}
}
}
