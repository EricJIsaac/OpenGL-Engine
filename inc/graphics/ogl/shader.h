#ifndef GRAPHICS_SHADER_H
#define GRAPHICS_SHADER_H

#include <vector>
#include <GL/glew.h>

namespace graphics {
namespace ogl {
namespace shader {
  
  GLuint compile(const char* file_path, GLenum shaderType);
  GLuint link(std::vector<GLuint> shader_ids);

} // namespace shader
} // namespace ogl
} // namespace graphics

#endif // GRAPHICS_SHADER_H
