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
SpatialHashFunction<R>::SpatialHashFunction(
    const std::vector<std::pair<glm::uvec3, R>>& entries,
    unsigned int max_attempts, 
    float ratio) {
  // build initial values
  // --------------------------------------------------------------------------
  unsigned int n = entries.size();

  unsigned int m_dim;
  if (n > (255 * 255 * 255)) m_dim = unsigned int(ceil(cbrt(n * 1.01)));
  else m_dim = unsigned int(ceil(cbrt(n)));

  unsigned int r_dim = unsigned int(ceil(cbrt(n * (1 / 6))));

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
    point(point), hash_0(hash_0), data(data) { }


template <class R>
SpatialHashFunction<R>::ConstructionElement::ConstructionElement() : ConstructionElement(glm::uvec3(0)) { }

template <class R>
SpatialHashFunction<R>::ConstructionElement::ConstructionElement(glm::uvec3 hash_1) : 
      hash_1(hash_1), elements(std::vector<EntryElement>()) {}


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
  this->p_hash_table = new std::vector<R>(m * m * m);

  std::vector<bool> offset_table_def = std::vector<bool>(r * r * r, false);
  this->p_offset_table = 
    new std::vector<GLushort>(r * r * r, 0);

  std::vector<ConstructionElement> offset_table_raw =
    std::vector<ConstructionElement>(r * r * r);

  // construct offset_table entries
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
    h_1 = glm::uvec3(e.first.x % r,
                     e.first.y % r,
                     e.first.z % r);
    h_0 = glm::uvec3(e.first.x % m,
                     e.first.y % m,
                     e.first.z % m);
    offset_table_raw[toIndex(h_1, r)].elements.push_back(
      EntryElement(e.first, h_0, e.second));
  }

  std::sort(offset_table_raw.begin(), 
            offset_table_raw.end(), 
            compare_struct);

  // --------------------------------------------------------------------------
  //  build tables
  GLushort offset;
  bool found_candidate;

  glm::uvec3 offset_vec;
  glm::uvec3 index;
  unsigned int hash_index;
  unsigned int offset_index;

  for (const ConstructionElement& e : offset_table_raw) {
    if (e.elements.size() == 0) {
      break;
    }

    found_candidate = false;

    // find offset from neighbouring offsets
    for (GLushort candidate : this->retrieveCandidates(e.hash_1, 
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
      //delete p_offset_table;
      delete p_hash_table;

      return false;
    } else {
      offset_index = toIndex(e.hash_1, r);
      offset_table_def[offset_index] = true;
      (*p_offset_table)[offset_index] = offset;

      offset_vec = glm::uvec3(offset, offset, offset);
      for (const EntryElement& element : e.elements) {
        hash_index = toIndex(glm::uvec3((element.hash_0.x + offset_vec.x) % m,
                                        (element.hash_0.y + offset_vec.y) % m,
                                        (element.hash_0.z + offset_vec.z) % m), m);
        
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
std::vector<GLushort> SpatialHashFunction<R>::retrieveCandidates(glm::uvec3 p,                        
                                                                 unsigned int dim,     
                                                                 const std::vector<bool>& offset_def,
                                                                 const std::vector<GLushort>& offset_val) {
  std::vector<GLushort> candidates = std::vector<GLushort>();

  unsigned int val[3] = { 0, 1, dim };
  unsigned int index;
  for (int x = 0; x < 3; x++) {
    for (int y = 0; y < 3; y++) {
      for (int z = 0; z < 3; z++) {
        index = this->toIndex(glm::uvec3((p.x + val[x]) % dim,
                                         (p.y + val[y]) % dim,
                                         (p.z + val[z]) % dim), dim);
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
    const std::vector<EntryElement>& elements,
    const std::vector<bool> hash_table_def) {
  glm::uvec3 vec_candidate = glm::uvec3(candidate, candidate, candidate);
  for (EntryElement e : elements) {
    unsigned index = toIndex(glm::uvec3((e.hash_0.x + vec_candidate.x) % m,
                                        (e.hash_0.y + vec_candidate.y) % m,
                                        (e.hash_0.z + vec_candidate.z) % m), m);
    if (index == 48) {
      bool test = true;
    }
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