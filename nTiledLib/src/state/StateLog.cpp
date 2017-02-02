#include "state\StateLog.h"


namespace nTiled {
namespace state {

Log::Log(bool is_logging_data,
         std::string path,
         unsigned int frame_start, 
         unsigned int frame_end,
         bool exit_after_done,
         unsigned int exit_frame) :
  is_logging_data(is_logging_data),
  path(path),
  frame_start(frame_start), 
  frame_end(frame_end),
  exit_after_done(exit_after_done),
  exit_frame(exit_frame) {
}


Log::Log(std::string path, unsigned int frame_start, unsigned int frame_end) :
  Log(true, path, frame_start, frame_end, false, 0) {
}

Log::Log() :
  Log(false, std::string(""), 0, 0, false, 0) {
}

}
}