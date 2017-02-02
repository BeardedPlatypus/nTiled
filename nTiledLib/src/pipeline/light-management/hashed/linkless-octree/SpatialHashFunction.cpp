#include "pipeline\light-management\hashed\linkless-octree\SpatialHashFunction.h"

// ---------------------------------------------------------------------------
//  Libraries
// ---------------------------------------------------------------------------
#include <cmath>
#include <algorithm>
#include <stdlib.h> 
#include <exception>


// ---------------------------------------------------------------------------
//  nTiled Headers 
// ---------------------------------------------------------------------------
#include "math\util.h"


// ---------------------------------------------------------------------------
//  Exception definitions
// ---------------------------------------------------------------------------
class OctreeException : public std::exception {
  virtual const char* what() const throw() {
    return "Octree has exceeded build attempts.";
  }
} octree_ex;


namespace nTiled {
namespace pipeline {
namespace hashed {

// ----------------------------------------------------------------------------
//  Constructor | Destructor
// ----------------------------------------------------------------------------
template <class R>
SpatialHashFunction<R>::SpatialHashFunction(
    const std::vector<std::pair<glm::uvec3, R>>& entries,
    unsigned int max_attempts,
    float ratio,
    std::mt19937& gen,
    std::uniform_int_distribution<unsigned short>& distribution) {

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

  while (!has_build && i < max_attempts) {
    while (!isAcceptableParameters(m_dim, r_dim)) r_dim += 2;
    has_build = buildTables(m_dim, r_dim, entries, gen, distribution);

    if (!has_build) {
      r_dim = unsigned int(ceil(ratio * r_dim));
      if ((r_dim & 1) == 0) r_dim++;
    }
    i++;
  }

  // check if build
  // --------------------------------------------------------------------------
  if (!has_build) {
    throw octree_ex;
  }
}


template <class R>
SpatialHashFunction<R>::~SpatialHashFunction() {
  delete this->p_hash_table;
  delete this->p_offset_table;
}


// ----------------------------------------------------------------------------
//  buildTables
// ----------------------------------------------------------------------------
//  Supporting structs
template <class R>
SpatialHashFunction<R>::EntryElement::EntryElement(glm::uvec3 point,
                                                   glm::uvec3 hash_0,
                                                   R data) :
  point(point), hash_0(hash_0), data(data) {
}


template <class R>
SpatialHashFunction<R>::ConstructionElement::ConstructionElement() : ConstructionElement(glm::uvec3(0)) {}

template <class R>
SpatialHashFunction<R>::ConstructionElement::ConstructionElement(glm::uvec3 hash_1) :
  hash_1(hash_1), elements(std::vector<EntryElement>()) {
}


// ----------------------------------------------------------------------------
//  Function
template <class R>
bool SpatialHashFunction<R>::buildTables(
    unsigned int m,
    unsigned int r,
    std::vector<std::pair<glm::uvec3, R>> entries,
    std::mt19937& gen,
    std::uniform_int_distribution<unsigned short>& distribution) {
  // --------------------------------------------------------------------------
  //  define common variables
  glm::uvec3 h_0;
  glm::uvec3 h_1;

  // --------------------------------------------------------------------------
  //  construct datastructures

  // hash table
  // ----------
  this->p_hash_table = new std::vector<R>(m * m * m);
  std::vector<bool> hash_table_def = std::vector<bool>(m * m * m, 
                                                       false);

  // offset table
  // ------------
  this->p_offset_table = new std::vector<glm::u8vec3>(r * r * r, 
                                                      glm::u8vec3(0));
  std::vector<bool> offset_table_def = std::vector<bool>(r * r * r, 
                                                         false);

  std::vector<ConstructionElement> offset_table_raw =
    std::vector<ConstructionElement>(r * r * r);

  // construct offset_table construction entries
  // -------------------------------------------
  for (unsigned int x = 0; x < r; x++) {
    for (unsigned int y = 0; y < r; y++) {
      for (unsigned int z = 0; z < r; z++) {
        h_1 = glm::uvec3(x, y, z);
        offset_table_raw[toIndex(h_1, r)] = ConstructionElement(h_1);
      }
    }
  }

  // --------------------------------------------------------------------------
  //  build the values for the offset table
  for (std::pair<glm::uvec3, R> e : entries) {
    h_0 = glm::uvec3(e.first.x % m,
                     e.first.y % m,
                     e.first.z % m);
    h_1 = glm::uvec3(e.first.x % r,
                     e.first.y % r,
                     e.first.z % r);
    offset_table_raw[toIndex(h_1, r)].elements.push_back(
      EntryElement(e.first, h_0, e.second));
  }

  // sort offset table based on number of collisions
  std::sort(offset_table_raw.begin(),
            offset_table_raw.end(),
            compare_struct);

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
                             r,
                             offset_table_def,
                             *p_offset_table,
                             candidate_vector);
                                  
    for (glm::u8vec3 candidate : candidate_vector) {
      if (isValidCandidate(candidate, m, e.elements, hash_table_def)) {
        offset = candidate;
        found_candidate = true;
        break;
      }
    }

    // find offset by random iteration
    if (!found_candidate) {
      for (unsigned int i = 0; i < 256 * 256 * 256; i++) {
        glm::u8vec3 candidate = glm::u8vec3(distribution(gen),
                                            distribution(gen),
                                            distribution(gen));
        if (isValidCandidate(candidate, m, e.elements, hash_table_def)) {
          offset = candidate;
          found_candidate = true;
          break;
        }
      }
    }

    // assign offset or return failure
    if (!found_candidate) {
      delete this->p_offset_table;
      delete this->p_hash_table;

      return false;
    } else {
      offset_index = toIndex(e.hash_1, r);
      offset_table_def[offset_index] = true;
      (*p_offset_table)[offset_index] = offset;

      for (const EntryElement& element : e.elements) {
        hash_index = toIndex(glm::uvec3((element.hash_0.x + offset.x) % m,
                                        (element.hash_0.y + offset.y) % m,
                                        (element.hash_0.z + offset.z) % m), m);

        hash_table_def[hash_index] = true;
        (*p_hash_table)[hash_index] = element.data;
      }
    }
  }

  // assign values to object
  this->m_dim = m;
  this->r_dim = r;

  return true;
}


// ----------------------------------------------------------------------------
//  Supporting functions
template <class R>
bool SpatialHashFunction<R>::isAcceptableParameters(unsigned int m,
                                                    unsigned int r) {
  unsigned int m_mod_r = m % r;
  return (r == 1 || (math::gcd(m, r) == 1 && m_mod_r != 1 && m_mod_r != (r - 1))); // if has only one entry than it is acceptable
}


template <class R>
void SpatialHashFunction<R>::retrieveCandidates(glm::uvec3 p,
                                                unsigned int dim,
                                                const std::vector<bool>& offset_def,
                                                const std::vector<glm::u8vec3>& offset_val,
                                                std::vector<glm::u8vec3>& candidate_vector) {
  unsigned int val[3] = { 0, 1, (dim - 1) };
  unsigned int index;

  for (int x = 0; x < 3; x++) {
    for (int y = 0; y < 3; y++) {
      for (int z = 0; z < 3; z++) {
        index = this->toIndex(glm::uvec3((p.x + val[x]) % dim,
                                         (p.y + val[y]) % dim,
                                         (p.z + val[z]) % dim), dim);
        if (offset_def[index] && 
            std::find(candidate_vector.begin(),
                      candidate_vector.end(),
                      offset_val[index]) == candidate_vector.end()) {
          candidate_vector.push_back(offset_val[index]);
        }
      }
    }
  }
}


template <class R>
bool SpatialHashFunction<R>::isValidCandidate(glm::u8vec3 candidate,
                                              unsigned int m,
                                              const std::vector<EntryElement>& elements,
                                              const std::vector<bool>& hash_table_def) {
  for (EntryElement e : elements) {
    unsigned index = toIndex(glm::uvec3((e.hash_0.x + candidate.x) % m,
                                        (e.hash_0.y + candidate.y) % m,
                                        (e.hash_0.z + candidate.z) % m), m);
    if (hash_table_def[index])
      return false;
  }

  return true;
}


template class SpatialHashFunction<glm::u8vec2>;
template class SpatialHashFunction<glm::uvec2>;

}
}
}
