#include <GL/glew.h>
#include <cstdio>
#include "graphics/image/save_image.h"

namespace graphics {
namespace image {

  void save_ppm(
    char* fileName,
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
          cur = nbytes * (x + y * width);
          GLubyte r = data[cur];
          GLubyte g = data[cur + 1];
          GLubyte b = data[cur + 2];
          fprintf(file, "%d %d %d", r, g, b);
        }
        fprintf(file, "\n");
      }
  }

} // namespace image
} // namespace graphics
