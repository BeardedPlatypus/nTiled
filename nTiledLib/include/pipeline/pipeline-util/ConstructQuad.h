/*! @file ConstructQuad.h
 *  @brief ConstructQuad.h contains all functions to construct quads objects
 *         in nTiled. 
 */
#pragma once

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "pipeline\PipelineObject.h"

namespace nTiled {
namespace pipeline {

/*! Construct a new Quad PipelineObject with the given parameters and 
 *  return a pointer to this PipelineObject.
 * 
 * @param lower_left_corner The lower left corner of the new Quad.
 * @param upper_right_corner The upper right corner of the new Quad.
 * @param depth The depth from the camera of this new Quad.
 * 
 * @return A pointer to the new PipelineObject Quad.
 */
PipelineObject* constructQuad(glm::vec2 lower_left_corner,
                              glm::vec2 upper_right_corner,
                              GLfloat depth);

/*! Construct a new PipelineObject containing a screen filling quad.
 * 
 * @return A pointer to the new PipelineObject Quad.
 */
PipelineObject* constructQuad();

}
}
