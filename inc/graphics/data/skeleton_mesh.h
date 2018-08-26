#ifndef GRAPHICS_SKELETON_MESH_H
#define GRAPHICS_SKELETON_MESH_H

#include "graphics/data/skeleton.h"
#include "graphics/data/mesh.h"
#include <unordered_map>
#include <string>

struct aiNode;
struct aiBone;
struct aiAnimation;
struct aiNodeAnim;

namespace graphics {
namespace data {

struct SkeletonMesh_Vertex;
struct SkeletonMesh;

namespace mesh {
  bool load(const std::string& file_path, SkeletonMesh*& outptr);
  bool load(aiNode*& node, Skeleton*& skeleton,
    std::unordered_map<std::string, aiBone*>& boneMap);
  bool load(aiNode*& node, BoneNode& bone,
    std::unordered_map<std::string, aiBone*>& boneMap);
  bool load(aiAnimation* aianim, Animation& anim, std::size_t id);
  bool load(aiNodeAnim* aianode, AnimNode& anode);

  void prune(BoneNode& bn,
    std::unordered_map<std::string, aiBone*>& boneMap);

  void map_bone_attributes(SkeletonMesh* mesh,
    std::unordered_map<std::string, aiBone*>& boneMap,
    std::unordered_map<std::string, unsigned short>& bone_ids,
    std::vector<std::string>& bone_names
  );

  void interleave_buffer(SkeletonMesh* smesh,
    std::vector<SkeletonMesh_Vertex>& out_buffer);
} // namespace mesh

struct SkeletonMesh_Vertex
{
public:
  float x,y,z;
  float nx,ny,nz;
  unsigned short b1, b2, b3, b4;
  float bw1, bw2, bw3, bw4;
};

struct SkeletonMesh
{
public:
  Skeleton* skeleton;
  Mesh* mesh;
};

} // namespace data
} // namespace graphics

#endif // GRAPHICS_SKELETON_MESH_H
