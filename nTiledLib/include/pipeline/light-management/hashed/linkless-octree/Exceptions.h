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


class SpatialHashFunctionConstructionExhaustedException : public SpatialHashFunctionException {
  virtual const char* what() const throw() {
    return "SpatialHashFunction has exceeded the number of tries to construct it.";
  }
};


class SpatialHashFunctionConstructionIllegalAccessTableException : public SpatialHashFunctionException {
  virtual const char* what() const throw() {
    return "Table point accessed an undefined point";
  }
};


class SpatialHashFunctionConstructionIllegalWriteTableException : public SpatialHashFunctionException {
  virtual const char* what() const throw() {
    return "Table point written a defined point";
  }
};


}
}
}
