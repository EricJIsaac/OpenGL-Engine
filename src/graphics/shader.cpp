#include "graphics/ogl/shader.h"
#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <vector>
#include <sstream>
#include <string>
#include <fstream>
#include <csignal>

namespace graphics {
namespace ogl {
namespace shader {

  GLuint compile(const char* file_path, GLenum shaderType)
  {
    GLuint shaderId  = glCreateShader(shaderType);

    std::string shaderCode;
    std::ifstream shaderStream(file_path, std::ios::in);
    if(shaderStream.is_open())
    {
      std::stringstream sstr;
      sstr << shaderStream.rdbuf();
      shaderCode = sstr.str();
      shaderStream.close();
    }
    else
    {
      printf("Unable to open file %s.\n", file_path);
      getchar();
      return 0;
    }

    GLint result = GL_FALSE;
    int infoLogLength;

    printf("Compiling shader : %s\n", file_path);
    char const * sourcePointer = shaderCode.c_str();
    glShaderSource(shaderId, 1, &sourcePointer, NULL);
    glCompileShader(shaderId);

    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);
    glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLogLength);
    if(infoLogLength > 0)
    {
      std::vector<char> shaderErrorMsg(infoLogLength+1);
      glGetShaderInfoLog(shaderId, infoLogLength, NULL, &shaderErrorMsg[0]);
      printf("%s\n", &shaderErrorMsg[0]);

      raise(SIGABRT);
    }

    return shaderId;
  }

  GLuint link(std::vector<GLuint> shader_ids)
  {
    printf("Linking program\n");
    GLuint programId = glCreateProgram();
    GLint result = GL_FALSE;
    int infoLogLength;

    for(std::size_t i = 0; i < shader_ids.size(); ++i)
    {
      glAttachShader(programId, shader_ids[i]);
    }

    glLinkProgram(programId);

    glGetProgramiv(programId, GL_LINK_STATUS, &result);
    glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &infoLogLength);
    if( infoLogLength > 0)
    {
      std::vector<char> programErrorMsg(infoLogLength+1);
      glGetProgramInfoLog(programId, infoLogLength, NULL, &programErrorMsg[0]);
      printf("%s\n", &programErrorMsg[0]);
    }

    for(std::size_t i = 0; i < shader_ids.size(); ++i)
    {
      glDetachShader(programId, shader_ids[i]);
      glDeleteShader(shader_ids[i]);
    }

    return programId;
  }

} // namespace shader

Shader::Shader(GLuint shader_program)
{
  this->m_program = shader_program;

  this->m_uniforms["mvp"] = glGetUniformLocation(this->m_program, "uniform_mvp");
  this->m_uniforms["bpos"] = glGetUniformLocation(this->m_program, "uniform_bpos");
}

Shader::~Shader()
{
  glDeleteProgram(this->m_program);
}

const GLuint Shader::get() const
{
  return this->m_program;
}

const GLuint Shader::uniform(std::string uniform_name) const
{
  if(this->m_uniforms.count(uniform_name) == 0)
  {
    printf("Unrecognized shader '%s' uniform requested.\n", uniform_name.c_str());
    return -1;
  }

  return this->m_uniforms.at(uniform_name);
}

} // namespace ogl
} // namespace graphics
