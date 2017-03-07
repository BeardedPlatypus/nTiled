#include "pipeline\light-management\hashed\linkless-octree\SpatialHashFunctionBuilder.h"

// ---------------------------------------------------------------------------
//  Libraries
// ---------------------------------------------------------------------------
#include <cmath>
#include <algorithm>
#include <stdlib.h> 

// ---------------------------------------------------------------------------
//  nTiled Headers
// ---------------------------------------------------------------------------
#include "math\util.h"
#include "pipeline\light-management\hashed\linkless-octree\Exceptions.h"

namespace nTiled {
namespace pipeline {
namespace hashed {

// ---------------------------------------------------------------------------
// SpatialHashFunctionBuilder
// ---------------------------------------------------------------------------
template <class R>
SpatialHashFunctionBuilder<R>::SpatialHashFunctionBuilder() {
  this->gen = std::mt19937(22);
  this->distribution = std::uniform_int_distribution<unsigned short>(0, 255);
}


template <class R>
SpatialHashFunction<R>* SpatialHashFunctionBuilder<R>::constructHashFunction(
    const std::vector<std::pair<glm::uvec3, R>>& entries,
    unsigned int max_attempts,
    float ratio) {
  // Sanitise input
  // --------------------------------------------------------------------------
  if (entries.empty()) throw SpatialHashFunctionConstructionException();

  // build initial values
  // --------------------------------------------------------------------------
  unsigned int n = entries.size();

  unsigned int m_dim;
  if (n > (255 * 255 * 255)) m_dim = unsigned int(ceil(cbrt(n * 1.01)));
  else m_dim = unsigned int(ceil(cbrt(n)));

  unsigned int r_dim = unsigned int(ceil(cbrt(n * (1.0 / 6.0))));

  if ((m_dim & 1) == 0) m_dim++;
  if ((r_dim & 1) == 0) r_dim++;

  // build tables
  // --------------------------------------------------------------------------
  unsigned short i = 0;
  bool has_build = false;

  std::vector<R>* p_hash_table = new std::vector<R>();
  std::vector<glm::u8vec3>* p_offset_table = new std::vector<glm::u8vec3>();

  while (!has_build && i < max_attempts) {
    while (!this->isAcceptableParameters(m_dim, r_dim)) r_dim += 2;
    has_build = buildTables(m_dim, 
                            r_dim, 
                            entries, 
                            *p_hash_table, 
                            *p_offset_table);

    if (!has_build) {
      r_dim = unsigned int(ceil(ratio * r_dim));
      if ((r_dim & 1) == 0) r_dim++;

      p_hash_table->clear();
      p_offset_table->clear();
    }
    i++;
  }

  // check if build
  // --------------------------------------------------------------------------
  if (!has_build) {
    throw SpatialHashFunctionConstructionExhaustedException();
  }

  return (new SpatialHashFunction<R>(m_dim, p_hash_table, r_dim, p_offset_table));

}


template <class R>
bool SpatialHashFunctionBuilder<R>::buildTables(
    unsigned int m_dim,
    unsigned int r_dim,
    const std::vector<std::pair<glm::uvec3, R>>& entries,
    std::vector<R>& hash_table,
    std::vector<glm::u8vec3>& offset_table) {
  // --------------------------------------------------------------------------
  // Sanitise input
  if (m_dim == 0) throw SpatialHashFunctionConstructionInvalidArgException();
  if (r_dim == 0) throw SpatialHashFunctionConstructionInvalidArgException();
  if (entries.empty()) throw SpatialHashFunctionConstructionInvalidArgException();
  if (entries.size() > ( m_dim * m_dim * m_dim )) throw SpatialHashFunctionConstructionInvalidArgException();

  if (!hash_table.empty()) hash_table.clear();
  if (!offset_table.empty()) offset_table.clear();

  // --------------------------------------------------------------------------
  //  define common variables
  glm::uvec3 h_0;
  glm::uvec3 h_1;

  // --------------------------------------------------------------------------
  //  prepare datastructures
  std::vector<bool> hash_table_def = std::vector<bool>(m_dim * m_dim * m_dim, false);
  std::vector<bool> offset_table_def = std::vector<bool>(r_dim * r_dim * r_dim, false);

  hash_table.reserve(m_dim * m_dim * m_dim);
  offset_table.reserve(r_dim * r_dim * r_dim);

  std::vector<ConstructionElement> offset_table_raw = std::vector<ConstructionElement>(r_dim * r_dim * r_dim);

  for (unsigned int x = 0; x < r_dim; x++) {
    for (unsigned int y = 0; y < r_dim; y++) {
      for (unsigned int z = 0; z < r_dim; z++) {
        h_1 = glm::uvec3(x, y, z);
        offset_table_raw[math::toIndex(h_1, r_dim)] = ConstructionElement(h_1);
      }
    }
  }

  // --------------------------------------------------------------------------
  //  build the values for the offset table
  for (std::pair<glm::uvec3, R> e : entries) {
    h_0 = glm::uvec3(e.first.x % m_dim,
                     e.first.y % m_dim,
                     e.first.z % m_dim);
    h_1 = glm::uvec3(e.first.x % r_dim,
                     e.first.y % r_dim,
                     e.first.z % r_dim);
    offset_table_raw[math::toIndex(h_1, r_dim)].elements.push_back(EntryElement(e.first, h_0, e.second));
  }

  ConstructionElementCompare const_compare = ConstructionElementCompare();
  // sort offset table based on number of collisions
  std::sort(offset_table_raw.begin(),
            offset_table_raw.end(),
            const_compare);

  // --------------------------------------------------------------------------
  //  build tables
  glm::u8vec3 offset;
  bool found_candidate;

  unsigned int hash_index;
  unsigned int offset_index;

  std::vector<glm::u8vec3> candidate_vector = std::vector<glm::u8vec3>();

  for (const ConstructionElement& e : offset_table_raw) {
    // stop construction if no elements map to this offset table entries
    // all further elements will also consists of zero elements due to sorting
    if (e.elements.size() == 0) {
      break;
    }

    found_candidate = false;

    // find offset from neighbouring offsets
    candidate_vector.clear();
    this->retrieveCandidates(e.hash_1,
                             r_dim,
                             offset_table_def,
                             offset_table,
                             candidate_vector);

    for (glm::u8vec3 candidate : candidate_vector) {
      if (isValidCandidate(candidate, m_dim, e.elements, hash_table_def)) {
        offset = candidate;
        found_candidate = true;
        break;
      }
    }

    // find offset by random iteration
    if (!found_candidate) {
      for (unsigned int i = 0; i < 256 * 256 * 256; i++) {
        glm::u8vec3 candidate = glm::u8vec3(this->distribution(this->gen),
                                            this->distribution(this->gen),
                                            this->distribution(this->gen));
        if (isValidCandidate(candidate, m_dim, e.elements, hash_table_def)) {
          offset = candidate;
          found_candidate = true;
          break;
        }
      }
    }

    if (!found_candidate) {
      return false;
    } else {
      offset_index = math::toIndex(e.hash_1, r_dim);
      offset_table_def[offset_index] = true;
      offset_table[offset_index] = offset;

      for (const EntryElement& element : e.elements) {
        hash_index = math::toIndex(glm::uvec3((element.hash_0.x + offset.x) % m_dim,
                                              (element.hash_0.y + offset.y) % m_dim,
                                              (element.hash_0.z + offset.z) % m_dim), m_dim);

        hash_table_def[hash_index] = true;
        hash_table[hash_index] = element.data;
      }
    }
  }

  return true;
}


template <class R>
bool SpatialHashFunctionBuilder<R>::isAcceptableParameters(unsigned int m,
                                                        unsigned int r) {
  unsigned int m_mod_r = m % r;
  return (r == 1 || (math::gcd(m, r) == 1 && m_mod_r != 1 && m_mod_r != (r - 1))); 
}


template <class R>
void SpatialHashFunctionBuilder<R>::retrieveCandidates(
    glm::uvec3 p,
    unsigned int r_dim,
    const std::vector<bool>& offset_defined_table,
    const std::vector<glm::u8vec3>& offset_table,
    std::vector<glm::u8vec3>& candidate_vector) {
  // --------------------------------------------------------------------------
  // Sanitise input
  if (r_dim == 0) throw SpatialHashFunctionConstructionInvalidArgException();

  // --------------------------------------------------------------------------
  unsigned int val[3] = { 0, 1, (r_dim - 1) };
  unsigned int index;

  for (int x = 0; x < 3; x++) {
    for (int y = 0; y < 3; y++) {
      for (int z = 0; z < 3; z++) {
        index = math::toIndex(glm::uvec3((p.x + val[x]) % r_dim,
                                         (p.y + val[y]) % r_dim,
                                         (p.z + val[z]) % r_dim), r_dim);
        if (offset_defined_table[index] &&
            std::find(candidate_vector.begin(),
                      candidate_vector.end(),
                      offset_table[index]) == candidate_vector.end()) {
          candidate_vector.push_back(offset_table[index]);
        }
      }
    }
  }
}


template <class R>
bool SpatialHashFunctionBuilder<R>::isValidCandidate(
    glm::u8vec3 candidate,
    unsigned int m_dim,
    const std::vector<EntryElement>& elements,
    const std::vector<bool>& hash_defined_table) {
  // --------------------------------------------------------------------------
  // Sanitise input
  if (m_dim == 0) throw SpatialHashFunctionConstructionInvalidArgException();
  if (elements.empty()) throw SpatialHashFunctionConstructionInvalidArgException();
  if (elements.size() > ( m_dim * m_dim * m_dim )) throw SpatialHashFunctionConstructionInvalidArgException();

  // --------------------------------------------------------------------------
  for (EntryElement e : elements) {
    unsigned index = math::toIndex(glm::uvec3((e.hash_0.x + candidate.x) % m_dim,
                                              (e.hash_0.y + candidate.y) % m_dim,
                                              (e.hash_0.z + candidate.z) % m_dim), m_dim);
    if (hash_defined_table[index]) return false;
  }

  return true;
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