#include "../includes/Plot.h"


void DrawPartical(BMPGraphics &Target, int I, int J, Color color) {
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

void DrawPartical(BMPGraphics &Target, int I, int J) {
    DrawPartical(Target, I, J, Color((unsigned char)255, (unsigned char)255, (unsigned char)255));
}