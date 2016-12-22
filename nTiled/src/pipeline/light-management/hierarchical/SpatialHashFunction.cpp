#include "pipeline\light-management\hierarchical\SpatialHashFunction.h"

#include <cmath>
#include <algorithm>
#include <stdlib.h> 
#include <exception>

#include "math\util.h"


class OctreeException : public std::exception {
  virtual const char* what() const throw() {
    return "Octree has exceeded build attempts.";
  }
} octree_ex;


namespace nTiled {
namespace pipeline {
namespace hierarchical {

// ----------------------------------------------------------------------------
//  Constructor | Destructor
// ----------------------------------------------------------------------------
template <class R>
SpatialHashFunction<R>::SpatialHashFunction<R>(
    const std::vector<std::pair<glm::uvec3, R>>& entries,
    unsigned int max_attempts, 
    float ratio) {
  // build initial values
  // --------------------------------------------------------------------------
  unsigned int n = entries.size();

  unsigned int m_dim;
  if (n > (255 * 255 * 255)) m_dim = ceil(cbrt(n * 1.01));
  else m_dim = ceil(cbrt(n));

  unsigned int r_dim = ceil(cbrt(n * (1 / 6)));

  if ((m_dim & 1) == 0) m_dim++;
  if ((r_dim & 1) == 0) r_dim++;

  // build tables
  // --------------------------------------------------------------------------
  unsigned short i = 0;
  bool has_build = false;

  while (!has_build && i < max_attempts) {
    while (!isAcceptableParameters(m_dim, r_dim)) r_dim += 2;
    has_build = buildTables(m_dim, r_dim, entries);
    
    if (!has_build) {
      r_dim = ceil(ratio * r_dim);
      if ((r_dim & 1) == 0) r_dim++;
    }
  }

  // check if build
  // --------------------------------------------------------------------------
  if (!has_build) {
    throw octree_ex;
  }
}


template <class R>
SpatialHashFunction<R>::~SpatialHashFunction() {
  delete this->hash_table;
  delete this->offset_table;
}


// ----------------------------------------------------------------------------
//  buildTables
// ----------------------------------------------------------------------------
//  Supporting structs
template <class R>
EntryElement<R>::EntryElement<R>(glm::uvec3 point, glm::uvec3 hash_0, R data) :
    point(point), hash_0(hash_0), data(data) { }


template <class R>
ConstructionElement<R>::ConstructionElement<R>(glm::uvec3 hash_1) : 
      hash_1(hash_1), elements(std::vector<EntryElement<R>>()) {}


template <class R>
bool compareConstructionElement(const ConstructionElement<R>& t1,
                                const ConstructionElement<R>& t2) {
  return (t1.elements.size() < t2.elements.size());
}


// ----------------------------------------------------------------------------
//  Function
template <class R>
bool SpatialHashFunction<R>::buildTables(
    unsigned int m,
    unsigned int r,
    std::vector<std::pair<glm::uvec3, R>> entries) {
  // --------------------------------------------------------------------------
  //  define common variables
  glm::uvec3 h_0;
  glm::uvec3 h_1;

  // --------------------------------------------------------------------------
  //  construct datastructures
  std::vector<bool> hash_table_def = std::vector<bool>(m * m * m, false);
  std::vector<R> p_hash_table = new std::vector<bool>(m * m * m);

  std::vector<bool> offset_table_def = std::vector<bool>(r * r * r, false);
  std::vector<GLushort>* p_offset_table = 
    new std::vector<GLushort>(r * r * r, 0);

  std::vector<ConstructionElement<R>> offset_table_raw =
    std::vector<ConstructionElement<R>>(r * r * r);

  // construct offset_table entries
  for (int x = 0; x < r; x++) {
    for (int y = 0; y < r; y++) {
      for (int z = 0; z < r; z++) {
        h_1 = glm::uvec3(x, y, z);
        offset_table_raw[toIndex(h_1)] = ConstructionElement(h_1);
      }
    }
  }

  // --------------------------------------------------------------------------
  //  build the values for the offset table
  for (std::pair<glm::uvec3, R> e : entries) {
    h_1 = glm::mod(e.first, r);
    h_0 = glm::mod(e.first, m);
    offset_table_raw[toIndex(h_1)].elements.push_back(
      EntryElement(e.first, h_0, e.second));
  }

  std::sort(offset_table_raw.begin(), 
            offset_table_raw.end(), 
            compareConstructionElement<R>);

  // --------------------------------------------------------------------------
  //  build tables
  GLushort offset;
  bool found_candidate;

  glm::uvec3 offset_vec;
  unsigned int hash_index;
  unsigned int offset_index;

  for (const ConstructionElement& e : offset_table_raw) {
    found_candidate = false;

    // find offset from neighbouring offsets
    for (GLushort candidate : this->retrieveCandidates<R>(e.hash_1, 
                                                          r, 
                                                          offset_table_def,
                                                          *p_offset_table)) {
      if (isValidCandidate(candidate, m, e.elements, hash_table_def)) {
        offset = candidate;
        found_candidate = true;
        break;
      }
    }

    // find offset by random iteration
    if (!found_candidate) {
      // TODO check if better way
      std::vector<GLushort> indices = std::vector<GLushort>();
      for (GLushort i = 0; i < 256; i++) indices.push_back(i);
      std::random_shuffle(indices.begin(), indices.end());

      for (unsigned int i = 0; i < 256; i++) {
        if (isValidCandidate(indices[i], m, e.elements, hash_table_def)) {
          offset = indices[i];
          found_candidate = true;
          break;
        }
      }
    }

    // assign offset or return failure
    if (!found_candidate) {
      delete p_hash_table;
      delete p_hash_table;

      return false;
    } else {
      offset_index = toIndex(e.hash_1);
      offset_table_def[offset_index] = true;
      (*p_offset_table)[offset_index] = offset;

      offset_vec = glm::uvec3(offset, offset, offset);
      for (const EntryElement& element : e.elements) {
        hash_index = toIndex(glm::mod(element.hash_0 + offset_vec, m));
        
        hash_table_def[hash_index] = true;
        (*p_hash_table)[hash_index] = element.data;
      }
    }
  }

  // assign values to object
  this->m_dim = m;
  this->r_dim = r;

  this->p_hash_table = p_hash_table;
  this->p_offset_table = offset_table;
  return true;
}


// ----------------------------------------------------------------------------
//  Supporting functions
template <class R>
bool SpatialHashFunction<R>::isAcceptableParameters(unsigned int m,
                                                    unsigned int r) {
  unsigned int m_mod_r = m % r;
  return (math::gcd(m, r) == 1 && m_mod_r != 1 && m_mod_r != (r - 1));
}


template <class R>
std::vector<GLushort> SpatialHashFunction<R>::retrieveCandidates(
    glm::uvec3 p,                        
    unsigned int dim,     
    const std::vector<<bool>& offset_def,
    const std::vector<GLushort>& offset_val) {
  std::vector<GLushort> candidates = std::vector<GLushort>();

  unsigned int val[3] = { 0, 1, dim };
  unsigned int i;
  for (int x = 0; x < 3; x++) {
    for (int y = 0; y < 3; y++) {
      for (int z = 0; z < 3; z++) {
        index = this->toIndex(glm::mod(p + glm::uvec3(val[x], val[y], val[z]), dim));
        if (!offset_def[index]) candidates.push_back(offset_val[index]);
      }
    }
  }
  
  return candidates;
}


template <class R>
bool SpatialHashFunction<R>::isValidCandidate(
    GLushort candidate, 
    unsigned int m,
    const std::vector<EntryElement<R>>& elements,
    const std::vector<bool> hash_table_def) {
  glm::uvec3 vec_candidate = glm::uvec3(candidate, candidate, candidate);
  for (EntryElement<R> e : elements) {
    if (hash_table_def[glm::mod(toIndex(e.hash_0 + vec_candidate, m))])
      return false;
  }

  return true;
}

}
}
}