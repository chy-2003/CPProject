#include <cstdio>
#include <cstring>
#include "includes/BMP.h"
#include "includes/Font.h"

//g++ -o 2.exe 2.cpp lib/*.cpp

const int Edge = 192, Total = 2048;
BMPGraphics Output(Total, Total, "BifurcationDiagram.bmp");
const int LineSize = 5;
const int Len = Total - 2 * Edge;
double r[2010];
double x[2010];

void GraphInit();

int main() {
    GraphInit();
    for (int i = 0; i < Len; ++i) r[i] = 0.6 + 0.4 * i / Len;
    for (int i = 0; i < Len; ++i) x[i] = 0.5;
    for (int k = 0; k < 8000; ++k)
        for (int i = 0; i < Len; ++i)
            x[i] = 4.0 * r[i] * x[i] * (1 - x[i]);
    for (int k = 0; k < 2000; ++k)
        for (int i = 0; i < Len; ++i) {
            x[i] = 4.0 * r[i] * x[i] * (1 - x[i]);
            int y = (int)(x[i] * Len);
            //if (i % 50 == 0) printf("%d, %.2lf %d\n", i, x[i], y);
            Output.SetPixel(Total - Edge - y, Edge + i, Color(0, 0, 255));
        }
    Output.DoOutput();
    return 0;
}

void GraphInit() {
    for (int i = 0; i < Total; ++i) {
        for (int j = 0; j < Total; ++j)
            Output.SetPixel(i, j, Color(255, 255, 255));
    }
    for (int i = Edge - LineSize - 1; i < Edge; ++i)
        for (int j = Edge - LineSize - 1; j < Total - Edge + LineSize; ++j)
            Output.SetPixel(i, j, Color(0, 0, 0));
    for (int i = Total - Edge; i < Total - Edge + LineSize; ++i)
        for (int j = Edge - LineSize - 1; j < Total - Edge + LineSize; ++j)
            Output.SetPixel(i, j, Color(0, 0, 0));
    for (int j = Edge - LineSize - 1; j < Edge; ++j)
        for (int i = Edge - LineSize - 1; i < Total - Edge + LineSize; ++i)
            Output.SetPixel(i, j, Color(0, 0, 0));
    for (int j = Total - Edge; j < Total - Edge + LineSize; ++j)
        for (int i = Edge - LineSize - 1; i < Total - Edge + LineSize; ++i)
            Output.SetPixel(i, j, Color(0, 0, 0));
    unsigned char Word[100] = "Bifurcation Diagram";
    WriteSentence(Output, 48, 580, Word, 19, Color(0, 0, 0), 2);
    std::memset(Word, 0, sizeof(Word));
    std::memcpy(Word, "0.6", 3);
    WriteSentence(Output, Total - Edge + 20, Edge - 72, Word, 3, Color(0, 0, 0), 2);
    for (int i = Total - Edge - 20; i < Total - Edge; ++i)
        for (int j = Edge + 416 - 2; j <= Edge + 416 + 2; ++j)
            Output.SetPixel(i, j, Color(0, 0, 0));
    std::memset(Word, 0, sizeof(Word));
    std::memcpy(Word, "0.7", 3);
    WriteSentence(Output, Total - Edge + 20, Edge + 416 - 72, Word, 3, Color(0, 0, 0), 2);
    for (int i = Total - Edge - 20; i < Total - Edge; ++i)
        for (int j = Edge + 416 * 2 - 2; j <= Edge + 416 * 2 + 2; ++j)
            Output.SetPixel(i, j, Color(0, 0, 0));
    std::memset(Word, 0, sizeof(Word));
    std::memcpy(Word, "0.8", 3);
    WriteSentence(Output, Total - Edge + 20, Edge + 416 * 2 - 72, Word, 3, Color(0, 0, 0), 2);

    std::memset(Word, 0, sizeof(Word));
    std::memcpy(Word, "r", 1);
    WriteSentence(Output, Total - Edge + 116, Edge + 416 * 2 - 24, Word, 3, Color(0, 0, 0), 2);

    for (int i = Total - Edge - 20; i < Total - Edge; ++i)
        for (int j = Edge + 416 * 3 - 2; j <= Edge + 416 * 3 + 2; ++j)
            Output.SetPixel(i, j, Color(0, 0, 0));
    std::memset(Word, 0, sizeof(Word));
    std::memcpy(Word, "0.9", 3);
    WriteSentence(Output, Total - Edge + 20, Edge + 416 * 3 - 72, Word, 3, Color(0, 0, 0), 2);
    std::memset(Word, 0, sizeof(Word));
    std::memcpy(Word, "1.0", 3);
    WriteSentence(Output, Total - Edge + 20, Edge + 416 * 4 - 72, Word, 3, Color(0, 0, 0), 2);

    std::memset(Word, 0, sizeof(Word));
    std::memcpy(Word, "1.0", 3);
    WriteSentence(Output, Edge - 48, Edge - 154, Word, 3, Color(0, 0, 0), 2);
    for (int i = Edge; i < Edge + 20; ++i)
        for (int j = Edge + 416 - 2; j <= Edge + 416 + 2; ++j)
            Output.SetPixel(j, i, Color(0, 0, 0));
    for (int i = Edge; i < Edge + 20; ++i)
        for (int j = Edge + 416 * 2 - 2; j <= Edge + 416 * 2 + 2; ++j)
            Output.SetPixel(j, i, Color(0, 0, 0));
    std::memset(Word, 0, sizeof(Word));
    std::memcpy(Word, "0.5", 3);
    WriteSentence(Output, Edge + 416 * 2 - 48, Edge - 154, Word, 3, Color(0, 0, 0), 2);
    for (int i = Edge; i < Edge + 20; ++i)
        for (int j = Edge + 416 * 3 - 2; j <= Edge + 416 * 3 + 2; ++j)
            Output.SetPixel(j, i, Color(0, 0, 0));
    std::memset(Word, 0, sizeof(Word));
    std::memcpy(Word, "0.0", 3);
    WriteSentence(Output, Edge + 416 * 4 - 48, Edge - 154, Word, 3, Color(0, 0, 0), 2);
    return;
}