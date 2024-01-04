#include "../includes/Plot.h"


void DrawParticle(BMPGraphics &Target, int I, int J, Color color) {
    Target.SetPixel(I, J, color);
    Target.SetPixel(I - 1, J - 1, color);
    Target.SetPixel(I - 1, J + 1, color);
    Target.SetPixel(I + 1, J - 1, color);
    Target.SetPixel(I + 1, J + 1, color);
    Target.SetPixel(I, J - 1, color);
    Target.SetPixel(I, J + 1, color);
    Target.SetPixel(I - 1, J, color);
    Target.SetPixel(I + 1, J, color);
    return;
}

void DrawParticle(BMPGraphics &Target, int I, int J) {
    DrawParticle(Target, I, J, Color((unsigned char)255, (unsigned char)255, (unsigned char)255));
}

void DrawParticleR(BMPGraphics &Target, int I, int J, int R, Color color) {
    for (int i = I - R; i <= I + R; ++i)
        for (int j = J - R; j <= J + R; ++j)
            Target.SetPixel(i, j, color);
    return;
}