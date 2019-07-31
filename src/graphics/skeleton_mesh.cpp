#include "graphics/data/skeleton_mesh.h"
#include <cstdio>
#include <cmath>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <algorithm>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

namespace graphics {
namespace data {
namespace mesh {

void toEulerAngle(aiQuaternion& q, double& roll, double& pitch, double& yaw)
{
	// roll (x-axis rotation)
	double sinr = +2.0 * (q.w * q.x + q.y * q.z);
	double cosr = +1.0 - 2.0 * (q.x * q.x + q.y * q.y);
	roll = atan2(sinr, cosr);

	// pitch (y-axis rotation)
	double sinp = +2.0 * (q.w * q.y - q.z * q.x);
	if (fabs(sinp) >= 1)
		pitch = copysign(3.14f / 2, sinp); // use 90 degrees if out of range
	else
		pitch = asin(sinp);

	// yaw (z-axis rotation)
	double siny = +2.0 * (q.w * q.z + q.x * q.y);
	double cosy = +1.0 - 2.0 * (q.y * q.y + q.z * q.z);
	yaw = atan2(siny, cosy);
}

bool load(const std::string& file_path, SkeletonMesh*& outptr)
{
  printf("Loading skeleton mesh: %s\n", file_path.c_str());
  Assimp::Importer importer;
  const aiScene* scene = importer.ReadFile(file_path,
    aiProcess_CalcTangentSpace | aiProcess_Triangulate |
    aiProcess_JoinIdenticalVertices | aiProcess_SortByPType |
    aiProcess_GenNormals
  );

  if(!scene)
  {
    printf("ASSIMP error: %s\n", importer.GetErrorString());
    return false;
  }

  if(scene->HasMeshes() == 0)
  {
    printf("Assimp scene has no meshes. Skeleton Mesh cannot be loaded.\n");
    return false;
  }

  if(scene->mNumMeshes > 1)
  {
    printf("Assimp scene has multiple meshes, only the root will be loaded\n");
  }

  aiMesh *aimesh = scene->mMeshes[0];
  if(!aimesh->HasBones())
  {
    printf("Assimp mesh has no bones. Seleton Mesh cannot be loaded. \n");
    return false;
  }

  outptr = new graphics::data::SkeletonMesh();
  if(!load(aimesh, outptr->mesh))
  {
    printf("Assimp failed to load mesh for skeleton mesh from scene.\n");
    return false;
  }

  std::unordered_map<std::string, aiBone*> bones;
  std::unordered_map<std::string, unsigned short> bone_ids;
  std::vector<std::string> bone_names;
  for(std::size_t i = 0; i < aimesh->mNumBones; ++i)
  {
    bones[aimesh->mBones[i]->mName.C_Str()] = aimesh->mBones[i];
    bone_ids[aimesh->mBones[i]->mName.C_Str()] = i;
    bone_names.push_back(aimesh->mBones[i]->mName.C_Str());
  }

  aiNode* skeletonNode = nullptr;
  int skeletonNodeDepth = 9999;
  for(std::size_t i = 0; i < bone_names.size(); ++i)
  {
    auto searchNode = scene->mRootNode->FindNode(bone_names[i].c_str());
    int searchNodeDepth = 0;
    auto parent = searchNode->mParent;

    while(parent != nullptr)
    {
      searchNodeDepth++;
      parent = parent->mParent;
    }
    
    if(skeletonNode == nullptr || searchNodeDepth < skeletonNodeDepth)
    {
      skeletonNode = searchNode;
      skeletonNodeDepth = searchNodeDepth;
      continue;
    }    
  }

  if(skeletonNode == nullptr)
  {
    printf("Assimp skeleton not found in scene node hiearchy.\n");
    return false;
  }

  // If the bones are parented in a flat hiearchy,
  //  this ensures they still get loaded
  if(skeletonNode->mParent != nullptr)
  {
    skeletonNode = skeletonNode->mParent;
  }

  printf("Root bone determined to be %s\n", skeletonNode->mName.C_Str());
  if(!load(skeletonNode, outptr->skeleton, bones))
  {
    printf("Assimp failed to load skeleton for skeleton mesh from scene.\n");
    return false;
  }

  prune(outptr->skeleton->root, bones);
  map_bone_attributes(outptr, bones, bone_ids, bone_names);

  for(std::size_t i = 0; i < scene->mNumAnimations; ++i)
  {
    Animation anim;
    if(!load(scene->mAnimations[i], anim, i))
    {
      printf("Warning, animation failed to load.\n");
    }
    else
    {
      outptr->skeleton->animations[anim.anim_name] = anim;
    }
  }

  return true;
}


bool load(aiNode*& node, Skeleton*& skeleton,
  std::unordered_map<std::string, aiBone*>& boneMap)
{
  skeleton = new Skeleton();
  
  aiMatrix4x4 m = node->mTransformation;
  skeleton->inv_global_transform = glm::mat4(
     m[0][0], m[0][1], m[0][2], m[0][3],
     m[1][0], m[1][1], m[1][2], m[1][3],
     m[2][0], m[2][1], m[2][2], m[2][3],
     m[3][0], m[3][1], m[3][2], m[3][3]);
  skeleton->inv_global_transform = glm::transpose(skeleton->inv_global_transform);
  skeleton->inv_global_transform = glm::inverse(skeleton->inv_global_transform);
  
  if(!load(node, skeleton->root, boneMap))
  {
    return false;
  }

  return true;
}

bool load(aiNode*& node, BoneNode& bone,
    std::unordered_map<std::string, aiBone*>& boneMap)
{
  bone.bone_name = node->mName.C_Str();
  printf("bone node: %s\n",bone.bone_name.c_str());
  if(boneMap.count(bone.bone_name) > 0)
  {
    aiMatrix4x4 m = boneMap[bone.bone_name]->mOffsetMatrix;
    bone.bone_offset = glm::mat4(
    m[0][0], m[0][1], m[0][2], m[0][3],
    m[1][0], m[1][1], m[1][2], m[1][3],
    m[2][0], m[2][1], m[2][2], m[2][3],
    m[3][0], m[3][1], m[3][2], m[3][3]);
    bone.bone_offset = glm::transpose(bone.bone_offset);
  }
  else
  {
    bone.bone_offset = glm::mat4(1.0f);
  }

  for(std::size_t i = 0; i < node->mNumChildren; ++i)
  {
    BoneNode bn;
    if(!load(node->mChildren[i], bn, boneMap))
    {
      return false;
    }
    bone.children.push_back(bn);
  }

  return true;
}

bool load(aiAnimation* aianim, Animation& anim, std::size_t id)
{
  anim.anim_name = aianim->mName.C_Str();
  if(anim.anim_name == "")
  {
    anim.anim_name = "default_" + std::to_string(id);
  }

  printf("Loading animation %s\n", anim.anim_name.c_str());
  for(std::size_t i = 0; i < aianim->mNumChannels; ++i)
  {
    AnimNode an;
    if(!load(aianim->mChannels[i], an))
    {
      return false;
    }

    anim.anim_nodes.push_back(an);
  }

  anim.setEndTime(aianim->mDuration);
  anim.setAnimBehavior(anim::Behavior::LoopFrames);
  return true;
}

bool load(aiNodeAnim* aianode, AnimNode& anode)
{
  anode.bone_name = aianode->mNodeName.C_Str();
  
  for(std::size_t i = 0; i < aianode->mNumPositionKeys; ++i)
  {
    aiVectorKey aik = aianode->mPositionKeys[i];
    Keyframe k;
    k.vec = glm::vec3(aik.mValue[0], aik.mValue[1], aik.mValue[2]);
    k.time = aik.mTime;
    anode.positions.push_back(k);
  }

  for(std::size_t i = 0; i < aianode->mNumScalingKeys; ++i)
  {
    aiVectorKey aik = aianode->mScalingKeys[i];
    Keyframe k;
    k.vec = glm::vec3(aik.mValue[0], aik.mValue[1], aik.mValue[2]);
    k.time = aik.mTime;
    anode.scales.push_back(k);
  }

  for(std::size_t i = 0; i < aianode->mNumRotationKeys; ++i)
  {
    aiQuatKey aik = aianode->mRotationKeys[i];
    Keyframe k;
    double x,y,z;
    toEulerAngle(aik.mValue, x,y,z);
    k.vec = glm::vec3(x,y,z);
    k.time = aik.mTime;
    anode.rotations.push_back(k);
  }

  return true;
}

void prune(BoneNode& bn,
  std::unordered_map<std::string, aiBone*>& boneMap)
{
  for(std::size_t i = 0; i < bn.children.size(); ++i)
  {
    if(boneMap.count(bn.children[i].bone_name) == 0)
    {
      for(std::size_t j = 0; j < bn.children[i].children.size(); ++j)
      {
        bn.children.push_back(bn.children[i].children[j]);
      }

      bn.children.erase(bn.children.begin() + i);
      --i;
    }
  }

  for(std::size_t i = 0; i < bn.children.size(); ++i)
  {
    prune(bn.children[i], boneMap);
  }
}

void map_bone_attributes(SkeletonMesh* smesh,
  std::unordered_map<std::string, aiBone*>& boneMap,
  std::unordered_map<std::string, unsigned short>& bone_ids,
  std::vector<std::string>& bone_names
)
{
  auto mesh = smesh->mesh;
  auto skeleton = smesh->skeleton;

  const int maxBonesPerVertex = 4;
  const int numVertices = mesh->vertices.size() / 3;
  mesh->bone_ids = std::vector<unsigned short>(numVertices *  maxBonesPerVertex);
  mesh->bone_weights = std::vector<float>(numVertices * maxBonesPerVertex);

  std::fill(mesh->bone_ids.begin(), mesh->bone_ids.end(), 0);
  std::fill(mesh->bone_weights.begin(), mesh->bone_weights.end(), 0);
  skeleton->bone_names.clear();

  for(std::size_t i = 0; i < bone_names.size(); ++i)
  {
    skeleton->bone_names.push_back(bone_names[i]);

    aiBone* aib = boneMap[bone_names[i]];
    for(std::size_t j = 0; j < aib->mNumWeights; ++j)
    {
      std::size_t vi = aib->mWeights[j].mVertexId;
      float w = aib->mWeights[j].mWeight;

      std::size_t minIndex = vi*maxBonesPerVertex;
      for(std::size_t k = vi*maxBonesPerVertex+1; k < (vi+1)*maxBonesPerVertex; ++k)
      {
        if(mesh->bone_weights[k] < mesh->bone_weights[minIndex])
        {
          minIndex = k;
        }
      }

      if(w > mesh->bone_weights[minIndex])
      {
        mesh->bone_ids[minIndex] = i;
        mesh->bone_weights[minIndex] = w;
      }
    }
  }
}

void interleave_buffer(SkeletonMesh* smesh,
  std::vector<SkeletonMesh_Vertex>& out_buffer)
{
  if(smesh == nullptr)
  {
    printf("Error: attempting to interleave buffers for a null skeleton mesh\n");
  }

  for(std::size_t i = 0; i < smesh->mesh->vertices.size()/3; ++i)
  {
    SkeletonMesh_Vertex v;
    v.x = smesh->mesh->vertices[i*3];
    v.y = smesh->mesh->vertices[i*3 + 1];
    v.z = smesh->mesh->vertices[i*3 + 2];

    v.nx = smesh->mesh->normals[i*3];
    v.ny = smesh->mesh->normals[i*3 + 1];
    v.nz = smesh->mesh->normals[i*3 + 2];

    v.b1 = smesh->mesh->bone_ids[i*4];
    v.b2 = smesh->mesh->bone_ids[i*4 + 1];
    v.b3 = smesh->mesh->bone_ids[i*4 + 2];
    v.b4 = smesh->mesh->bone_ids[i*4 + 3];

    v.bw1 = smesh->mesh->bone_weights[i*4];
    v.bw2 = smesh->mesh->bone_weights[i*4 + 1];
    v.bw3 = smesh->mesh->bone_weights[i*4 + 2];
    v.bw4 = smesh->mesh->bone_weights[i*4 + 3];
    out_buffer.push_back(v);
  }
}

} // namespace mesh
} // namespace data
} // namespace graphics
