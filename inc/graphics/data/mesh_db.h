#ifndef GRAPHICS_MESH_DB
#define GRAPHICS_MESH_DB

#include <vector>
#include <unordered_map>
#include <string>
#include <memory>

namespace graphics {
namespace data {
struct Mesh;
struct SkeletonMesh;

class MeshDB {
public:
  bool load(std::string filePath);

  std::size_t getMeshId(std::string name);
  std::vector<Mesh*> getMeshes();
  std::vector<SkeletonMesh*> getSkeletonMeshes();

private:
  std::size_t errorMeshId = 0;
  std::unordered_map<std::string, std::size_t> m_ids;

  std::vector<Mesh*> m_meshPtrs;
  std::vector<SkeletonMesh*> m_smeshPtrs;

  std::vector<std::unique_ptr<Mesh>> m_meshes;
  std::vector<std::unique_ptr<SkeletonMesh>> m_skeletonMeshes;
};

} // namespace data
} // namespace graphics

#endif // GRAPHICS_MESH_DB
