#pragma once

#include <exception>
namespace nTiled {
namespace pipeline {
namespace hashed {

class SLTException : public std::exception {
  virtual const char* what() const throw() {
    return "Single Light Tree Exception";
  }
};


class SLTLightNotInOctreeException : public SLTException {
  virtual const char* what() const throw() {
    return "Light does not fit within the specified light octree";
  }
};


class SLTUnsetNodeAccess : public SLTException {
  virtual const char* what() const throw() {
    return "An unset node has been accessed within a partial node";
  }
};

class SLTLatticeConstructionException : public SLTException {
  virtual const char* what() const throw() {
    return "An error was encountered while building a Lattice";
  }
};


class SLTOutOfLatticeBoundsException : public SLTException {
  virtual const char* what() const throw() {
    return "A lattice has been accessed with a cube that does not intersect.";
  }
};


}
}
}
