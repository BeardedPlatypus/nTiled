#include "pipeline\shader-util\LoadShaders.h"

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <iostream>
#include <sstream>
#include <fstream>

// ----------------------------------------------------------------------------
//  Load Shaders
// ----------------------------------------------------------------------------
// TODO add more descriptive error messages

namespace nTiled {
namespace pipeline {
std::stringstream readShader(const std::string& path) {
  std::ifstream f;
  f.open(path.c_str(), std::ios::in | std::ios::binary);

  if (!f.is_open()) {
    throw std::runtime_error(std::string("Can't open shader file: ") + path);
  }

  std::stringstream buffer;
  buffer << f.rdbuf();
  return buffer;
}

// TODO cleanup
std::stringstream readShaderWithLights(const std::string& path,
                                       int n_lights) {
  // open shader
  std::ifstream f;
  f.open(path.c_str(), std::ios::in | std::ios::binary);

  if (!f.is_open()) {
    throw std::runtime_error(std::string("Can't open shader file: ") + path);
  }

  std::stringstream buffer;

  std::string replaceLine = "#define NUM_LIGHTS ";

  for (std::string line; std::getline(f, line);) {
    if (line.compare(0, replaceLine.size(), replaceLine) == 0) {
      buffer << replaceLine << std::to_string(n_lights) << std::endl;
    } else {
      buffer << line << std::endl;
    }
  }

  return buffer;
}

std::stringstream readShaderWithLightsAndOctreeMaps(const std::string& path,
                                                    unsigned int n_lights,
                                                    unsigned int n_octree_maps) {
  // open shader
  std::ifstream f;
  f.open(path.c_str(), std::ios::in | std::ios::binary);

  if (!f.is_open()) {
    throw std::runtime_error(std::string("Can't open shader file: ") + path);
  }

  std::stringstream buffer;

  std::string replaceLineLights = "#define NUM_LIGHTS ";
  std::string replaceLineOctreeMaps = "#define OCTREE_DEPTH ";

  for (std::string line; std::getline(f, line);) {
    if (line.compare(0, 
                     replaceLineLights.size(), 
                     replaceLineLights) == 0) {
      buffer << replaceLineLights << std::to_string(n_lights) << std::endl;
    } else if (line.compare(0, 
                            replaceLineOctreeMaps.size(), 
                            replaceLineOctreeMaps) == 0) {
      buffer << replaceLineOctreeMaps << std::to_string(n_octree_maps) << std::endl;
    } else {
      buffer << line << std::endl;
    }
  }

  return buffer;

}


GLuint compileShader(GLenum shaderType, const std::string& shaderRaw) {
  GLuint shader = glCreateShader(shaderType);
  if (!shader) {
    throw std::runtime_error(std::string("Can't create shader"));
  }
  const GLchar* shaderCode = shaderRaw.c_str();
  const GLchar* codeArray[] = { shaderCode };

  GLint size = strlen(shaderCode);
  glShaderSource(shader, 1, codeArray, NULL);
  glCompileShader(shader);

  GLint status;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
  if (!status) {
    std::cerr << "Compilation error in shader file " << std::endl;
    GLint logLen;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);
    if (logLen > 0) {
      char *log = new char[logLen];
      GLsizei written;
      glGetShaderInfoLog(shader, logLen, &written, log);

      std::cerr << "Shader log: " << std::endl;
      std::cerr << log << std::endl;
      delete[] log;
    }
    std::cout << std::endl;
    std::cout << shaderRaw << std::endl;
    throw std::runtime_error(std::string("Can't compile the shader defined in file: "));
  }
  return shader;
}

GLuint createProgram(GLuint vertShader, GLuint fragShader) {
  GLuint program = glCreateProgram();
  if (!program) {
    throw std::runtime_error("Can't create GLSL program.");
  }

  glAttachShader(program, vertShader);
  glAttachShader(program, fragShader);
  glLinkProgram(program);

  GLint status;
  glGetProgramiv(program, GL_LINK_STATUS, &status);
  if (!status) {
    std::cerr << "Linking error in shader program!" << std::endl;
    GLint logLen;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLen);
    if (logLen > 0) {
      char *log = new char[logLen];
      GLsizei written;
      glGetProgramInfoLog(program, logLen, &written, log);
      std::cerr << "Shader log: " << std::endl;
      std::cerr << log << std::endl;

      delete[] log;
    }
    throw std::runtime_error("Can't link shader program.");
  }
  return program;
}


// ----------------------------------------------------------------------------
//  Load and compile shaders
// ----------------------------------------------------------------------------
GLuint loadAndCompileShader(GLenum shaderType, const std::string& path) {
  std::ifstream f;
  f.open(path.c_str(), std::ios::in | std::ios::binary);
  if (!f.is_open()) {
    throw std::runtime_error(std::string("Can't open shader file: ") + path);
  }

  std::stringstream buffer;
  buffer << f.rdbuf();

  GLuint shader = glCreateShader(shaderType);
  if (!shader) {
    throw std::runtime_error(std::string("Can't create shader for file: ") + path);
  }

  const std::string& bufferAsString = buffer.str();
  const GLchar* shaderCode = bufferAsString.c_str();
  const GLchar* codeArray[] = { shaderCode };
  GLint size = strlen(shaderCode);
  glShaderSource(shader, 1, codeArray, NULL);
  glCompileShader(shader);

  GLint status;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
  if (!status) {
    std::cerr << "Compilation error in shader file " << path << std::endl;
    GLint logLen;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);
    if (logLen > 0) {
      char *log = new char[logLen];
      GLsizei written;
      glGetShaderInfoLog(shader, logLen, &written, log);

      std::cerr << "Shader log: " << std::endl;
      std::cerr << log << std::endl;
      delete[] log;
    }
    throw std::runtime_error(std::string("Can't compile the shader defined in file: ") + path);
  }
  return shader;
}

// ----------------------------------------------------------------------------
//  Create Vertex Fragment program
// ----------------------------------------------------------------------------
GLuint createVertexFragmentProgram(const std::string& vertex_shader_path,
                                   const std::string& fragment_shader_path) {
  GLuint vertShader = loadAndCompileShader(GL_VERTEX_SHADER, vertex_shader_path);
  GLuint fragShader = loadAndCompileShader(GL_FRAGMENT_SHADER, fragment_shader_path);

  GLuint program = glCreateProgram();
  if (!program) {
    throw std::runtime_error("Can't create GLSL program.");
  }

  glAttachShader(program, vertShader);
  glAttachShader(program, fragShader);
  glLinkProgram(program);

  GLint status;
  glGetProgramiv(program, GL_LINK_STATUS, &status);
  if (!status) {
    std::cerr << "Linking error in shader program!" << std::endl;
    GLint logLen;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLen);
    if (logLen > 0) {
      char *log = new char[logLen];
      GLsizei written;
      glGetProgramInfoLog(program, logLen, &written, log);
      std::cerr << "Shader log: " << std::endl;
      std::cerr << log << std::endl;

      delete[] log;
    }
    throw std::runtime_error("Can't link shader program.");
  }
  return program;
}

// ----------------------------------------------------------------------------
//  ComputeShader compilation
// ----------------------------------------------------------------------------
GLuint createComputeProgram(const std::string& compute_shader_buffer) {
  // create shader
  GLint compute_shader = compileShader(GL_COMPUTE_SHADER, compute_shader_buffer);

  // create program
  GLuint program = glCreateProgram();
  if (!program) {
    throw std::runtime_error("Can't create GLSL program.");
  }

  glAttachShader(program, compute_shader);
  glLinkProgram(program);

  GLint status;
  glGetProgramiv(program, GL_LINK_STATUS, &status);
  if (!status) {
    std::cerr << "Linking error in shader program!" << std::endl;
    GLint logLen;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLen);
    if (logLen > 0) {
      char *log = new char[logLen];
      GLsizei written;
      glGetProgramInfoLog(program, logLen, &written, log);
      std::cerr << "Shader log: " << std::endl;
      std::cerr << log << std::endl;

      delete[] log;
    }
    throw std::runtime_error("Can't link shader program.");
  }
  return program;
}


} // pipeline
} // nTiled