#include <catch.hpp>
#include "pipeline\light-management\hashed\light-octree\nodes\LOLeaf.h"
#include "pipeline\light-management\hashed\light-octree\nodes\LOBranch.h"
#include "pipeline\light-management\hashed\light-octree\slt\nodes\SLTLeaf.h"


SCENARIO("The subdivide method should return the appropriate subdivided LOLeaf when provided with correct data",
         "[LightOctreeFull][LightOctree][LOLeaf]") {
  GIVEN("A set of LOLeafs") {
    std::vector<nTiled::pipeline::hashed::LOLeaf*> leaves = {};
    std::vector<nTiled::pipeline::hashed::LOBranch*> parents = {};
    std::vector<unsigned int> n_indices = {};
      
    nTiled::pipeline::hashed::LOLeaf* p_leaf;
    nTiled::pipeline::hashed::LOBranch* p_parent;
    for (unsigned int i = 1; i <= 30; ++i) {
      p_leaf = new nTiled::pipeline::hashed::LOLeaf();
      p_parent = new nTiled::pipeline::hashed::LOBranch();

      for (unsigned int j = 0; j < 5 * i; ++j) {
        p_leaf->addIndex(j);
      }
      leaves.push_back(p_leaf);
      parents.push_back(p_parent);
      n_indices.push_back(p_leaf->getIndices().size());
    }

    WHEN("A LOLeaf is subdivided") {
      std::vector<nTiled::pipeline::hashed::LOBranch*> subdivided_leaves = {};
      
      for (unsigned int i = 0; i < 30; ++i) {
        subdivided_leaves.push_back(leaves.at(i)->subdivide(parents.at(i), glm::bvec3(false)));
      }

      THEN("The parent should be equal to the specified parent, and each new node should contain the same values as the subvided child.") {
        nTiled::pipeline::hashed::LOBranch* p_subd;
        nTiled::pipeline::hashed::LOLeaf* p_new_node;
        nTiled::pipeline::hashed::LOBranch* p_parent;
        for (unsigned int i = 0; i < 30; ++i) {
          p_subd = subdivided_leaves.at(i);
          p_parent = parents.at(i);

          REQUIRE(p_parent->getChildNode(glm::bvec3(false)) == p_subd);

          for (unsigned int x = 0; x < 2; x++) {
            for (unsigned int y = 0; y < 2; y++) {
              for (unsigned int z = 0; z < 2; z++) {
                std::vector<GLuint> leaf_indices = leaves.at(i)->getIndices();
                std::vector<GLuint> copy_indices = p_subd->retrieveLights(glm::vec3(0.25 + x * 0.5,
                                                                                    0.25 + y * 0.5,
                                                                                    0.25 + z * 0.5),
                                                                          nTiled::pipeline::hashed::NodeDimensions(glm::vec3(0.0),
                                                                                                                   1.0));

                REQUIRE(copy_indices.size() == leaf_indices.size());
                
                for (unsigned int j = 0; j < copy_indices.size(); ++j) {
                  REQUIRE(std::find(leaf_indices.begin(), 
                                    leaf_indices.end(), 
                                    copy_indices.at(j)) != leaf_indices.end());
                }
              }
            }
          }
        }
      }

      AND_WHEN("The subdivided nodes are modified") {
        for (nTiled::pipeline::hashed::LOBranch* p : subdivided_leaves) {
          unsigned int val = 600;
          for (unsigned int x = 0; x < 2; x++) {
            for (unsigned int y = 0; y < 2; y++) {
              for (unsigned int z = 0; z < 2; z++) {
                glm::bvec3 index = glm::bvec3(x == 1,
                                              y == 1,
                                              z == 1);
                p->getChildNode(index)->addSLTNode(nTiled::pipeline::hashed::SLTLeaf(true), p, index, val);
                val++;
              }
            }
          }
        }

        THEN("only that node is modified.") {
          nTiled::pipeline::hashed::LOBranch* p_subd;
          nTiled::pipeline::hashed::LOLeaf* p_new_node;
          nTiled::pipeline::hashed::LOBranch* p_parent;

          for (unsigned int i = 0; i < 30; ++i) {
            p_subd = subdivided_leaves.at(i);
            p_parent = parents.at(i);
            unsigned int val = 600;
            
            for (unsigned int x = 0; x < 2; x++) {
              for (unsigned int y = 0; y < 2; y++) {
                for (unsigned int z = 0; z < 2; z++) {
                  glm::bvec3 index = glm::bvec3(x == 1,
                                                y == 1,
                                                z == 1);
                  
                  std::vector<GLuint> copy_indices = p_subd->retrieveLights(glm::vec3(0.25 + x * 0.5,
                                                                                      0.25 + y * 0.5,
                                                                                      0.25 + z * 0.5),
                                                                            nTiled::pipeline::hashed::NodeDimensions(glm::vec3(0.0),
                                                                                                                   1.0));
                  
                  REQUIRE(copy_indices.size() == n_indices.at(i) + 1);         
                  REQUIRE(std::find(copy_indices.begin(),
                                    copy_indices.end(),
                                    val) != copy_indices.end());
                  val++;
                }
              }
            }
          }
        }
      }
    }
  }
}
