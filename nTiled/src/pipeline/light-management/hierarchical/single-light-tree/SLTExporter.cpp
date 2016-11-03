#include "pipeline\light-management\hierarchical\single-light-tree\SLTExporter.h"

// ---------------------------------------------------------------------------
//  Libraries
// ---------------------------------------------------------------------------
#include <rapidjson\writer.h>
#include <rapidjson\stringbuffer.h>
#include <fstream>

namespace nTiled {
namespace pipeline {
namespace hierarchical {

void exportSLT(const std::string& path,
               const SingleLightTree& tree) {
  rapidjson::StringBuffer s;
  rapidjson::Writer<rapidjson::StringBuffer> writer(s);

  std::ofstream output_stream;
  output_stream.open(path);
  output_stream << s.GetString();
  output_stream.close();
}

}
}
}