#include <math.h>
#include <stdio.h>
#include <unistd.h>

float a;
const static int W = 73; // Width
const static int H = 24; // Height

int main() {
  int frame = 40;
  float scale = 0;
  float velocity = 0;
  while (frame--) {
    velocity += (1 - scale) / 10 - velocity / 4;
    scale += velocity;
    for (int j = 0; j < H; j ++) { // Loop over y-axis
      for (int i = 0; i < W; i++) { // Loop over x-axis
        float x = scale * (i - 27);
        int colour = 0;
        // Set bits of the 3-bit pixel
        for (int bit = 0; bit < 3; ++bit) { // Calculate color (brightness)
          float y = scale * (j + bit - W / 2);
          //////////////////////////////////////////////////////////////////////
          
          colour = rand() % 8;
          
          //////////////////////////////////////////////////////////////////////
        }
        // "Color" map: 3 bit -> 8 colors, " " is background
        const static char* const COLOURS = " ``~.@o@";
        // Draw the pixel
        putchar(COLOURS[colour]);
      }
      putchar('\n');
    }
    usleep(50000); // 50 ms
    if (frame)
      puts("\x1b[25A"); // Move up Height+1 lines
  }
  return 0;
}
