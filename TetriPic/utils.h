#include "bmp_header.h"

#ifndef UTILS_H
#define UTILS_H

#define SQUARE_LENGHT       10
#define FILE_HEADER_SIZE    54
#define BITS_PER_PIXEL      24
#define NUMBER_OF_PLANES    1

#define BLACK   0x000000
#define WHITE   0xffffff
#define BLUE    0xff0000
#define GREEN   0x00ff00
#define RED     0x0000ff
#define YELLOW  0x00ffff
#define ORANGE  0x008cff
#define PINK    0xff00ff
#define PURPLE  0xff0082

#define PIECE_O "\
0000\n\
0110\n\
0110\n\
0000\0"

#define PIECE_I "\
000\n\
010\n\
010\n\
010\n\
010\n\
000\0"

#define PIECE_S "\
00000\n\
00110\n\
01100\n\
00000\0"

#define PIECE_Z "\
00000\n\
01100\n\
00110\n\
00000\0"

#define PIECE_L "\
0000\n\
0100\n\
0100\n\
0110\n\
0000\0"

#define PIECE_J "\
0000\n\
0010\n\
0010\n\
0110\n\
0000\0"

#define PIECE_T "\
00000\n\
01110\n\
00100\n\
00000\0"

#pragma pack(1)

typedef struct
{
    unsigned char blue;
    unsigned char green;
    unsigned char red;
} pixel;

#pragma pack()

typedef struct
{
    signed int width;
    signed int height;
} size;


int idToColor(char id);
char colorToID(int color);
int getPixelColor(pixel pix);
void setPixelColor(int color, pixel *pix);
pixel **readBMP(char * filename, size *sz);
void addID(char base[6][6], size sz, char id);
unsigned int getImageSize(int width, int height);
void freeMatrix(pixel **pixels, unsigned height);
pixel **createImage(char **board, size sz, size *res_sz);
void getPiece(char ID, int rotation, char base[6][6], size *sz);
void setSize(size *sz, unsigned int width, unsigned int height);
char **createBoard(pixel **pixels, size image_sz, size *board_sz);
void setPixelSquareAt(int col, int row, int color, pixel **pixels);
void fillHeader(bmp_fileheader *header, bmp_infoheader *info, size *s);
void writeBMP(const bmp_fileheader *header, const bmp_infoheader *info,
              const pixel **pixels, const char *filename);
pixel **createPiece(char base[6][6], size *sz, size *res_sz, int color);

#endif