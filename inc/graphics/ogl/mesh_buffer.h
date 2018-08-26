#ifndef GRAPHICS_MESH_BUFFER_H
#define GRAPHICS_MESH_BUFFER_H

#include <GL/glew.h>
#include <vector>

namespace graphics {
namespace data {
  struct Mesh;
} // namespace data

namespace ogl {

class MeshBuffer
{
public:
  void generate(std::vector<graphics::data::Mesh*> meshes);
  void predraw();
  void draw(std::size_t id);
  void postdraw();
  void destroy();

private:
  struct Entry
  {
    std::size_t index_count;
    std::size_t index_offset;
    std::size_t vertex_offset;
  };

  GLuint m_vao;
  GLuint m_vbo;
  GLuint m_ele;

  std::vector<Entry> m_entries;
};

} // namespace ogl
} // namespace graphics

#endif // GRAPHICS_MESH_BUFFER_H
