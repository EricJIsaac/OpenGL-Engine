#include <cstdlib>
#include <GL/glew.h>
#include "io/image/save_image.h"

int main() {
  int width = 800;
  int height = 600;
  int format_nbytes = 4;
  int cur = 0;

  void* pixels = malloc(format_nbytes * width * height);

  GLubyte* data = static_cast<GLubyte*>(pixels);
  for(int y = 0; y < height; y++) {
    for(int x = 0; x < width; x++) {
          cur = format_nbytes * (x + y * width);
          data[cur] = 255;
          data[cur + 1] = 0;
          data[cur + 2] = 0;
    }
  }

  io::image::save_ppm(
    "test-save-red.ppm",
    width,
    height,
    format_nbytes,
    pixels
  );

  for(int y = 0; y < height; y++) {
    for(int x = 0; x < width; x++) {
          cur = format_nbytes * (x + y * width);
          data[cur] = 0;
          data[cur + 1] = 255;
          data[cur + 2] = 0;
    }
  }

  io::image::save_ppm(
    "test-save-green.ppm",
    width,
    height,
    format_nbytes,
    pixels
  );

  for(int y = 0; y < height; y++) {
    for(int x = 0; x < width; x++) {
          cur = format_nbytes * (x + y * width);
          data[cur] = 0;
          data[cur + 1] = 0;
          data[cur + 2] = 255;
    }
  }

  io::image::save_ppm(
    "test-save-blue.ppm",
    width,
    height,
    format_nbytes,
    pixels
  );

  free(pixels);
  return 0;
}
