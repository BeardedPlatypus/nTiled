#include "pipeline\light-management\hashed\linkless-octree\Table.h"

// ----------------------------------------------------------------------------
//  nTiled Headers
// ----------------------------------------------------------------------------
#include "pipeline\light-management\hashed\linkless-octree\Exceptions.h"
#include "math\util.h"


namespace nTiled {
namespace pipeline {
namespace hashed {

template <class R>
Table<R>::Table(unsigned int t_dim) : 
  t_dim(t_dim),
  is_def(std::vector<bool>(t_dim * t_dim * t_dim, false)),
  data(std::vector<R>(t_dim * t_dim * t_dim)) { 
  if (t_dim == 0) throw SpatialHashFunctionConstructionInvalidArgException();
}


template <class R>
bool Table<R>::isDefined(glm::uvec3 p) const {
  return this->is_def.at(math::toIndex(p, this->t_dim));
}


template <class R>
R Table<R>::getPoint(glm::uvec3 p) const {
  if (!this->isDefined(p)) {
    throw SpatialHashFunctionConstructionIllegalAccessTableException();
  }

  return this->data.at(math::toIndex(p, this->t_dim));
}


template <class R>
void Table<R>::setPoint(glm::uvec3 p, R d) {
  if (this->isDefined(p)) {
    throw SpatialHashFunctionConstructionIllegalWriteTableException();
  }

  unsigned int index = math::toIndex(p, this->getDim());
  this->data.at(index) = d;
  this->is_def.at(index) = true;
}


template class Table<glm::u8vec3>;
template class Table<glm::u8vec2>;
template class Table<glm::uvec2>;

}
}
}
