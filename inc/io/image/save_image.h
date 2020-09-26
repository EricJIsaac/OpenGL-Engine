#ifndef IO_SAVE_IMAGE_H
#define IO_SAVE_IMAGE_H

namespace io {
namespace image {

void save_ppm(
  const char* fileName,
  int width,
  int height,
  int nbytes, // e.g. rgb = 3, rgba = 4
  void* pixels
);

} // namespace image
} // namespace io

#endif // IO_SAVE_IMAGE_H
