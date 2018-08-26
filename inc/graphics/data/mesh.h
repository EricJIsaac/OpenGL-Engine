#ifndef GRAPHICS_MESH_H
#define GRAPHICS_MESH_H

#include <vector>
#include <string>

class aiMesh;

namespace graphics {
namespace data {

struct Mesh;
namespace mesh {
  /**
  * Attributes:
  *   [0] vertices
  *   [1] normals
  */
  static const std::size_t attribute_count = 2;

  bool load(const std::string& file_path, Mesh*& outptr);
  bool load(aiMesh*& aimesh, Mesh*& mesh);
  void interleave_buffer(Mesh* mesh, std::vector<float>& out_buffer);
} // namespace mesh

struct Mesh
{
public:
  // Attributes
  std::vector<float> vertices;
  std::vector<float> normals;

  std::vector<unsigned short> bone_ids;
  std::vector<float> bone_weights;

  // Elements
  std::vector<unsigned short> indices;
};

} // namespace data
} // namespace graphics

#endif // GRAPHICS_MESH_H
