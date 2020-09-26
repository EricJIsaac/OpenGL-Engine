#include <GL/glew.h>
#include <cstdio>
#include "io/image/save_image.h"

namespace io {
namespace image {

  void save_ppm(
    const char* fileName,
    int width,
    int height,
    int nbytes,
    void* pixels
  ) {
      GLubyte* data = static_cast<GLubyte*>(pixels);
      int cur = 0;
      FILE* file = fopen(fileName, "w");
      fprintf(file, "P3\n%d %d\n%d\n", width, height, 255);

      for(int y = 0; y < height; y++) {
        for(int x = 0; x < width; x++) {
          cur = nbytes * (x + (height - y - 1) * width);
          GLubyte r = data[cur];
          GLubyte g = data[cur + 1];
          GLubyte b = data[cur + 2];
          fprintf(file, "%d %d %d\n", r, g, b);
        }
      }
  }

} // namespace image
} // namespace io
