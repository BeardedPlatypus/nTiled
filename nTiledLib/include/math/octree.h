/*! @file octree.h
 * @brief octree.h contains the utility math functions to convert different 
 *        attributes of an octree in different other aspects.
 * 
 * The following attributes are defined
 */
#pragma once

#include <cmath>
#include "util.h"

namespace nTiled {
namespace math {

// ----------------------------------------------------------------------------
/*! @brief Calculate the number of nodes at single dimension given a depth
 *
 * @param depth the depth at which we calculate the number of nodes
 * @returns the number of nodes in a single dimension
 */
inline unsigned int calculateNNodes(unsigned int depth) {
  return (1 << depth);
}


/*! @brief Calculate the depth given the number of nodes that reside at that 
 *         depth.

 *  in case the n_nodes do not correspond with the number of nodes at a depth
 *  the depth which contains at least as many nodes is returned
 * 
 * @param n_nodes Number of nodes at the depth
 * @returns the depth that contains at least as many nodes as specified
 */
inline unsigned int calculateDepth(unsigned int n_nodes) {
  unsigned int(log2(getNextPow2(n_nodes)));
}


// ----------------------------------------------------------------------------
/*! @brief Calculate the nminimum node size given the maximum_node_size at 
 *         depth 0 and the depth at which the minimum nodes reside.
 *
 * @param depth The depth difference between the nodes equalling 
 *              maxmimum_node_size and the depth at which the nodes of which 
 *              the size should be calculated reside.
 * @depth node_size_at_depth_0 The size of a single node at depth == 0
 *
 * @returns the size of the nodes residing at depth_0 + depth
 */
inline float calculateNodeSizeAtDepth(unsigned int depth, float node_size_at_depth_0) {
  return node_size_at_depth_0 / (calculateNNodes(depth));
}


/*! @brief Calculate the maximum node size given the node_size_at_depth and 
 *         the depth at which these nodes reside.
 *
 * @param depth The depth at which the nodes of the specified node size reside
 * @param node_size_at_depth The node size of the nodes at depth depth
 *
 * @returns the node size of the nodes at depth 0
 */
inline float calculateNodeSizeAtDepth0(unsigned int depth, float node_size_at_depth) {
  return node_size_at_depth * calculateNNodes(depth);
}


}
}