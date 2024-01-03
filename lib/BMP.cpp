#include "../includes/BMP.h"

Color::Color() : R(0), G(0), B(0) {}

Color::~Color() {}

void BMPGraphics::VectorFitSize() {
    Pixel.clear();
    Pixel.resize(Row * Colume);
    Pixel.shrink_to_fit();
    return;
}

BMPGraphics::BMPGraphics() {
    std::time_t Now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::stringstream SS;
    SS << std::put_time(std::localtime(&Now), "%F%T");
    Name = SS.str(); Name = Name + ".bmp";
    Row = 512;
    Colume = 512;
    VectorFitSize();
    return;
}

BMPGraphics::BMPGraphics(int _Row, int _Colume) : Row(_Row), Colume(_Colume) {
    std::time_t Now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::stringstream SS;
    SS << std::put_time(std::localtime(&Now), "%F%T");
    Name = SS.str(); Name = Name + ".bmp";
    VectorFitSize();
    return;
}

BMPGraphics::BMPGraphics(int _Row, int _Colume, std::string _Name) : Row(_Row), Colume(_Colume), Name(_Name) {
    VectorFitSize();
    return;
}

void BMPGraphics::Rename(std::string _Name) { Name = _Name; return; }

BMPGraphics::~BMPGraphics() {
    Row = Colume = 0;
    Name = "";
    VectorFitSize();
    return;
}

void BMPGraphics::SetPixel(int RowNumber, int ColumeNumber, Color TargetColor) {
    if (RowNumber >= Row || ColumeNumber >= Colume) return;
    Pixel[(Row - RowNumber - 1) * Colume + ColumeNumber] = TargetColor;
    return;
}

void BMPGraphics::SetBackground(Color TargetColor) {
    for (int i = 0; i < Row; ++i)
        for (int j = 0; j < Row; ++j)
            SetPixel(i, j, TargetColor);
    return;
}

Color BMPGraphics::GetPixel(int RowNumber, int ColumeNumber) {
    if (RowNumber >= Row || ColumeNumber >= Colume) return Color(0, 0, 0);
    return Pixel[(Row - RowNumber - 1) * Colume + ColumeNumber];
}

void BMPGraphics::DoOutput() {
    FILE *OutputTarget = fopen(Name.data(), "wb");
    int RowSize = (24 * Colume + 31) / 32 * 4;
    int RawSize = RowSize * Row;
    int FileSize = 54 + RawSize;
    short InsertedZero = RowSize - Colume * 3;
    //Bitmap File Header : 00-01 Name, BM
    fprintf(OutputTarget, "BM");
    //Bitmap File Header : 02-05 FileSize, Little Endian
    fprintf(OutputTarget, "%c%c%c%c", char(FileSize & 0xff), char((FileSize >> 8) & 0xff), 
            char((FileSize >> 16) & 0xff), char((FileSize >> 24))); 
    //Bitmap File Header : 06-07 reserved, zero
    fprintf(OutputTarget, "%c%c", char(0), char(0));
    //Bitmap File Header : 08-09 reserved, zero
    fprintf(OutputTarget, "%c%c", char(0), char(0));
    //Bitmap File Header : 0A-0D Start Position, Little Endian, 0x36 = 54
    fprintf(OutputTarget, "%c%c%c%c", char(36), char(0), char(0), char(0));

    //Bitmap Information Header : 0E-11 Size of DIB header, Little Endian, 0x28 = 40
    fprintf(OutputTarget, "%c%c%c%c", char(40), char(0), char(0), char(0));
    //Bitmap Information Header : 12-15 Colume Number, Little Endian
    fprintf(OutputTarget, "%c%c%c%c", char(Colume & 0xff), char((Colume >> 8) & 0xff),
            char((Colume >> 16) & 0xff), char(Colume >> 24));
    //Bitmap Information Header : 16-19 Row Number, Little Endian
    fprintf(OutputTarget, "%c%c%c%c", char(Colume & 0xff), char((Colume >> 8) & 0xff),
            char((Colume >> 16) & 0xff), char(Colume >> 24));
    //Bitmap Information Header : 1A-1B Number of Color Plane, Must be 1
    fprintf(OutputTarget, "%c%c", char(1), char(0));
    //Bitmap Information Header : 1C-1D Color Depth 0x18 = 24
    fprintf(OutputTarget, "%c%c", char(24), char(0));
    //Bitmap Information Header : 1E-21 Zip method 0
    fprintf(OutputTarget, "%c%c%c%c", char(0), char(0), char(0), char(0));
    //Bitmap Information Header : 22-25 RawSize, Little Endian
    fprintf(OutputTarget, "%c%c%c%c", char(RawSize & 0xff), char((RawSize >> 8) & 0xff),
            char((RawSize >> 16) & 0xff), char(RawSize >> 24));
    //Bitmap Information Header : 26-29 Raw Pixel / Meter, Not Important, set zero
    fprintf(OutputTarget, "%c%c%c%c", char(0), char(0), char(0), char(0));
    //Bitmap Information Header : 2A-2D Colume Pixel / Meter, Not Important, set zero
    fprintf(OutputTarget, "%c%c%c%c", char(0), char(0), char(0), char(0));
    //BitmapInformation Header : 2E-31 Color in panel, Not Important, set zero(black)
    fprintf(OutputTarget, "%c%c%c%c", char(0), char(0), char(0), char(0));
    //BitmapInformation Header : 32-35 Number of Important Color, Set zero, all are important
    fprintf(OutputTarget, "%c%c%c%c", char(0), char(0), char(0), char(0));

    //Bitmap Body
    for (int i = 0; i < Row; ++i) {
        for (int j = 0; j < Colume; ++j)
            fprintf(OutputTarget, "%c%c%c", Pixel[i * Colume + j].R, 
                    Pixel[i * Colume + j].G, Pixel[i * Colume + j].B);
        for (short j = 0; j < InsertedZero; ++j)
            fprintf(OutputTarget, "%c", char(0));
    }
    fclose(OutputTarget);
    return;
}