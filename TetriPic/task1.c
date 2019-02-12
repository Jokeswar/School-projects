#include "bmp_header.h"
#include "utils.h"
#include <stdio.h>

// repetitve... I get the piece, transformit in pixels with a given color 
// fill the bmp headers, and write everything to a file.
void task1(void)
{
    bmp_fileheader header;
    bmp_infoheader info;
    size sz;
    size res_sz;
    pixel **pixels;
    char base[6][6];

    getPiece('O', 0, base, &sz);
    pixels = createPiece(base, &sz, &res_sz, YELLOW);
    fillHeader(&header, &info, &res_sz);
    writeBMP(&header, &info, (const pixel **)pixels, "piesa_O.bmp");
    freeMatrix(pixels, res_sz.height);

    getPiece('I', 0, base, &sz);
    pixels = createPiece(base, &sz, &res_sz, BLUE);
    fillHeader(&header, &info, &res_sz);
    writeBMP(&header, &info, (const pixel **)pixels, "piesa_I.bmp");
    freeMatrix(pixels, res_sz.height);

    getPiece('S', 0, base, &sz);
    pixels = createPiece(base, &sz, &res_sz, RED);
    fillHeader(&header, &info, &res_sz);
    writeBMP(&header, &info, (const pixel **)pixels, "piesa_S.bmp");
    freeMatrix(pixels, res_sz.height);

    getPiece('Z', 0, base, &sz);
    pixels = createPiece(base, &sz, &res_sz, GREEN);
    fillHeader(&header, &info, &res_sz);
    writeBMP(&header, &info, (const pixel **)pixels, "piesa_Z.bmp");
    freeMatrix(pixels, res_sz.height);

    getPiece('L', 0, base, &sz);
    pixels = createPiece(base, &sz, &res_sz, ORANGE);
    fillHeader(&header, &info, &res_sz);
    writeBMP(&header, &info, (const pixel **)pixels, "piesa_L.bmp");
    freeMatrix(pixels, res_sz.height);

    getPiece('J', 0, base, &sz);
    pixels = createPiece(base, &sz, &res_sz, PINK);
    fillHeader(&header, &info, &res_sz);
    writeBMP(&header, &info, (const pixel **)pixels, "piesa_J.bmp");
    freeMatrix(pixels, res_sz.height);

    getPiece('T', 0, base, &sz);
    pixels = createPiece(base, &sz, &res_sz, PURPLE);
    fillHeader(&header, &info, &res_sz);
    writeBMP(&header, &info, (const pixel **)pixels, "piesa_T.bmp");
    freeMatrix(pixels, res_sz.height);

    return;
}