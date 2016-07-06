#include "state\StateLog.h"


namespace nTiled {
namespace state {

Log::Log(bool is_logging_data,
         std::string path) :
  is_logging_data(is_logging_data),
  path(path) {
}


Log::Log(std::string path) :
  Log(true, path) {
}

Log::Log() :
  Log(false, std::string("")) {
}

}
}