#include "pipeline\light-management\hashed\linkless-octree\SpatialHashFunctionBuilder.h"

// ---------------------------------------------------------------------------
//  Libraries
// ---------------------------------------------------------------------------


namespace nTiled {
namespace pipeline {
namespace hashed {

// ---------------------------------------------------------------------------
// SpatialHashFunctionBuilder
// ---------------------------------------------------------------------------
template <class R>
SpatialHashFunctionBuilder<R>::SpatialHashFunctionBuilder() {

}


template <class R>
SpatialHashFunction<R>* SpatialHashFunctionBuilder<R>::constructHashFunction(
    const std::vector<std::pair<glm::uvec3, R>>& entries,
    unsigned int max_attempts,
    float ratio) {
  return nullptr;
}


template <class R>
std::pair<std::vector<R>*, std::vector<glm::u8vec3>*> SpatialHashFunctionBuilder<R>::buildTables(
    unsigned int m_dim,
    unsigned int r_dim,
    const std::vector<std::pair<glm::uvec3, R>>& entries) {
  std::vector<R>* p_hash_table = new std::vector<R>();
  std::vector<glm::u8vec3>* p_offset_table = new std::vector<glm::u8vec3>();

  std::pair<std::vector<R>*, std::vector<glm::u8vec3>*> result =
    std::pair<std::vector<R>*, std::vector<glm::u8vec3>*>(p_hash_table, p_offset_table);

  return result;
}


template <class R>
bool SpatialHashFunctionBuilder<R>::isAcceptableParameters(unsigned int m,
                                                        unsigned int r) {
  return false;
}


template <class R>
void SpatialHashFunctionBuilder<R>::retrieveCandidates(
    glm::uvec3 p,
    unsigned int r_dim,
    const std::vector<bool>& offset_defined_table,
    const std::vector<glm::u8vec3>& offset_table,
    std::vector<glm::u8vec3>& candidate_vector) {
}


template <class R>
bool SpatialHashFunctionBuilder<R>::isValidCandidate(
    glm::u8vec3 candidate,
    unsigned int m_dim,
    const std::vector<glm::uvec3>& elements,
    const std::vector<bool>& hash_defined_table) {
  return false;
}


// ---------------------------------------------------------------------------
// ConstructionElement
// ---------------------------------------------------------------------------
template <class R>
SpatialHashFunctionBuilder<R>::ConstructionElement::ConstructionElement() : 
  ConstructionElement(glm::uvec3(0)) {
}


template <class R>
SpatialHashFunctionBuilder<R>::ConstructionElement::ConstructionElement(glm::uvec3 hash_1) :
    hash_1(hash_1), 
    elements(std::vector<EntryElement>()) {
}


// ---------------------------------------------------------------------------
// EntryElement
// ---------------------------------------------------------------------------
template <class R>
SpatialHashFunctionBuilder<R>::EntryElement::EntryElement(glm::uvec3 point,
                              glm::uvec3 hash_0,
                              R data) :
    point(point), 
    hash_0(hash_0), 
    data(data) {
}


// Class initialisations
template class SpatialHashFunctionBuilder<glm::u8vec2>;
template class SpatialHashFunctionBuilder<glm::uvec2>;

} // hashed
} // pipeline
} // nTiled