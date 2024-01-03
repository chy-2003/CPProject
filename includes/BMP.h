#ifndef __BBG_BMP__
#define __BBG_BMP__


/*
 * Programed by PaimonZAYCHIK(chy)
 * 
 * version 3 (2023.12.11)
 *  - Bug Fix
 * 
 * version 2 (2023.12.8)
 * Prevent segment fault in function SetPixel and GetPixel
 * 
 * version 1 (2023.11.26)
 *  - Class BMPGraphics
 *        Add Rename Function
 * 
 * version 0 (2023.11.24, 2023.11.26)
 *  - Class Color
 *      omit
 *  - Class BMPGraphics
 *      a basic api
 *      Default size 512 * 512, Name = [timeNow].bmp, index from [0,0]
 *      Please add ".bmp" while use non-default settings.
 */

#include <chrono>
#include <ctime>
#include <string>
#include <sstream>
#include <iomanip>
#include <cstdio>
#include <vector>


class Color {
    public :
        unsigned char R, G, B;
        Color();
        Color(unsigned char _R, unsigned char _G, unsigned char _B) : R(_R), G(_G), B(_B) {}
        ~Color();
};

class BMPGraphics {
    private :
        std::string Name;
        int Row, Colume;
        std::vector<Color> Pixel;
        void VectorFitSize();
    public :
        BMPGraphics();
        BMPGraphics(int _Row, int _Colume); 
        BMPGraphics(int _Row, int _Colume, std::string _Name); 
        ~BMPGraphics(); 
        void SetPixel(int RowNumber, int ColumeNumber, Color TargetColor);
        void SetBackground(Color);
        Color GetPixel(int RowNumber, int ColumeNumber);
        void DoOutput();
        void Rename(std::string _Name);
};

#endif