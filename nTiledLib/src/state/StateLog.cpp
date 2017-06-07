#include "state\StateLog.h"


namespace nTiled {
namespace state {

Log::Log(bool is_logging_data,
         bool is_counting_calculations,
         std::string path,
         std::string path_calculations,
         unsigned int frame_start, 
         unsigned int frame_end,
         bool exit_after_done,
         unsigned int exit_frame) :
  is_logging_data(is_logging_data),
  is_counting_calculations(is_counting_calculations),
  path(path),
  path_calculations(path_calculations),
  frame_start(frame_start), 
  frame_end(frame_end),
  exit_after_done(exit_after_done),
  exit_frame(exit_frame) {
}


Log::Log(std::string path, unsigned int frame_start, unsigned int frame_end) :
  Log(true, false, path, std::string(""), frame_start, frame_end, false, 0) {
}

Log::Log() :
  Log(false, false, std::string(""), std::string(""), 0, 0, false, 0) {
}

}
}