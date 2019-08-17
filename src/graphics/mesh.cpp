#include "graphics/data/mesh.h"
#include <cstdio>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace graphics {
namespace data {
namespace mesh {

bool load(const std::string& file_path, Mesh*& outptr)
{
  printf("Loading mesh: %s\n", file_path.c_str());
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

  if(scene->mNumMeshes == 0)
  {
    printf("No mesh was found in this assimp scene\n");
    return false;
  }

  if(scene->mNumMeshes > 1)
  {
    printf("Assimp scene has multiple meshes, only the root will be loaded\n");
  }

  aiMesh *aimesh = scene->mMeshes[0];
  bool result = load(aimesh, outptr);

  importer.FreeScene();
  return result;
}

bool load(aiMesh*& aimesh, Mesh*& mesh)
{
  mesh = new Mesh();

  for(std::size_t i = 0; i < aimesh->mNumVertices; ++i)
  {
    aiVector3D v = aimesh->mVertices[i];
    mesh->vertices.push_back(v[0]);
    mesh->vertices.push_back(v[1]);
    mesh->vertices.push_back(v[2]);

    aiVector3D n = aimesh->mNormals[i];
    mesh->normals.push_back(n[0]);
    mesh->normals.push_back(n[1]);
    mesh->normals.push_back(n[2]);
  }

  if(mesh->vertices.size() == 0)
  {
    printf("Warning: this mesh has no geometry\n");
  }

  if(mesh->normals.size() != mesh->vertices.size())
  {
    printf("Warning: this mesh has a %lu verts & %lu norms\n",
      mesh->vertices.size(), mesh->normals.size());
  }

  for(std::size_t i = 0; i < aimesh->mNumFaces; i++)
  {
    aiFace face = aimesh->mFaces[i];
    for(std::size_t j = 0; j < face.mNumIndices; j++)
    {
        mesh->indices.push_back(face.mIndices[j]);
    }
  }

  return true;
}

/**
* If this function is edited, the number of attribute counts will
* need to be changed accordingly
*
* This function interleaves graphics::data::mesh::attribute_count
* number of attributes into a single buffer
*/
void interleave_buffer(Mesh* mesh, std::vector<float>& out_buffer)
{
  if(mesh == nullptr)
  {
    printf("Error: attempting to interleave buffers for a null mesh\n");
  }

  for(std::size_t i = 0; i < mesh->vertices.size(); i += 3)
  {
    out_buffer.push_back(mesh->vertices[i]);
    out_buffer.push_back(mesh->vertices[i + 1]);
    out_buffer.push_back(mesh->vertices[i + 2]);

    out_buffer.push_back(mesh->normals[i]);
    out_buffer.push_back(mesh->normals[i + 1]);
    out_buffer.push_back(mesh->normals[i + 2]);
  }
}

} // namespace mesh
} // namespace data
} // namespace graphics
