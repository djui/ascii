#include <math.h>
#include <stdio.h>
#include <unistd.h>

float a;
const static int W = 73;
const static int H = 24;

int
main()
{
    int frame = 40;
    float scale = 0;
    float velocity = 0;
    while (frame--)
    {
        velocity += (1 - scale) / 10 - velocity / 4;
        scale += velocity;
        for (int j = 0; j < (W - 1); j += 3)
        {
            for (int i = 0; i++ < W;)
            {
                float x = scale * (i - 27);
                int colour = 0;
                // Set bits of the 3-bit pixel.
                for (int bit = 0; bit < 3; ++bit)
                {
                    float y = scale * (j + bit - W / 2);
                    // Flip if inside the ellipse.
                    colour ^= (136 * (x*x) + 84 * (y*y) < 92033) << bit;
                    for (int k = 0, p = 6, m = 0; m < 8;)
                    {
                        const static char BASE = 'O';
                        a = ("O:85!fI,wfO8!yZfO8!f*hXK3&fO;:O;#hP;\"i"[k] - BASE) / (46 / M_PI);
                        if (("<[\\]O=IKNAL;KNRbF8EbGEROQ@BSXXtG!#t3!^"[k++] - BASE) / 1.16 > x*cos(a) + y*sin(a))
                        {
                            k = p;
                            p = "<AFJPTX"[m++] - 50;
                        }
                        else if (k == p)
                        {
                            // Flip if inside the polygon:
                            // twice for "Y", once for "!"
                            colour ^= (1 << bit);
                            break;
                        }
                    }
                }
                // Draw the pixel.
                const static char* const COLOURS = " ''\".$u$";
                putchar(COLOURS[colour]);
            }
            putchar('\n');
        }
        usleep(50000);
        if (frame)
        {
            puts("\x1b[25A");
        }
    }
    return 0;
}
