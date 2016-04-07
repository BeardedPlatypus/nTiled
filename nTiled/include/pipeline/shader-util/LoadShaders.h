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
std::stringstream readShader(const std::string& path);
GLuint compileShader(GLenum shaderType, const std::string& shader);
GLuint createProgram(GLuint vertexShader, GLuint fragmentShader);

GLuint loadAndCompileShader(GLenum shaderType, const std::string& path);
GLuint createVertexFragmentProgram(const std::string& vertex_shader_path,
	                                 const std::string& fragment_shader_path);

} // pipeline
} // nTiled
