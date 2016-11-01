#pragma once

namespace nTiled {
namespace pipeline {
namespace hierarchical {
namespace slt {


class Node {
public:
  // --------------------------------------------------------------------------
  //  Constructor
  // --------------------------------------------------------------------------
  virtual ~Node() = 0;
};


class FullLightNode : public Node {
public:
  FullLightNode();
  ~FullLightNode();
};


class NoLightNode : public Node {
public:
  NoLightNode();
  ~NoLightNode();
};


class PartialLightNode : public Node {
public:
  //                                 xyz
  PartialLightNode(const Node& child_000,
                   const Node& child_100,
                   const Node& child_010,
                   const Node& child_110,
                   const Node& child_001,
                   const Node& child_101,
                   const Node& child_011,
                   const Node& child_111);

  PartialLightNode(Node const * const * children);

private:
  //                xyz
  const Node& child_000;
  const Node& child_100;
  const Node& child_010;
  const Node& child_110;
  const Node& child_001;
  const Node& child_101;
  const Node& child_011;
  const Node& child_111;
};


} // slt
} // hierarchical
} // pipeline
} // nTiled