#include "graphics/data/mesh_db.h"
#include "graphics/data/mesh.h"
#include "graphics/data/skeleton_mesh.h"

#include "json/json.h"
#include <iostream>
#include <fstream>

namespace graphics {
namespace data {

bool MeshDB::load(std::string filePath) {
  std::ifstream ifs(filePath);
  Json::Value root;
  ifs >> root;

  Json::Value meshes = root["meshes"];
  for(Json::Value::const_iterator it = meshes.begin();
    it != meshes.end(); it++) {
    std::string meshName = it.key().asString();
    std::string meshFileName = (*it).asString();
    std::string meshFilePath = "data/geometry/meshes/" + meshFileName;

    this->m_ids[meshName] = this->m_meshes.size();
    this->m_meshes.push_back(std::make_unique<Mesh>());

    graphics::data::Mesh* mesh = this->m_meshes.back().get();
    if(!graphics::data::mesh::load(meshFilePath, mesh))
    {
      printf("Failed to load mesh %s\n", meshFileName.c_str());
      continue;
    }

    this->m_meshPtrs.push_back(mesh);
  }

  Json::Value smeshes = root["skeletonMeshes"];
  for(Json::Value::const_iterator it = smeshes.begin();
    it != smeshes.end(); it++) {
    std::string meshName = it.key().asString();
    std::string meshFileName = (*it).asString();
    std::string meshFilePath = "data/geometry/skeletonMeshes/" + meshFileName;

    this->m_ids[meshName] = this->m_skeletonMeshes.size();
    this->m_skeletonMeshes.push_back(std::make_unique<SkeletonMesh>());

    graphics::data::SkeletonMesh* mesh = this->m_skeletonMeshes.back().get();
    if(!graphics::data::mesh::load(meshFilePath, mesh))
    {
      printf("Failed to load skeleton-mesh %s\n", meshFileName.c_str());
      continue;
    }

    this->m_smeshPtrs.push_back(mesh);
  }

  ifs.close();
  return true;
}

std::size_t MeshDB::getMeshId(std::string name) {
  return this->m_ids.count(name) ?
    this->m_ids[name] : this->errorMeshId;
}

std::vector<Mesh*> MeshDB::getMeshes() { return this->m_meshPtrs; }
std::vector<SkeletonMesh*> MeshDB::getSkeletonMeshes() { return this->m_smeshPtrs; }

} // namespace data
} // namespace graphics
