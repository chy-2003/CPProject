#ifndef __BBG_FONT__
#define __BBG_FONT__

/*
 * Programed by PaimonZAYCHIK(chy)
 *
 * version 2 (2023.12.11)
 * WriteNoteASC48 (3)
 * WriteSentence (3)
 * WriteSentenceT (3)
 * 
 * version 1 (2023.12.8)
 * WriteNoteHZK16 (3)
 * WriteNoteHZK48 (3)
 * 
 * version 0 (2023.12.7)
 * Procedure WriteNoteHZK16
 */

#include "BMP.h"


void WriteNoteHZK16(BMPGraphics &, int, int, unsigned char *);
void WriteNoteHZK16(BMPGraphics &, int, int, unsigned char *, int);
void WriteNoteHZK16(BMPGraphics &, int, int, unsigned char *, Color, int);

void WriteNoteHZK48(BMPGraphics &, int, int, unsigned char *);
void WriteNoteHZK48(BMPGraphics &, int, int, unsigned char *, int);
void WriteNoteHZK48(BMPGraphics &, int, int, unsigned char *, Color, int);

void WriteNoteASC48(BMPGraphics &, int, int, unsigned char *);
void WriteNoteASC48(BMPGraphics &, int, int, unsigned char *, int);
void WriteNoteASC48(BMPGraphics &, int, int, unsigned char *, Color, int);

void WriteSentence(BMPGraphics &, int, int, unsigned char *, int);
void WriteSentence(BMPGraphics &, int, int, unsigned char *, int, int);
void WriteSentence(BMPGraphics &, int, int, unsigned char *, int, Color, int);

void WriteSentenceT(BMPGraphics &, int, int, unsigned char *, int);
void WriteSentenceT(BMPGraphics &, int, int, unsigned char *, int, int);
void WriteSentenceT(BMPGraphics &, int, int, unsigned char *, int, Color, int);

#endif