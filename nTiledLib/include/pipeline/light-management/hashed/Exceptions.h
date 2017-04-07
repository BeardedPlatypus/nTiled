#pragma once

#include <exception>
namespace nTiled {
namespace pipeline {
namespace hashed {

class HashedShadingException : public std::exception {
  virtual const char* what() const throw() {
    return "Hashed Shading Exception";
  }
};


class HashedShadingNoLightException : public HashedShadingException {
  virtual const char* what() const throw() {
    return "No light contained in the world.";
  }
};


class HashedShadingInvalidStartingDepthException : public HashedShadingException {
  virtual const char* what() const throw() {
    return "The starting_depth paramater exceeds the actual depth of the octree.";
  }
};

}
}
}