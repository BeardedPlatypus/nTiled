#pragma once

// ---------------------------------------------------------------------------
//  Libraries
// ---------------------------------------------------------------------------
#include <rapidjson\writer.h>
#include <rapidjson\stringbuffer.h>


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

  virtual void exportToJson(rapidjson::Writer<rapidjson::StringBuffer>& writer) const = 0;
};


class FullLightNode : public Node {
public:
  FullLightNode();
  ~FullLightNode() override;

  virtual void exportToJson(rapidjson::Writer<rapidjson::StringBuffer>& writer) const override;
};


class NoLightNode : public Node {
public:
  NoLightNode();
  ~NoLightNode() override;

  virtual void exportToJson(rapidjson::Writer<rapidjson::StringBuffer>& writer) const override;
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
  ~PartialLightNode() override;

  virtual void exportToJson(rapidjson::Writer<rapidjson::StringBuffer>& writer) const override;

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