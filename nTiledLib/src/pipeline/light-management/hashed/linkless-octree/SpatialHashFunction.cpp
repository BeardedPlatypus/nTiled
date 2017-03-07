#include "pipeline\light-management\hashed\linkless-octree\SpatialHashFunction.h"

#include "pipeline\light-management\hashed\linkless-octree\SpatialHashFunction.h"


// ---------------------------------------------------------------------------
//  Libraries
// ---------------------------------------------------------------------------


namespace nTiled {
namespace pipeline {
namespace hashed {

template <class R>
SpatialHashFunction<R>::SpatialHashFunction(unsigned int m_dim,
                                            std::vector<R>* p_hash_table,
                                            unsigned int r_dim,
                                            std::vector<glm::u8vec3>* p_offset_table) :
    m_dim(m_dim),
    p_hash_table(p_hash_table),
    r_dim(r_dim),
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
  return this->p_hash_table->at(0);
}


template class SpatialHashFunction<glm::u8vec2>;
template class SpatialHashFunction<glm::uvec2>;

} // hashed
} // pipeline
} // nTiled
