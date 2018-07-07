#ifndef GRAPHICS_MESH_H
#define GRAPHICS_MESH_H

#include <vector>

namespace graphics {
namespace data {

struct Mesh
{
public:
  std::vector<float> vertices;
  std::vector<unsigned short> indices;
};

} // namespace data
} // namespace graphics

#endif // GRAPHICS_MESH_H
