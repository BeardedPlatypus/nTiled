#include "pipeline\PipelineObject.h"

namespace nTiled {
namespace pipeline {

PipelineObject::PipelineObject(const GLuint vao,
                               const GLuint element_buffer,
                               const GLuint n_elements,
                               glm::mat4 transformation_matrix) :
    vao(vao),
    element_buffer(element_buffer),
    n_elements(n_elements),
    transformation_matrix(transformation_matrix) { }

}
}