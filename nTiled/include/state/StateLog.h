#pragma once

#include <string>

namespace nTiled {
namespace state {

struct Log {
  Log(bool is_logging_data,
      std::string path);
  Log(std::string path);
  Log();

  bool is_logging_data;
  std::string path;
};

}
}
