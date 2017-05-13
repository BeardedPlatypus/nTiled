#include "pipeline\light-management\hashed\HashedConfig.h"

namespace nTiled {
namespace pipeline {
namespace hashed {

HashedConfig::HashedConfig() : HashedConfig(1.0, 3, 1.5, 10) {
}


HashedConfig::HashedConfig(float minimum_node_size,
                           unsigned int starting_depth,
                           float r_increase_ratio,
                           unsigned int max_attempts) :
  minimum_node_size(minimum_node_size),
  starting_depth(starting_depth),
  r_increase_ratio(r_increase_ratio),
  max_attempts(max_attempts),
  seed(22) {
}


HashedConfig::HashedConfig(float minimum_node_size,
                           unsigned int starting_depth,
                           float r_increase_ratio,
                           unsigned int max_attempts,
                           unsigned int seed) :
  minimum_node_size(minimum_node_size),
  starting_depth(starting_depth),
  r_increase_ratio(r_increase_ratio),
  max_attempts(max_attempts),
  seed(seed) {
}

}
}
}