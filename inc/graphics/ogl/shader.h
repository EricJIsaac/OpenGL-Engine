#ifndef GRAPHICS_SHADER_H
#define GRAPHICS_SHADER_H

#include <string>
#include <vector>
#include <unordered_map>
#include <GL/glew.h>

namespace graphics {
namespace ogl {
namespace shader {

  GLuint compile(const char* file_path, GLenum shaderType);
  GLuint link(std::vector<GLuint> shader_ids);

} // namespace shader

struct Shader {
public:
  Shader(GLuint shader_program);
  ~Shader();
  const GLuint get() const;
  const GLuint uniform(std::string uniform_name) const;

private:
  GLuint m_program;
  std::unordered_map<std::string, GLuint> m_uniforms;
};

} // namespace ogl
} // namespace graphics

#endif // GRAPHICS_SHADER_H
