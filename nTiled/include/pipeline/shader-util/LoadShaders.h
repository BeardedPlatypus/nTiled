/*! @file LoadShaders.h
 *  @brief LoadShaders.h contains all function definitions related to loading 
 *         and compiling openGL shaders.
 */
#pragma once

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <glad\glad.h>
#include <string>

namespace nTiled {
namespace pipeline {

// ----------------------------------------------------------------------------
//  Shader loading and compiling.
// ----------------------------------------------------------------------------
/*! @brief Read the glsl shader at the given Path and return as stringstream.
 * 
 * @param path The path to the openGL file to be read.
 *
 * @return A std::stringstream containing the read glsl file.
 */
std::stringstream readShader(const std::string& path);

/*! @brief Read the glsl shader at the given Path and return as stringstream
 *         where #N_LIGHTS has been replaced with the provided number of 
 *         lights.
 * 
 * @param path The path to the openGL file to be read.
 * @param n_lights The value with which #N_LIGHTS in the glsl file should
 *        be replaced.
 *
 * @return A std::stringstream containing the read glsl file with
 *         #N_LIGHTS replaced by n_lights
 */
std::stringstream readShaderWithLights(const std::string& path, int n_lights);

/*! @brief Compile the given shader with the given shadertype into video memory
 *
 * @param shader_type The GLenum specifying the type of shader to be compiled
 * @param shader The string containing the glsl code of the shader to be 
 *               compiled.
 *
 * @return openGL pointer to the compiled shader in video memory.
 */
GLuint compileShader(GLenum shader_type, const std::string& shader);

/*! @brief Create a openGL program with the specified vertex and 
 *         fragment shaders.
 * 
 * @param vertex_shader openGL pointer to the compiled vertex shader
 * @param fragment_shader openGL pointer to the compiled fragment shader
 *
 * @return openGL pointer to the compiled openGL program
 */
GLuint createProgram(GLuint vertex_shader, GLuint fragment_shader);

/*! @brief Load and compile the shader with the specified shader type at the 
 *         the specified path.
 * 
 * @param shader_type The GLenum specifying the new type of shader to 
 *                    be compiled.
 * @param path The path to the openGL file to be read.
 * 
 * @return openGL pointer to the compiled shader in video memory.
 */
GLuint loadAndCompileShader(GLenum shader_type, const std::string& path);

/*! @brief Load and compile an openGL shader program with the specified 
 *         vertex and fragment shader files.
 * 
 * @param vertex_shader_path Path to the Vertex Shader openGL file
 * @param fragment_shader_path Path to the Fragement Shader openGL file
 * 
 * @return openGL pointer to the compiled openGL program
 */
GLuint createVertexFragmentProgram(const std::string& vertex_shader_path,
	                                 const std::string& fragment_shader_path);


/*! @brief Load and compile the openGL compute shader in the compute_shader
 * 
 * @param compute_shader String containing the compute_shader openGL code
 * 
 * @return openGL pointer to the compiled compute shader in video memory.
 */
GLuint createComputeProgram(const std::string& compute_shader);

} // pipeline
} // nTiled
