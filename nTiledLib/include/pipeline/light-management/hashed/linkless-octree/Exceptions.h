#pragma once

// ---------------------------------------------------------------------------
//  Libraries
// ---------------------------------------------------------------------------
#include <exception>
namespace nTiled {
namespace pipeline {
namespace hashed{

class SpatialHashFunctionException : public std::exception {
  virtual const char* what() const throw() {
    return "SpatialHashFunction Exception";
  }
};


class SpatialHashFunctionConstructionException : public SpatialHashFunctionException {
  virtual const char* what() const throw() {
    return "SpatialHashFunction has encountered an exception during construction.";
  }
};


class SpatialHashFunctionConstructionInvalidArgException : public SpatialHashFunctionException {
  virtual const char* what() const throw() {
    return "SpatialHashFunction has encountered an exception with an argument.";
  }
};

}
}
}
