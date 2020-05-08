#include "graphics/ogl/skeleton_mesh_buffer.h"
#include <cstdio>
#include <algorithm>
#include "graphics/data/skeleton_mesh.h"

namespace graphics {
namespace ogl {

void SkeletonMeshBuffer::generate(
  std::vector<graphics::data::SkeletonMesh*> meshes)
{
  printf("Generating shared skeleton mesh buffer:\n");
  std::size_t vsize = 0;
  std::size_t isize = 0;
  for(std::size_t i = 0; i < meshes.size(); ++i)
  {
    Entry e;
    e.index_count = meshes[i]->mesh->indices.size();
    e.index_offset = isize;
    e.vertex_offset = vsize;
    this->m_entries.push_back(e);

    isize += meshes[i]->mesh->indices.size();
    vsize += meshes[i]->mesh->vertices.size();

    printf(
      "Mesh %lu: index offset %lu | index count %lu | vertex offset %lu\n",
      i, e.index_offset, e.index_count, e.vertex_offset);
  }

  std::size_t vcount = vsize / 3;
  std::size_t buffer_byte_size =
    sizeof(graphics::data::SkeletonMesh_Vertex) * vcount * sizeof(float);

  this->m_skeletons = std::vector<graphics::data::Skeleton*>(meshes.size());
  std::fill(this->m_skeletons.begin(), this->m_skeletons.end(), nullptr);

  glGenVertexArrays(1, &this->m_vao);
  glBindVertexArray(this->m_vao);

  glGenBuffers(1, &this->m_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, this->m_vbo);
  glBufferData(GL_ARRAY_BUFFER, buffer_byte_size, 0, GL_STATIC_DRAW);

  for(std::size_t i =0 ; i < meshes.size(); ++i)
  {
    std::vector<graphics::data::SkeletonMesh_Vertex> ilb;
    graphics::data::mesh::interleave_buffer(meshes[i], ilb);

    std::size_t byte_offset =
      this->m_entries[i].vertex_offset / 3 * sizeof(graphics::data::SkeletonMesh_Vertex);
    std::size_t byte_size = ilb.size() * sizeof(graphics::data::SkeletonMesh_Vertex);

    glBufferSubData(GL_ARRAY_BUFFER, byte_offset, byte_size, &ilb[0]);
    this->m_skeletons[i] = meshes[i]->skeleton;
  }

  glGenBuffers(1, &this->m_ele);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_ele);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
    isize * sizeof(unsigned short), 0, GL_STATIC_DRAW);

  for(std::size_t i = 0; i < meshes.size(); ++i)
  {
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER,
      this->m_entries[i].index_offset * sizeof(unsigned short),
      this->m_entries[i].index_count * sizeof(unsigned short),
      &meshes[i]->mesh->indices[0]);
  }
}

void SkeletonMeshBuffer::predraw()
{
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);
  glEnableVertexAttribArray(3);
  glBindBuffer(GL_ARRAY_BUFFER, this->m_vbo);

  // Vertices
  std::size_t stride = sizeof(graphics::data::SkeletonMesh_Vertex);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);

  // Normals
  char* offset = (char*)0 + 3 * sizeof(float);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)offset);

  // Bone Ids
  offset = (char*)0 + 6 * sizeof(float);
  glVertexAttribIPointer(2, 4, GL_UNSIGNED_SHORT, stride, (void*)offset);

  // Bone Weights
  offset = (char*)0 + 6 * sizeof(float) + 4 * sizeof(unsigned short);
  glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, stride, (void*)offset);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_ele);
}

void SkeletonMeshBuffer::draw(std::size_t id)
{
  if(id < 0 || id > this->m_entries.size())
  {
    printf("Invalid skeleton mesh id %lu requested\n", id);
    return;
  }

  char* offset = (char*)0 +
    this->m_entries[id].index_offset * sizeof(unsigned short);

  glDrawElementsBaseVertex(
    GL_TRIANGLES,
    this->m_entries[id].index_count,
    GL_UNSIGNED_SHORT,
    (void*)offset,
    this->m_entries[id].vertex_offset / 3);
}

void SkeletonMeshBuffer::postdraw()
{
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
}

void SkeletonMeshBuffer::destroy()
{
  glDeleteBuffers(1, &this->m_vbo);
  glDeleteBuffers(1, &this->m_ele);
  glDeleteVertexArrays(1, &this->m_vao);
}

void SkeletonMeshBuffer::getBonePositions(std::size_t id,
					  std::string anim_name,
					  float anim_time,
					  std::vector<glm::mat4x4>& out)
{
  if(this->m_skeletons[id] == nullptr)
  {
    printf("No skeleton exists for id %lu\n", id);
    return;
  }

  auto skeleton = this->m_skeletons[id];

  graphics::data::matrix4x4_map map;
  skeleton->getMatrices(anim_name, anim_time, map);

  out.clear();
  for(std::size_t i = 0; i < skeleton->bone_names.size(); ++i)
  {
    out.push_back(map[skeleton->bone_names[i]]);
  }
  return;
}

} // namespace ogl
} // namespace graphics
