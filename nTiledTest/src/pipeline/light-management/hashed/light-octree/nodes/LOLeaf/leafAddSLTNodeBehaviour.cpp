#include <catch.hpp>
#include "pipeline\light-management\hashed\light-octree\nodes\LOLeaf.h"
#include "pipeline\light-management\hashed\light-octree\nodes\LOBranch.h"

#include "pipeline\light-management\hashed\light-octree\slt\nodes\SLTLeaf.h"
#include "pipeline\light-management\hashed\light-octree\slt\nodes\SLTBranch.h"


SCENARIO("When a SLTNode is added to a LOLeaf it should be updated accordingly.",
         "[LightOctreeFull][LightOctree][LOLeaf]") {
  GIVEN("A set of LOLeaf nodes with parents, and per LOLeaf a set of empty SLTLeaves and indices.") {
    std::vector<nTiled::pipeline::hashed::LOLeaf*> leaves = {};
    std::vector<nTiled::pipeline::hashed::LOBranch*> parents = {};
    std::vector<unsigned int> n_indices = {};
    std::vector<std::vector<GLuint>> current_indices = {};
      
    nTiled::pipeline::hashed::LOLeaf* p_leaf;
    nTiled::pipeline::hashed::LOBranch* p_parent;
    for (unsigned int i = 1; i <= 30; ++i) {
      p_leaf = new nTiled::pipeline::hashed::LOLeaf();
      p_parent = new nTiled::pipeline::hashed::LOBranch();

      for (unsigned int j = 0; j < 5 * i; ++j) {
        p_leaf->addIndex(j);
      }

      p_parent->updateChild(glm::bvec3(false), p_leaf);

      leaves.push_back(p_leaf);
      parents.push_back(p_parent);
      current_indices.push_back(p_leaf->getIndices());
      n_indices.push_back(p_leaf->getIndices().size());
    }

    std::vector<std::vector<GLuint>> additional_indices = {};
    for (unsigned int i = 0; i < 30; ++i) {
      std::vector<GLuint> extra_indices = {};
      GLuint offset = leaves.at(i)->getIndices().size();
      for (unsigned int j = 0; j < (1 + i) * 5; ++j) {
        extra_indices.push_back(offset + j);
      }

      additional_indices.push_back(extra_indices);
    }

    WHEN("These SLTnodes are added to the leaves") {
      nTiled::pipeline::hashed::LOLeaf* p_leaf;
      for (unsigned int i = 0; i < 30; ++i) {
        p_leaf = leaves.at(i);

        for (GLuint val : additional_indices.at(i)) {
          p_leaf->addSLTNode(nTiled::pipeline::hashed::SLTLeaf(false), parents.at(i), glm::bvec3(false), val);
        }
      }

      THEN("The LOLeaf nodes should contain the original indices and the SLTLeaf indices.") {
        std::vector<GLuint> c;
        for (unsigned int i = 0; i < 30; ++i) {
          p_leaf = leaves.at(i);
          c = p_leaf->getIndices();

          // check if sizes correpsond
          REQUIRE(c.size() == n_indices.at(i));
          // check if each node in previous indices and new indices is inside the new indices
          for (GLuint v : current_indices.at(i)) {
            REQUIRE(std::find(c.begin(), c.end(), v) != c.end());
          }
        }
      }
    }
    for (nTiled::pipeline::hashed::LOBranch* p : parents) delete p;
  }

  GIVEN("A set of LOLeaf nodes with parents, and per LOLeaf a set of non empty SLTLeaves and indices.") {
    std::vector<nTiled::pipeline::hashed::LOLeaf*> leaves = {};
    std::vector<nTiled::pipeline::hashed::LOBranch*> parents = {};
    std::vector<unsigned int> n_indices = {};
    std::vector<std::vector<GLuint>> current_indices = {};
      
    nTiled::pipeline::hashed::LOLeaf* p_leaf;
    nTiled::pipeline::hashed::LOBranch* p_parent;
    for (unsigned int i = 1; i <= 30; ++i) {
      p_leaf = new nTiled::pipeline::hashed::LOLeaf();
      p_parent = new nTiled::pipeline::hashed::LOBranch();

      for (unsigned int j = 0; j < 5 * i; ++j) {
        p_leaf->addIndex(j);
      }

      p_parent->updateChild(glm::bvec3(false), p_leaf);

      leaves.push_back(p_leaf);
      parents.push_back(p_parent);
      current_indices.push_back(p_leaf->getIndices());
      n_indices.push_back(p_leaf->getIndices().size());
    }

    std::vector<std::vector<GLuint>> additional_indices = {};
    for (unsigned int i = 0; i < 30; ++i) {
      std::vector<GLuint> extra_indices = {};
      GLuint offset = leaves.at(i)->getIndices().size();
      for (unsigned int j = 0; j < (1 + i) * 5; ++j) {
        extra_indices.push_back(offset + j);
      }

      additional_indices.push_back(extra_indices);
    }

    WHEN("These SLTnodes are added to the leaves") {
      nTiled::pipeline::hashed::LOLeaf* p_leaf;
      for (unsigned int i = 0; i < 30; ++i) {
        p_leaf = leaves.at(i);

        for (GLuint val : additional_indices.at(i)) {
          p_leaf->addSLTNode(nTiled::pipeline::hashed::SLTLeaf(true), parents.at(i), glm::bvec3(false), val);
        }
      }

      THEN("The LOLeaf nodes should contain the original indices and the SLTLeaf indices.") {
        std::vector<GLuint> c;
        for (unsigned int i = 0; i < 30; ++i) {
          p_leaf = leaves.at(i);
          c = p_leaf->getIndices();

          // check if sizes correpsond
          REQUIRE(c.size() == n_indices.at(i) + additional_indices.at(i).size());
          // check if each node in previous indices and new indices is inside the new indices
          for (GLuint v : current_indices.at(i)) {
            REQUIRE(std::find(c.begin(), c.end(), v) != c.end());
          }

          for (GLuint v : additional_indices.at(i)) {
            REQUIRE(std::find(c.begin(), c.end(), v) != c.end());
          }
        }
      }
    }

    for (nTiled::pipeline::hashed::LOBranch* p : parents) delete p;
  }

  GIVEN("A set of LOLeaf nodes with parents, and per LOLeaf a set of SLTBranches and indices.") {
    std::vector<nTiled::pipeline::hashed::LOLeaf*> leaves = {};
    std::vector<nTiled::pipeline::hashed::LOBranch*> parents = {};
    std::vector<unsigned int> n_indices = {};
    std::vector<std::vector<GLuint>> current_indices = {};
      
    nTiled::pipeline::hashed::LOLeaf* p_leaf;
    nTiled::pipeline::hashed::LOBranch* p_parent;
    for (unsigned int i = 1; i <= 30; ++i) {
      p_leaf = new nTiled::pipeline::hashed::LOLeaf();
      p_parent = new nTiled::pipeline::hashed::LOBranch();

      for (unsigned int j = 0; j < 5 * i; ++j) {
        p_leaf->addIndex(j);
      }

      p_parent->updateChild(glm::bvec3(false), p_leaf);

      leaves.push_back(p_leaf);
      parents.push_back(p_parent);
      current_indices.push_back(p_leaf->getIndices());
      n_indices.push_back(p_leaf->getIndices().size());
    }

    std::vector<std::vector<GLuint>> additional_indices = {};
    std::vector<std::vector<nTiled::pipeline::hashed::SLTBranch*>> slt_branches = {};

    nTiled::pipeline::hashed::SLTBranch* b;
    for (unsigned int i = 0; i < 30; ++i) {
      std::vector<GLuint> extra_indices = {};
      std::vector<nTiled::pipeline::hashed::SLTBranch*> br = {};

      GLuint offset = leaves.at(i)->getIndices().size();
      for (unsigned int j = 0; j < (1 + i) * 5; ++j) {
        extra_indices.push_back(offset + j);

        b = new nTiled::pipeline::hashed::SLTBranch();
        b->setNode(glm::bvec3(false, false, false),
                  new nTiled::pipeline::hashed::SLTLeaf(true));
        b->setNode(glm::bvec3( true, false, false),
                  new nTiled::pipeline::hashed::SLTLeaf(false));
        b->setNode(glm::bvec3(false,  true, false),
                  new nTiled::pipeline::hashed::SLTLeaf(false));
        b->setNode(glm::bvec3( true,  true, false),
                  new nTiled::pipeline::hashed::SLTLeaf(false));
        b->setNode(glm::bvec3(false, false, true),
                  new nTiled::pipeline::hashed::SLTLeaf(false));
        b->setNode(glm::bvec3( true, false, true),
                  new nTiled::pipeline::hashed::SLTLeaf(false));
        b->setNode(glm::bvec3(false,  true, true),
                  new nTiled::pipeline::hashed::SLTLeaf(false));
        b->setNode(glm::bvec3( true,  true, true),
                  new nTiled::pipeline::hashed::SLTLeaf(true));
        br.push_back(b);
      }

      additional_indices.push_back(extra_indices);
      slt_branches.push_back(br);
    }

    WHEN("These SLTnodes are added to the leaves") {
      for (unsigned int i = 0; i < 30; ++i) {
        for (unsigned int j = 0; j < (1 + i) * 5; ++j) {
          parents.at(i)->getChildNode(glm::bvec3(false))->addSLTNode(*(slt_branches.at(i).at(j)), parents.at(i), glm::bvec3(false), additional_indices.at(i).at(j));
        }
      }

      THEN("The LOLeaf nodes should contain the original indices and the SLTLeaf indices.") {
        std::vector<GLuint> c;

        nTiled::pipeline::hashed::LONode* node;
        for (unsigned int i = 0; i < 30; ++i) {
          node = parents.at(i)->getChildNode(glm::bvec3(false));

          unsigned int c_size;
          unsigned int f_size;

          for (unsigned int x = 0; x < 2; ++x) {
            for (unsigned int y = 0; y < 2; ++y) {
              for (unsigned int z = 0; z < 2; ++z) {
                glm::bvec3 index = glm::bvec3(x == 1,
                                              y == 1,
                                              z == 1);

                c = node->retrieveLights(glm::vec3((0.25 + 0.5 * x),
                                                   (0.25 + 0.5 * y),
                                                   (0.25 + 0.5 * z)),
                                         nTiled::pipeline::hashed::NodeDimensions(glm::vec3(0.0), 1.0));
                c_size = c.size();

                if (( index.x &&  index.y &&  index.z) ||
                    (!index.x && !index.y && !index.z)) {
                  f_size = n_indices.at(i) + additional_indices.at(i).size();
                  REQUIRE(c_size == f_size);
                  
                  for (GLuint v : current_indices.at(i)) {
                    REQUIRE(std::find(c.begin(), c.end(), v) != c.end());
                  }
                  
                  for (GLuint v : additional_indices.at(i)) {
                    REQUIRE(std::find(c.begin(), c.end(), v) != c.end());
                  }
                } else {
                  f_size = n_indices.at(i);
                  REQUIRE(c_size == f_size);
                  
                  for (GLuint v : current_indices.at(i)) {
                    REQUIRE(std::find(c.begin(), c.end(), v) != c.end());
                  }
                }
              }
            }
          }
        }
      }
    }  

    for (nTiled::pipeline::hashed::LOBranch* p : parents) delete p;

    for (const std::vector<nTiled::pipeline::hashed::SLTBranch*>& l : slt_branches)
      for (nTiled::pipeline::hashed::SLTBranch* p : l)
        delete p;
  }
}
