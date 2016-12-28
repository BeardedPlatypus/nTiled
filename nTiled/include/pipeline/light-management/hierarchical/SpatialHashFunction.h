#pragma once

#include <vector>
#include <glad\glad.h>
#include <glm\glm.hpp>

namespace nTiled {
namespace pipeline {
namespace hierarchical {



template <class R>
class SpatialHashFunction {
  struct EntryElement {
    EntryElement(glm::uvec3 point, glm::uvec3 hash_0, R data);
    
    glm::uvec3 point;
    glm::uvec3 hash_0;
    R data;
  };

  struct ConstructionElement {
    ConstructionElement();
    ConstructionElement(glm::uvec3 hash_1);

    glm::uvec3 hash_1;
    std::vector<EntryElement> elements;
  };

  struct ConstructionElementCompare {
    bool operator() (ConstructionElement i, ConstructionElement j) { return (i.elements.size() < j.elements.size() ); }
  } compare_struct;

public:
  // --------------------------------------------------------------------------
  //  Constructor | Destructor
  SpatialHashFunction(const std::vector<std::pair<glm::uvec3, R>>& entries,
                      unsigned int max_attempts,
                      float ratio);
  ~SpatialHashFunction();

  // --------------------------------------------------------------------------
  //  Getters
  const std::vector<R>& getHashTable() const { return *(this->p_hash_table); }
  const std::vector<GLushort>& getOffsetTable() const { return *(this->p_offset_table); }

  inline unsigned int getM() const { return this->m_dim; }
  inline unsigned int getR() const { return this->r_dim; }

private:
  // -------------------------------------------------------------------------
  //  buildTable
  bool buildTables(unsigned int m,
                   unsigned int r,
                   std::vector<std::pair<glm::uvec3, R>> entries);

  // --------------------------------------------------------------------------
  //  supporting functions
  bool isAcceptableParameters(unsigned int m, unsigned int r);

  inline unsigned int toIndex(glm::uvec3 p, unsigned int dim) { 
    return p.x + p.y * dim + p.z * dim * dim; 
  }
  
  std::vector<GLushort>retrieveCandidates(glm::uvec3 p,
                                          unsigned int dim,
                                          const std::vector<bool>& offset_def,
                                          const std::vector<GLushort>& offset_val);

  bool isValidCandidate(GLushort candidate,
                        unsigned int m,
                        const std::vector<EntryElement>& elements,
                        const std::vector<bool> hash_table_def);

  // --------------------------------------------------------------------------
  //  properties
  unsigned int m_dim;
  std::vector<R>* p_hash_table;

  unsigned int r_dim;
  std::vector<GLushort>* p_offset_table;
};

}
}
}
