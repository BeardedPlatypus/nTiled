#pragma once

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "pipeline\PipelineObject.h"

namespace nTiled {
namespace pipeline {

PipelineObject* constructQuad(glm::vec2 lower_left_corner,
                              glm::vec2 upper_right_corner,
                              GLfloat depth);
PipelineObject* constructQuad();

}
}
