#include "utils.h"
#include "bmp_header.h"
#include <stdio.h>
#include <stdlib.h>

// I get the piece, transformit in pixels with a given color 
// fill the bmp headers, and write everything to a file.
// does this for every rotation
static void writeRotatedPiece(int color, char id)
{
    bmp_fileheader header;
    bmp_infoheader info;
    size sz;
    size res_sz;
    pixel **pixels;
    char filename[30];
    char base[6][6];

    sprintf(filename, "piesa_%c_90.bmp", id);
    getPiece(id, 90, base, &sz);
    pixels = createPiece(base, &sz, &res_sz, color);
    fillHeader(&header, &info, &res_sz);
    writeBMP(&header, &info, (const pixel **)pixels, filename);
    freeMatrix(pixels, res_sz.height);

    sprintf(filename, "piesa_%c_180.bmp", id);
    getPiece(id, 180, base, &sz);
    pixels = createPiece(base, &sz, &res_sz, color);
    fillHeader(&header, &info, &res_sz);
    writeBMP(&header, &info, (const pixel **)pixels, filename);
    freeMatrix(pixels, res_sz.height);

    sprintf(filename, "piesa_%c_270.bmp", id);
    getPiece(id, 270, base, &sz);
    pixels = createPiece(base, &sz, &res_sz, color);
    fillHeader(&header, &info, &res_sz);
    writeBMP(&header, &info, (const pixel **)pixels, filename);
    freeMatrix(pixels, res_sz.height);

    return;
}

// repetitve... yet I use funtions
void task2(void)
{
    writeRotatedPiece(YELLOW, 'O');
    writeRotatedPiece(BLUE, 'I');
    writeRotatedPiece(RED, 'S');
    writeRotatedPiece(GREEN, 'Z');
    writeRotatedPiece(ORANGE, 'L');
    writeRotatedPiece(PINK, 'J');
    writeRotatedPiece(PURPLE, 'T');

    return;
}