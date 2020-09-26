#ifndef GRAPHICS_SAVE_IMAGE_H
#define GRAPHICS_SAVE_IMAGE_H

namespace graphics {
namespace image {

void save_ppm(
  char* fileName,
  int width,
  int height,
  int nbytes, // e.g. rgb = 3, rgba = 4
  void* pixels
);

} // namespace image
} // namespace graphics

#endif // GRAPHICS_SAVE_IMAGE_H
