#include "pipeline\light-management\hierarchical\light-octree\Branch.h"

// ----------------------------------------------------------------------------
//  nTiled Headers
// ----------------------------------------------------------------------------
#include "pipeline\light-management\hierarchical\light-octree\Leaf.h"

namespace nTiled {
namespace pipeline {
namespace hierarchical {
namespace lo {

// ----------------------------------------------------------------------------
//  Constructor Methods
// ----------------------------------------------------------------------------
Branch::Branch(Parent* p_parent, unsigned char index, Leaf* leaf_node) : Node(p_parent,
                                                                              index) {
  this->p_parent->updateChild(this);

  this->sub_nodes[leaf_node->getIndex()] = leaf_node;
  leaf_node->setParent(this);

  for (unsigned char i = 0; i < leaf_node->getIndex(); i++) {
    this->sub_nodes[i] = new Leaf(this, i, leaf_node->getLightIndices());
  }

  for (unsigned char i = leaf_node->getIndex() + 1; i < 8; i++) {
    this->sub_nodes[i] = new Leaf(this, i, leaf_node->getLightIndices());
  }
}


Branch::Branch(Parent* p_parent, unsigned char index) : Node(p_parent, 
                                                             index) {
  for (unsigned char i = 0; i < 8; i++) {
    this->sub_nodes[i] = new Leaf(this, i);
  }
}


// ----------------------------------------------------------------------------
// Destructor Methods
// ----------------------------------------------------------------------------
Branch::~Branch() {
  for (int i = 0; i < 8; i++) {
    delete this->sub_nodes[i];
  }
}

// ----------------------------------------------------------------------------
//  Interaction methods
// ----------------------------------------------------------------------------
void Branch::addSLTInternal(const SingleLightTree& slt, 
                            unsigned int depth, 
                            glm::uvec3 current_middle) {
  glm::uvec3 slt_middle = slt.getMiddleInOctree();
  glm::bvec3 key = glm::bvec3(current_middle.x < slt_middle.x,
                              current_middle.y < slt_middle.y,
                              current_middle.z < slt_middle.z);
  unsigned int offset = 1 << (depth - 1);
  // TODO check if this conversion goes alright
  glm::uvec3 new_middle = current_middle + (glm::uvec3(offset) * glm::uvec3(key));
  this->getChild(this->getIndexChild(key))->addSLT(slt, depth - 1, new_middle);
}


void Branch::addSLTNode(const SingleLightTree& slt,
                        const slt::Node& node) {
  node.addToOctreeNode(this, slt);
}


void Branch::addPartialNode(const SingleLightTree& slt,
                            const slt::PartialLightNode& partial_node) {
  this->sub_nodes[0]->addSLTNode(slt, partial_node.child_000);
  this->sub_nodes[1]->addSLTNode(slt, partial_node.child_100);
  this->sub_nodes[2]->addSLTNode(slt, partial_node.child_010);
  this->sub_nodes[3]->addSLTNode(slt, partial_node.child_110);
  this->sub_nodes[4]->addSLTNode(slt, partial_node.child_001);
  this->sub_nodes[5]->addSLTNode(slt, partial_node.child_101);
  this->sub_nodes[6]->addSLTNode(slt, partial_node.child_011);
  this->sub_nodes[7]->addSLTNode(slt, partial_node.child_111);
}


void Branch::addLight(GLuint light_index) {
  for (int i = 0; i < 8; i++) {
    this->sub_nodes[i]->addLight(light_index);
  }
}


int Branch::getIndexChild(glm::bvec3 key) {
  int index = 0;

  if (key.x) index += 1;
  if (key.y) index += 2;
  if (key.z) index += 4;

  return index;
}


void Branch::updateChild(Node* child) {
  this->sub_nodes[child->getIndex()] = child;
}


Node* Branch::getChild(int index) {
  return this->sub_nodes[index];
}


void Branch::exportToJson(rapidjson::Writer<rapidjson::StringBuffer>& writer) const {
  writer.StartObject();
    writer.Key("type");
    writer.String("branch");

    writer.Key("sub-nodes");
    writer.StartArray();
    
    for (Node* p : this->sub_nodes) {
      p->exportToJson(writer);
    }

    writer.EndArray();
  writer.EndObject();
}

} // lo
} // hierarchical
} // pipeline
} // nTiled
