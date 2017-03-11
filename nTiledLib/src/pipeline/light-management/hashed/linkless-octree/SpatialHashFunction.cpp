#include "pipeline\light-management\hashed\linkless-octree\SpatialHashFunction.h"

#include "pipeline\light-management\hashed\linkless-octree\Exceptions.h"


// ---------------------------------------------------------------------------
//  Libraries
// ---------------------------------------------------------------------------


namespace nTiled {
namespace pipeline {
namespace hashed {

template <class R>
SpatialHashFunction<R>::SpatialHashFunction(Table<R>* p_hash_table,
                                            Table<glm::u8vec3>* p_offset_table) :
    p_hash_table(p_hash_table),
    p_offset_table(p_offset_table) {
}


template <class R>
SpatialHashFunction<R>::~SpatialHashFunction() {
  delete this->p_hash_table;
  delete this->p_offset_table;
}


// TODO: change this to a maybe?
template <class R>
R SpatialHashFunction<R>::getData(glm::uvec3 p) const {
  glm::uvec3 h_1 = glm::uvec3((p.x % p_offset_table->getDim()),
                              (p.y % p_offset_table->getDim()),
                              (p.z % p_offset_table->getDim()));

  if (!this->p_offset_table->isDefined(h_1)) {
    throw nTiled::pipeline::hashed::SpatialHashFunctionIllegalAccessException();
  }

  glm::u8vec3 offset = this->p_offset_table->getPoint(h_1);

  glm::uvec3 h = glm::uvec3(((p.x + offset.x) % p_hash_table->getDim()),
                            ((p.y + offset.y) % p_hash_table->getDim()),
                            ((p.z + offset.z) % p_hash_table->getDim()));

  if (!this->p_hash_table->isDefined(h)) {
    throw nTiled::pipeline::hashed::SpatialHashFunctionIllegalAccessException();
  }

  return p_hash_table->getPoint(h);
}


template class SpatialHashFunction<glm::u8vec2>;
template class SpatialHashFunction<glm::uvec2>;

} // hashed
} // pipeline
} // nTiled
