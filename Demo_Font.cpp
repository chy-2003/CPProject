#include <cstdio>
#include "includes/BMP.h"
#include "includes/Font.h"

//g++ -o Demo_Font.exe Demo_Font.cpp lib/*.cpp

BMPGraphics DemoFont(1024, 1024, "FontTest.bmp");

int main() {
    unsigned char Word[100];
    //scanf("%s", Word);
    unsigned char ch;
    int len = 0;
    ch = getchar();
    while (ch != '\n') {
        Word[len++] = ch;
        ch = getchar();
    }
    WriteSentenceT(DemoFont, 10, 10, Word, len);
    DemoFont.DoOutput();
    return 0;
}