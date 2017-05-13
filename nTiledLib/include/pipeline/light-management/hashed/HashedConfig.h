#pragma once

namespace nTiled {
namespace pipeline {
namespace hashed {

struct HashedConfig {
  HashedConfig();
  HashedConfig(float minimum_node_size,
               unsigned int starting_depth,
               float r_increase_ratio,
               unsigned int max_attempts);
  HashedConfig(float minimum_node_size,
               unsigned int starting_depth,
               float r_increase_ratio,
               unsigned int max_attempts,
               unsigned int seed);

  double minimum_node_size;
  unsigned int starting_depth;
  double r_increase_ratio;
  unsigned int max_attempts;
  unsigned int seed;
};

}
}
}