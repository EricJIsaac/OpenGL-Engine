#ifndef GRAPHICS_SKELETON_MESH_BUFFER_H
#define GRAPHICS_SKELETON_MESH_BUFFER_H

#include <GL/glew.h>
#include <vector>
#include <string>
#include <glm/mat4x4.hpp>

namespace graphics {
namespace data {
  struct SkeletonMesh;
  struct Skeleton;
} // namespace data

namespace ogl {

class SkeletonMeshBuffer
{
public:
  void generate(std::vector<graphics::data::SkeletonMesh*> meshes);
  void predraw();
  void draw(std::size_t id);
  void postdraw();
  void destroy();

  void getBonePositions(std::size_t id,
			std::string anim_name,
			float anim_time,
			std::vector<glm::mat4x4>& out);

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
  std::vector<graphics::data::Skeleton*> m_skeletons;
};

} // namespace ogl
} // namespace graphics

#endif // GRAPHICS_SKELETON_MESH_BUFFER_H
