#include <catch.hpp>
#include "pipeline\light-management\hashed\light-octree\nodes\LOLeaf.h"


SCENARIO("The constructor should return the appropriate LOLeaf when provided with correct data",
         "[LightOctreeFull][LightOctree][LOLeaf]") {
  GIVEN("") {
    WHEN("An empty LOLeaf node is created") {
      nTiled::pipeline::hashed::LOLeaf leaf =
        nTiled::pipeline::hashed::LOLeaf();

      THEN("It should not contain any light indices") {
        REQUIRE(leaf.isEmpty());
        REQUIRE(leaf.getIndices().empty());
      }
    }
  }

  GIVEN("A set of LOLeaves") {
    std::vector<nTiled::pipeline::hashed::LOLeaf*> leaves = {};

    nTiled::pipeline::hashed::LOLeaf* p_leaf;
    for (unsigned int i = 1; i <= 30; ++i) {
      p_leaf = new nTiled::pipeline::hashed::LOLeaf();
      for (unsigned int j = 0; j < 5 * i; ++j) {
        p_leaf->addIndex(j);
      }
      leaves.push_back(p_leaf);
    }

    WHEN("A copy of LOLeaf node is created") {
      std::vector<nTiled::pipeline::hashed::LOLeaf*> copy_of_leaves = {};
      
      for (nTiled::pipeline::hashed::LOLeaf* p_leaf : leaves) {
        copy_of_leaves.push_back(new nTiled::pipeline::hashed::LOLeaf(*p_leaf));
      }

      THEN("It should contain the exact same light indices as the leaf") {
        for (unsigned int i = 0; i < 30; ++i) {
          std::vector<GLuint> leaf_indices = leaves.at(i)->getIndices();
          std::vector<GLuint> copy_indices = copy_of_leaves.at(i)->getIndices();

          REQUIRE(copy_of_leaves.at(i)->isEmpty() == leaves.at(i)->isEmpty());
          REQUIRE(copy_indices.size() == leaf_indices.size());

          for (unsigned int j = 0; j < copy_indices.size(); ++j) {
            REQUIRE(std::find(leaf_indices.begin(), 
                              leaf_indices.end(), 
                              copy_indices.at(j)) != leaf_indices.end());
          }
        }
      }
      
      for (nTiled::pipeline::hashed::LOLeaf* p_leaf : copy_of_leaves)
        delete p_leaf;
    }

    for (nTiled::pipeline::hashed::LOLeaf* p_leaf : leaves)
      delete p_leaf;
  }
}
