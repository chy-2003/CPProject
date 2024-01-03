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