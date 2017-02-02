#include "pipeline\light-management\hashed\light-octree\single-light-tree\octree-layer\OctreeLayer.h"


#include <rapidjson\writer.h>
#include <rapidjson\stringbuffer.h>
#include <fstream>


namespace nTiled {
namespace pipeline {
namespace hashed {
namespace slt {


// ----------------------------------------------------------------------------
//  Constructors
// ----------------------------------------------------------------------------
OctreeLayer::OctreeLayer(const glm::uvec3 origin_in_lattice,
                         const unsigned int n_nodes,
                         const int depth,
                         const float node_size) :
  origin_in_lattice(origin_in_lattice),
  n_nodes(n_nodes),
  depth(depth),
  node_size(node_size),
  unique_nodes(std::vector<OctreeLayerNode*>()),
  layer_nodes(std::vector<OctreeLayerNode*>(n_nodes * n_nodes * n_nodes)) {
}


OctreeLayer::OctreeLayer(const glm::uvec3 origin_in_lattice,
                         const unsigned int n_nodes,
                         const int depth,
                         const float node_size,
                         OctreeLayerNode* p_node) :
  origin_in_lattice(origin_in_lattice),
  n_nodes(n_nodes),
  depth(depth),
  node_size(node_size),
  unique_nodes(std::vector<OctreeLayerNode*>({ p_node })),
  layer_nodes(std::vector<OctreeLayerNode*>((n_nodes * n_nodes * n_nodes), p_node)) {
}



OctreeLayer::OctreeLayer(const glm::uvec3 origin_in_lattice,
                         const unsigned int n_nodes,
                         const int depth,
                         const float node_size,
                         const No& no_light) :
  OctreeLayer(origin_in_lattice,
              n_nodes,
              depth,
              node_size,
              new OctreeLayerNode(&no_light)) {
}


OctreeLayer::OctreeLayer(const glm::uvec3 origin_in_grid,
                         const unsigned int n_nodes,
                         const int depth,
                         const float node_size,
                         const Full& full_light) :
  OctreeLayer(origin_in_grid,
              n_nodes,
              depth,
              node_size,
              new OctreeLayerNode(&full_light)) {
}


// ----------------------------------------------------------------------------
//  Destructor
// ----------------------------------------------------------------------------
OctreeLayer::~OctreeLayer() {
  for (OctreeLayerNode* p_node : this->unique_nodes) {
    delete p_node;
  }
}


// ----------------------------------------------------------------------------
//  Get || Set methods
// ----------------------------------------------------------------------------
const OctreeLayerNode& OctreeLayer::getOctreeLayerNode(glm::uvec3 position) const {
  return *(this->layer_nodes[this->getIndexNode(position)]);
}


void OctreeLayer::setOctreeLayerNode(glm::uvec3 position,
                                     OctreeLayerNode* p_node) {
  this->unique_nodes.push_back(p_node);
  this->layer_nodes[this->getIndexNode(position)] = p_node;
}


void OctreeLayer::setOctreeLayerNode(glm::uvec3 position,
                             const OctreeLayerNode& node) {
  this->setOctreeLayerNode(position, new OctreeLayerNode(node));
}


void OctreeLayer::setOctreeLayerNode(glm::uvec3 position, const No& no_light) {
  this->setOctreeLayerNode(position, new OctreeLayerNode(&no_light));
}


void OctreeLayer::setOctreeLayerNode(glm::uvec3 position, const Full& full_light) {
  this->setOctreeLayerNode(position, new OctreeLayerNode(&full_light));
}


void OctreeLayer::setOctreeLayerNode(glm::uvec3 position, const Partial& partial_light) {
  this->setOctreeLayerNode(position, new OctreeLayerNode(&partial_light));
}


int OctreeLayer::getIndexNode(glm::uvec3 position) const {
  return (position.x
          + position.y * this->getNNodes()
          + position.z * this->getNNodes() * this->getNNodes()
          );
}


void OctreeLayer::exportToJson(const std::string& path, glm::vec3 octree_origin) const {
  rapidjson::StringBuffer s;
  rapidjson::Writer<rapidjson::StringBuffer> writer(s);

  writer.StartObject();

  writer.Key("node_size");
  writer.Double(this->getNodeSize());

  writer.Key("depth");
  writer.Uint(this->getDepth());

  writer.Key("origin_in_lattice");
  writer.StartObject();
  glm::uvec3 position = this->getOriginInLattice();
  writer.Key("x");
  writer.Double(position.x);
  writer.Key("y");
  writer.Double(position.y);
  writer.Key("z");
  writer.Double(position.z);
  writer.EndObject();

  writer.Key("octree_origin");
  writer.StartObject();
  writer.Key("x");
  writer.Double(octree_origin.x);
  writer.Key("y");
  writer.Double(octree_origin.y);
  writer.Key("z");
  writer.Double(octree_origin.z);
  writer.EndObject();

  writer.Key("nodes");
  writer.StartArray();

  for (unsigned int z = 0; z < this->getNNodes(); z++) {
    for (unsigned int y = 0; y < this->getNNodes(); y++) {
      for (unsigned int x = 0; x < this->getNNodes(); x++) {
        glm::uvec3 pos = glm::uvec3(x, y, z);
        this->layer_nodes[this->getIndexNode(pos)]->node->exportToJson(writer, pos);
      }
    }
  }
  
  writer.EndArray();

  writer.EndObject();

  std::ofstream output_stream;
  output_stream.open(path);
  output_stream << s.GetString();
  output_stream.close();
}


} // slt
} // hashed
} // pipeline
} // nTiled
