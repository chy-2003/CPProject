#include "includes/BMP.h"
#include "includes/BasicSupport.h"

//g++ -o Demo.exe Demo.cpp lib/*.cpp

BMPGraphics InterestingPicture(1024, 1024);

namespace InterestingGraphics {
    Color ColorRing(int Row, int Colume) {
        char R = char(BasicFunctions::sqr(std::cos(std::atan2(Colume - 512.0, Row - 512.0) / 2.0)) * 255);
        char G = char(BasicFunctions::sqr(std::cos(std::atan2(Colume - 512.0, Row - 512.0) / 2.0 
                - 2.0 * BasicConstants::Pi / 3)) * 255);
        char B = char(BasicFunctions::sqr(std::cos(std::atan2(Colume - 512.0, Row - 512.0) / 2.0 
                + 2.0 * BasicConstants::Pi / 3)) * 255);
        return Color(R, G, B);
    }
    Color ColorSierpinski3(int Row, int Colume) {
        char R = char(std::cos(Row & Colume) * 255);
        char G = char(std::cos((1024 - Row) & (1024 - Colume)) * 255);
        char B = char(std::tan(Row | Colume) * 125);
        return Color(R, G, B);
    }
    Color ColorWave(int Row, int Colume) {
        double s = 3.0 / (Colume + 99);
        double y = s * (Colume + std::sin((Row * Row + (Colume - 700) * (Colume - 700) * 5.0) / 100 / 1024) * 35);
        char R = char((int((Row + 1024) * s + y) % 2 + int((2048 - Row) * s + y) % 2) * 127);
        char G = char((int(5 * (Row + 1024) * s + y) % 2 + int(5 * (2048 - Row) * s + y) % 2) * 127);
        char B = char((int(29 * (Row + 1024) * s + y) % 2 + int(29 * (2048 - Row) * s + y) % 2) * 127);
        return Color(R, G, B);
    }
}

int main(int argc, char **argv) {
    for (int i = 0; i < 1024; ++i)
        for (int j = 0; j < 1024; ++j)
            InterestingPicture.SetPixel(i, j, InterestingGraphics::ColorRing(j, i));
    InterestingPicture.Rename(std::string("Demo_ColorRing.bmp"));
    InterestingPicture.DoOutput();

    for (int i = 0; i < 1024; ++i)
        for (int j = 0; j < 1024; ++j)
            InterestingPicture.SetPixel(i, j, InterestingGraphics::ColorSierpinski3(j, i));
    InterestingPicture.Rename(std::string("Demo_Sierpinski3.bmp"));
    InterestingPicture.DoOutput();
    
    for (int i = 0; i < 1024; ++i)
        for (int j = 0; j < 1024; ++j)
            InterestingPicture.SetPixel(i, j, InterestingGraphics::ColorWave(j, i));
    InterestingPicture.Rename(std::string("Demo_ColorWave.bmp"));
    InterestingPicture.DoOutput();
    return 0;
}