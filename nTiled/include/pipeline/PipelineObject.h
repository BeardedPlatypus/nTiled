/*! @file PipelineObject.h
 *  @biref PipelineObject.h contains the definition of PipelineObject. Which
 *         serves as a data struct to render that specific object in a 
 *         Pipeline.
 */
#pragma once


// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <glad\glad.h>
#include <glm\glm.hpp>


namespace nTiled {
namespace pipeline {

/*! @brief The PipelineObject is the datastructure which holds all relevant
 *         data for a Pipeline to render an object with the Shader associated
 *         with it.
 */
struct PipelineObject {
public:
  //--------------------------------------------------------------------------
  // constructor
  //--------------------------------------------------------------------------
  /*! @brief Construct a new PipelineObject with the given parameters.
   *
   * @param vao openGL pointer to the Vertex Array Object (vao) of this new
   *            new PipelineObject.
   * @param element_buffer openGL pointer to the element buffer of this new
   *                       PipelineObject
   * @param n_elements The number of elements in the element_buffer of this 
   *                   new PipelineObject.
   * @param transformation_matrix The transformation matrix of this new
   *                              PipelineObject.
   */
  PipelineObject(const GLuint vao,
                 const GLuint element_buffer,
                 const GLuint n_elements,
                 glm::mat4 transformation_matrix);

  //--------------------------------------------------------------------------
  // member variables
  //--------------------------------------------------------------------------
  /*! @brief openGL pointer to the Vertex Array Object (vao) of this
   *         PipelineObject.
   */
  const GLuint vao;
  /*! @brief openGL pointer to the element buffer in video memory of this 
   *         PipelineObject.
   */
  const GLuint element_buffer;
  /*! @brief number of elements in this PipelineObject. */
  const GLuint n_elements;
  /*! @brief transformation matrix of this PipelineObject. */
  glm::mat4 transformation_matrix;
};

} // pipeline
} // nTiled