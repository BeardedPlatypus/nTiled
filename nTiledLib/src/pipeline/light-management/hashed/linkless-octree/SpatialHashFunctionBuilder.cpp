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
SpatialHashFunctionBuilder<R>::SpatialHashFunctionBuilder(unsigned int seed) {
  this->gen = std::mt19937(seed);
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

  Table<R>* p_hash_table;
  Table<glm::u8vec3>* p_offset_table;

  std::vector<ConstructionElement> entry_vector;

  do {
    while (!this->isAcceptableParameters(m_dim, r_dim)) r_dim += 2;

    p_hash_table = new Table<R>(m_dim);
    p_offset_table = new Table<glm::u8vec3>(r_dim);

    if (mapEntryVector(entries, m_dim, r_dim, entry_vector)) {
      has_build = buildTables(entry_vector, 
                              *p_hash_table, 
                              *p_offset_table);
    }

    if (!has_build) {
      r_dim = unsigned int(ceil(ratio * r_dim));
      if ((r_dim & 1) == 0) r_dim++;

      delete p_hash_table;
      delete p_offset_table;


    }
    i++;
  } while (!has_build && i < max_attempts);

  // check if build
  // --------------------------------------------------------------------------
  if (!has_build) {
    throw SpatialHashFunctionConstructionExhaustedException();
  }

  return (new SpatialHashFunction<R>(p_hash_table,p_offset_table));
}


template <class R>
bool SpatialHashFunctionBuilder<R>::buildTables(
    const std::vector<ConstructionElement>& entry_vector,
    Table<R>& hash_table,
    Table<glm::u8vec3>& offset_table) {
  // --------------------------------------------------------------------------
  // sanitise input
  if (entry_vector.empty()) 
    throw SpatialHashFunctionConstructionInvalidArgException();

  // --------------------------------------------------------------------------
  // define common variables
  glm::uvec3 h_0;
  glm::uvec3 h_1;

  glm::u8vec3 offset;
  bool found_candidate;

  std::vector<glm::u8vec3> candidate_vector = std::vector<glm::u8vec3>();

  for (const ConstructionElement& e : entry_vector) {
    // stop construction if no elements map to this offset table entries
    // all further elements will also consists of zero elements due to sorting
    if (e.elements.size() == 0) {
      break;
    }

    found_candidate = false;

    // find offset from neighbouring offsets
    candidate_vector.clear();
    this->retrieveCandidates(e.hash_1,
                             offset_table,
                             candidate_vector);

    for (glm::u8vec3 candidate : candidate_vector) {
      if (isValidCandidate(candidate, e.elements, hash_table)) {
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
        if (isValidCandidate(candidate, e.elements, hash_table)) {
          offset = candidate;
          found_candidate = true;
          break;
        }
      }
    }

    if (!found_candidate) {
      return false;
    } else {
      offset_table.setPoint(e.hash_1, offset);

      for (const EntryElement& element : e.elements) {
        hash_table.setPoint(glm::uvec3((element.hash_0.x + offset.x) % hash_table.getDim(),
                                       (element.hash_0.y + offset.y) % hash_table.getDim(),
                                       (element.hash_0.z + offset.z) % hash_table.getDim()),
                            element.data);
      }
    }
  }

  return true;
}


template <class R>
bool SpatialHashFunctionBuilder<R>::mapEntryVector(
    const std::vector<std::pair<glm::uvec3, R>>& entries,
    unsigned int m_dim,
    unsigned int r_dim,
    std::vector<ConstructionElement>& result_entry_set) const {
  // --------------------------------------------------------------------------
  // Error Checking
  if (entries.empty()) 
    throw SpatialHashFunctionConstructionInvalidArgException();
  if (m_dim == 0) 
    throw SpatialHashFunctionConstructionInvalidArgException();
  if (r_dim == 0) 
    throw SpatialHashFunctionConstructionInvalidArgException();
  if (m_dim * m_dim * m_dim < entries.size())
    throw SpatialHashFunctionConstructionInvalidArgException();

  // --------------------------------------------------------------------------
  // Common variables
  glm::uvec3 h_0;
  glm::uvec3 h_1;

  // --------------------------------------------------------------------------
  // Prepare result
  result_entry_set.clear();
  result_entry_set.resize(r_dim * r_dim * r_dim);

  for (unsigned int x = 0; x < r_dim; x++) {
    for (unsigned int y = 0; y < r_dim; y++) {
      for (unsigned int z = 0; z < r_dim; z++) {
        h_1 = glm::uvec3(x, y, z);
        result_entry_set.at(math::toIndex(h_1, r_dim)) =  
          ConstructionElement(h_1);
      }
    }
  }

  // --------------------------------------------------------------------------
  // Add entries and check for double collisions
  unsigned int index;
  bool similar_element;

  for (std::pair<glm::uvec3, R> entry : entries) {
    h_0 = glm::uvec3((entry.first.x % m_dim),
                     (entry.first.y % m_dim),
                     (entry.first.z % m_dim));
    h_1 = glm::uvec3((entry.first.x % r_dim),
                     (entry.first.y % r_dim),
                     (entry.first.z % r_dim));
    index = math::toIndex(h_1, r_dim);

    similar_element = false;
    for (EntryElement e : result_entry_set.at(index).elements) {
      if ((e.hash_0.x == h_0.x) &&
          (e.hash_0.y == h_0.y) &&
          (e.hash_0.z == h_0.z)) {
        if (entry.second == e.data) {
          // element already exists within the set being added
          similar_element = true;
          break;
        } else {
          // element with similar hashes but different data is added
          // thus impossible to construct perfect spatial hash function
          return false;
        }
      }
    }

    if (!similar_element) {
      result_entry_set.at(index).elements.push_back(
        EntryElement(entry.first, h_0, entry.second));
    }
  }

  ConstructionElementCompare const_compare = ConstructionElementCompare();
  // sort offset table based on number of collisions
  std::sort(result_entry_set.begin(),
            result_entry_set.end(),
            const_compare);

  return true;
}


template <class R>
bool SpatialHashFunctionBuilder<R>::isAcceptableParameters(unsigned int m,
                                                           unsigned int r) {
  unsigned int m_mod_r = m % r;
  return (( r == 1 ) || 
          ( m == 1 ) ||
          ((math::gcd(m, r) == 1) && (m_mod_r != 1) && (m_mod_r != (r - 1)))); 
}


template <class R>
void SpatialHashFunctionBuilder<R>::retrieveCandidates(
    glm::uvec3 p,
    const Table<glm::u8vec3>& offset_table,
    std::vector<glm::u8vec3>& candidate_vector) {

  // --------------------------------------------------------------------------
  unsigned int val[3] = { 0, 1, (offset_table.getDim() - 1) };
  glm::uvec3 offset_point;
  glm::u8vec3 candidate;

  for (int x = 0; x < 3; x++) {
    for (int y = 0; y < 3; y++) {
      for (int z = 0; z < 3; z++) {
        offset_point = glm::uvec3((p.x + val[x]) % offset_table.getDim(),
                                  (p.y + val[y]) % offset_table.getDim(),
                                  (p.z + val[z]) % offset_table.getDim());
        
        if (offset_table.isDefined(offset_point)) {
          candidate = offset_table.getPoint(offset_point);

          if (std::find(candidate_vector.begin(),
                        candidate_vector.end(),
                        candidate) == candidate_vector.end()) {
            candidate_vector.push_back(candidate);
          }
        }
      }
    }
  }
}


template <class R>
bool SpatialHashFunctionBuilder<R>::isValidCandidate(
    glm::u8vec3 candidate,
    const std::vector<EntryElement>& elements,
    const Table<R>& hash_table) {
  // --------------------------------------------------------------------------
  // Sanitise input
  if (elements.empty()) throw SpatialHashFunctionConstructionInvalidArgException();
  if (elements.size() > ( hash_table.getDim() * 
                          hash_table.getDim() * 
                          hash_table.getDim() )) throw SpatialHashFunctionConstructionInvalidArgException();

  // --------------------------------------------------------------------------
  for (EntryElement e : elements) {
    if (hash_table.isDefined(glm::uvec3(
      (e.hash_0.x + candidate.x) % hash_table.getDim(),
      (e.hash_0.y + candidate.y) % hash_table.getDim(),
      (e.hash_0.z + candidate.z) % hash_table.getDim()))) {
      return false;
    }
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