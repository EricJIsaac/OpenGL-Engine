#include "graphics/ogl/mesh_buffer.h"
#include <cstdio>
#include "graphics/data/mesh.h"

namespace graphics {
namespace ogl {

void MeshBuffer::generate(
  std::vector<graphics::data::Mesh*> meshes)
{
  printf("Generating shared mesh buffer:\n");
  std::size_t vsize = 0;
  std::size_t isize = 0;
  for(std::size_t i = 0; i < meshes.size(); ++i)
  {
    Entry e;
    e.index_count = meshes[i]->indices.size();
    e.index_offset = isize;
    e.vertex_offset = vsize;
    this->m_entries.push_back(e);

    isize += meshes[i]->indices.size();
    vsize += meshes[i]->vertices.size();

    printf(
      "Mesh %lu: index offset %lu | index count %lu | vertex offset %lu\n",
      i, e.index_offset, e.index_count, e.vertex_offset);
  }

  int attribCount = graphics::data::mesh::attribute_count;
  std::size_t buffer_byte_size = attribCount * vsize * sizeof(float);

  glGenVertexArrays(1, &this->m_vao);
  glBindVertexArray(this->m_vao);

  glGenBuffers(1, &this->m_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, this->m_vbo);
  glBufferData(GL_ARRAY_BUFFER, buffer_byte_size, 0, GL_STATIC_DRAW);

  for(std::size_t i =0 ; i < meshes.size(); ++i)
  {
    std::vector<float> ilb;
    graphics::data::mesh::interleave_buffer(meshes[i], ilb);

    std::size_t byte_offset =
      this->m_entries[i].vertex_offset * sizeof(float) * attribCount;
    std::size_t byte_size = ilb.size() * sizeof(float);

    glBufferSubData(GL_ARRAY_BUFFER, byte_offset, byte_size, &ilb[0]);
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
      &meshes[i]->indices[0]);
  }
}

void MeshBuffer::predraw()
{
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, this->m_vbo);

  std::size_t stride = 6 * sizeof(float);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);

  char* offset = (char*)0 + 3 * sizeof(float);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)offset);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_ele);
}

void MeshBuffer::draw(std::size_t id)
{
  if(id < 0 || id > this->m_entries.size())
  {
    printf("Invalid mesh id %lu requested\n", id);
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

void MeshBuffer::postdraw()
{
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

void MeshBuffer::destroy()
{
  glDeleteBuffers(1, &this->m_vbo);
  glDeleteBuffers(1, &this->m_ele);
  glDeleteVertexArrays(1, &this->m_vao);
}

} // namespace ogl
} // namespace graphics
