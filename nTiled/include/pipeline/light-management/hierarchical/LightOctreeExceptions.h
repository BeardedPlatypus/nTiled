#pragma once

#include <string>
#include <stdexcept>


namespace nTiled {
namespace pipeline {
namespace hierarchical {
namespace lo {

class UnsupportedMethodException : public std::runtime_error {
public:
  UnsupportedMethodException() : runtime_error("This method is not supported.") {}
  UnsupportedMethodException(std::string msg) : runtime_error(msg.c_str()) {}
};

}
}
}
}

