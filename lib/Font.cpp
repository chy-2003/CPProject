#include "../includes/Font.h"

void WriteNoteHZK16(BMPGraphics &Target, int I, int J, unsigned char *Word, Color color, int Scale) {
	//printf("%s\n", Word);
	FILE *HZK16 = fopen("font/HZK16", "rb");
	if (HZK16 == NULL) {
		printf("HZK16 import error.\n");
		return;
	}
	unsigned char buffer[32];
	//unsigned char key[8] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};
	int offset = (94 * (unsigned int)(Word[0] - 0xa0 - 1) + (Word[1] - 0xa0 - 1)) * 32;
	int flg;
	fseek(HZK16, offset, SEEK_SET);
	fread(buffer, 1, 32, HZK16);
	for (int k = 0; k < 16; ++k)
		for (int j = 0; j < 2; ++j) 
			for (int i = 0; i < 8; ++i) {
				//flg = buffer[k * 2 + j] & key[i];
				flg = (buffer[k * 2 + j] >> (7 - i)) & 1;
				if (flg) {
					for (int ii = 0; ii < Scale; ++ii)
						for (int jj = 0; jj < Scale; ++jj)
							Target.SetPixel(I + k * Scale + ii, J + (j * 8 + i) * Scale + jj, color);
				}
			}
	fclose(HZK16);
	return;
}

void WriteNoteHZK16(BMPGraphics &Target, int I, int J, unsigned char *Word) {
	WriteNoteHZK16(Target, I, J, Word, Color((unsigned char)255, (unsigned char)255, (unsigned char)255), 1);
	return;
}

void WriteNoteHZK16(BMPGraphics &Target, int I, int J, unsigned char *Word, int Scale) {
	WriteNoteHZK16(Target, I, J, Word, Color((unsigned char)255, (unsigned char)255, (unsigned char)255), Scale);
	return;
}

void WriteNoteHZK48(BMPGraphics &Target, int I, int J, unsigned char *Word, Color color, int Scale) {
	FILE *HZK48 = fopen("font/HZK48", "rb");
	if (HZK48 == NULL) {
		printf("HZK48 import error.\n");
		return;
	}
	unsigned char buffer[288];
	int offset = (94 * (unsigned int)(Word[0] - 0xa0 - 1) + (Word[1] - 0xa0 - 1)) * 288;
	int flg;
	fseek(HZK48, offset, SEEK_SET);
	fread(buffer, 1, 288, HZK48);
	for (int k = 0; k < 48; ++k) 
		for (int j = 0; j < 6; ++j)
			for (int i = 0; i < 8; ++i) {
				flg = (buffer[k * 6 + j] >> (7 - i)) & 1;
				if (flg) 
					for (int ii = 0; ii < Scale; ++ii)
						for (int jj = 0; jj < Scale; ++jj)
							Target.SetPixel(I + k * Scale + ii, J + (j * 8 + i) * Scale + jj, color);
			}
	fclose(HZK48);
	return;
}

void WriteNoteHZK48(BMPGraphics &Target, int I, int J, unsigned char *Word) {
	WriteNoteHZK48(Target, I, J, Word, Color((unsigned char)255, (unsigned char)255, (unsigned char)255), 1);
	return;
}

void WriteNoteHZK48(BMPGraphics &Target, int I, int J, unsigned char *Word, int Scale) {
	WriteNoteHZK48(Target, I, J, Word, Color((unsigned char)255, (unsigned char)255, (unsigned char)255), Scale);
	return;
}


void WriteNoteASC48(BMPGraphics &Target, int I, int J, unsigned char *Word, Color color, int Scale) {
	FILE *ASC48 = fopen("font/ASC48", "rb");
	if (ASC48 == NULL) {
		printf("HZK48 import error.\n");
		return;
	}
	unsigned char buffer[288];
	int offset = Word[0] * 144;
	int flg;
	fseek(ASC48, offset, SEEK_SET);
	fread(buffer, 1, 144, ASC48);
	for (int k = 0; k < 48; ++k)
		for (int j = 0; j < 3; ++j)
			for (int i = 0; i < 8; ++i) {
				flg = (buffer[k * 3 + j] >> (7 - i)) & 1;
				if (flg) 
					for (int ii = 0; ii < Scale; ++ii)
						for (int jj = 0; jj < Scale; ++jj)
							Target.SetPixel(I + k * Scale + ii, J + (j * 8 + i) * Scale + jj, color);
			}
	fclose(ASC48);
	return;
}

void WriteNoteASC48(BMPGraphics &Target, int I, int J, unsigned char *Word) {
	WriteNoteASC48(Target, I, J, Word, Color((unsigned char)255, (unsigned char)255, (unsigned char)255), 1);
	return;
}

void WriteNoteASC48(BMPGraphics &Target, int I, int J, unsigned char *Word, int Scale) {
	WriteNoteASC48(Target, I, J, Word, Color((unsigned char)255, (unsigned char)255, (unsigned char)255), Scale);
	return;
}


void WriteSentence(BMPGraphics &Target, int I, int J, unsigned char *Word, int len, Color color, int Scale) {
	int Shift = 0;
	for (int i = 0; i < len; ++i) {
		if ((Word[Shift] >> 7) & 1) {
			WriteNoteHZK48(Target, I, J + Shift * 24 * Scale, Word + Shift, color, Scale);
			Shift += 2;
		} else {
			WriteNoteASC48(Target, I, J + Shift * 24 * Scale, Word + Shift, color, Scale);
			Shift += 1;
		}
	}
	return;
}

void WriteSentence(BMPGraphics &Target, int I, int J, unsigned char *Word, int len) {
	WriteSentence(Target, I, J, Word, len, Color((unsigned char)255, (unsigned char)255, (unsigned char)255), 1);
	return;
}

void WriteSentence(BMPGraphics &Target, int I, int J, unsigned char *Word, int len, int Scale) {
	WriteSentence(Target, I, J, Word, len, Color((unsigned char)255, (unsigned char)255, (unsigned char)255), Scale);
	return;
}

void WriteSentenceT(BMPGraphics &Target, int I, int J, unsigned char *Word, int len, Color color, int Scale) {
	int Shift = 0;
	for (; Shift < len; ) {
		if ((Word[Shift] >> 7) & 1) {
			WriteNoteHZK48(Target, I, J + Shift * 24 * Scale, Word + Shift, color, Scale);
			Shift += 2;
		} else {
			WriteNoteASC48(Target, I, J + Shift * 24 * Scale, Word + Shift, color, Scale);
			Shift += 1;
		}
	}
	return;
}

void WriteSentenceT(BMPGraphics &Target, int I, int J, unsigned char *Word, int len) {
	WriteSentenceT(Target, I, J, Word, len, Color((unsigned char)255, (unsigned char)255, (unsigned char)255), 1);
	return;
}

void WriteSentenceT(BMPGraphics &Target, int I, int J, unsigned char *Word, int len, int Scale) {
	WriteSentenceT(Target, I, J, Word, len, 
			Color((unsigned char)255, (unsigned char)255, (unsigned char)255), Scale);
	return;
}