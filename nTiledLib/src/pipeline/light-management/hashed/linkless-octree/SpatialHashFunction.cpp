#include "pipeline\light-management\hashed\linkless-octree\SpatialHashFunction.h"

#include "pipeline\light-management\hashed\linkless-octree\SpatialHashFunction.h"


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
  return this->p_hash_table->getPoint(glm::uvec3(0));
}


template class SpatialHashFunction<glm::u8vec2>;
template class SpatialHashFunction<glm::uvec2>;

} // hashed
} // pipeline
} // nTiled
