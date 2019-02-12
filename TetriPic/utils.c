#include "bmp_header.h"
#include "utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

// integer -> pixel
// easy to store, easy to use
void setPixelColor(int color, pixel *pix)
{
    //  color = R G B
    pix->red = color;
    pix->green = color >> 8;
    pix->blue = color >> 16;

    return;
}

// assigns values to a size struct
void setSize(size *sz, unsigned int width, unsigned int height)
{
    sz->height = height;
    sz->width = width;

    return;
}

// given the size, calculate the size of an image
unsigned int getImageSize(int width, int height)
{
    if(width % 4 == 0)
        return width * height * 3;
    else
        return ((width * 3 / 4) + 1) * 4 * height;
}

// simple free method
void freeMatrix(pixel **pixels, unsigned int height)
{
    for(unsigned int i = 0; i < height; i++)
        free(pixels[i]);
    
    free(pixels);
    return;
}

// makes a 10X10 square a specified color in the pixels matrix
void setPixelSquareAt(int col, int row, int color, pixel **pixels)
{
    // translate to pixel array
    col *= 10;
    row *= 10;

    for(int i = row; i < row + SQUARE_LENGHT; i++)
        for(int j = col; j < col + SQUARE_LENGHT; j++)
            setPixelColor(color, &pixels[i][j]);

    return;
}

// creates a matrix representing a pice from a string that has
// a specific format
static void getBasePiece(char *string, char base[6][6], size *sz)
{
    int len = strlen(string);
    int row = 0;
    int col = 0;

    for(int i = 0; i < len; i++)
        if(string[i] != '\n')
        {
            base[row][col] = string[i] - '0';
            col++;
        }
        else
        {
            row++;
            col = 0;
        }

    sz->height = row + 1;
    sz->width = col;

    return;
}

// creates a bmp file with the image data pixels
void writeBMP(const bmp_fileheader *header, const bmp_infoheader *info,
              const pixel **pixels, const char *filename)
{
    FILE *file = fopen(filename, "wb");
    if(file == NULL) // error checking
    {
        fprintf(stderr, "Could not create file %s\n", filename);
        return;
    }

    int width = info->width;
    int height = info->height;
    int pad_count = getImageSize(width, 1) - width * 3; // line padding
    char *pad = "\0\0\0\0";

    fwrite(header, sizeof(bmp_fileheader), 1, file);
    fwrite(info, sizeof(bmp_infoheader), 1, file);

    for(int i = height - 1; i >= 0; i--)
    {
        // write line and padding
        fwrite(pixels[i], sizeof(pixel), width, file);
        fwrite(pad, 1, pad_count, file);
    }
    fclose(file);

    return;
}

// fills header information based on the size of the image provide through s
void fillHeader(bmp_fileheader *header, bmp_infoheader *info, size *s)
{
    header->fileMarker1 = 'B';
    header->fileMarker2 = 'M';
    header->bfSize = FILE_HEADER_SIZE; // 54
    header->unused1 = 0;
    header->unused2 = 0;
    header->imageDataOffset = FILE_HEADER_SIZE; // 54

    info->biSize = sizeof(bmp_infoheader);
    info->width = s->width;
    info->height = s->height;
    info->planes = NUMBER_OF_PLANES; // 1
    info->bitPix = BITS_PER_PIXEL; // 24
    info->biCompression = 0;
    info->biSizeImage = getImageSize(info->width, info->height);
    info->biXPelsPerMeter = 0;
    info->biYPelsPerMeter = 0;
    info->biClrUsed = 0;
    info->biClrImportant = 0;

    header->bfSize += info->biSizeImage;

    return;
}

// from a given base creates an image rotated 0 degrees, with a specific 
// color and returns it's size in res_sz and the image itself
pixel **createPiece(char base[6][6], size *sz, size *res_sz, int color)
{
    res_sz->height = sz->height * 10;
    res_sz->width = sz->width * 10;

    // allocating memory
    pixel **pixels = (pixel **)malloc(res_sz->height * sizeof(pixel *));
    assert(pixels != NULL);
    for(int i = 0; i < res_sz->height; i++)
    {
        pixels[i] = (pixel *)malloc(res_sz->width * sizeof(pixel));
        assert(pixels[i] != NULL);
    }

    // placing colors
    for(int i = 0; i < sz->height; i++)
        for(int j = 0; j < sz->width; j++)
            if(base[i][j] == 0)
                setPixelSquareAt(j, i, WHITE, pixels);
            else
                setPixelSquareAt(j, i, color, pixels);

    return pixels;
}

// rotates a given piece 
static void getRotatedBasePiece(char *string, int rotation,
                                char base[6][6], size *sz)
{
    char base_copy[6][6];
    size base_size;

    getBasePiece(string, base_copy, &base_size);
    if(rotation == 0 || rotation == 180) // same size
        setSize(sz, base_size.width, base_size.height);
    else // size values inverted
        setSize(sz, base_size.height, base_size.width);

    for(int i = 0; i < base_size.height; i++)
        for(int j = 0; j < base_size.width; j++)
            switch(rotation) // copy based on the rotation
            {
                case 0:
                    base[i][j] = base_copy[i][j];
                    break;

                case 90:
                    base[j][base_size.height - i - 1] = base_copy[i][j];
                    break;

                case 180:
                    base[base_size.height - i - 1][base_size.width - j - 1] =
                    base_copy[i][j];
                    break;

                case 270:
                    base[base_size.width - j - 1][i] = base_copy[i][j];
                    break;
            }

    return;
}

// function that gets a piece based on the letter and rotation
void getPiece(char ID, int rotation, char base[6][6], size *sz)
{
    switch(ID)
    {
        case 'O':
            getRotatedBasePiece(PIECE_O, rotation, base, sz);
            return;
        
        case 'I':
            getRotatedBasePiece(PIECE_I, rotation, base, sz);
            return;

        case 'S':
            getRotatedBasePiece(PIECE_S, rotation, base, sz);
            return;

        case 'Z':
            getRotatedBasePiece(PIECE_Z, rotation, base, sz);
            return;

        case 'L':
            getRotatedBasePiece(PIECE_L, rotation, base, sz);
            return;

        case 'J':
            getRotatedBasePiece(PIECE_J, rotation, base, sz);
            return;

        case 'T':
            getRotatedBasePiece(PIECE_T, rotation, base, sz);
            return;
    }
    return;
}


// adds uniq id for every piece so we know what color goes where
void addID(char base[6][6], size sz, char id)
{
    for(int i = 0; i < sz.height; i++)
        for(int j = 0; j < sz.width; j++)
            if(base[i][j] != 0)
                base[i][j] += id - 1;

    return;
}

// makes it easy to use 
int idToColor(char id)
{
    switch(id)
    {
        case 'O':
            return YELLOW;

        case 'I':
            return BLUE;

        case 'S':
            return RED;

        case 'Z':
            return GREEN;

        case 'L':
            return ORANGE;

        case 'J':
            return PINK;

        case 'T':
            return PURPLE;
        
        case 0:
            return BLACK;

        case 1:
            return WHITE;
    }

    return -1; // invalid id
}

// creates an image from a given board
pixel **createImage(char **board, size sz, size *res_sz)
{
    res_sz->height = sz.height * 10;
    res_sz->width = sz.width * 10;

    // allocating memory
    pixel **pixels = (pixel **)malloc(res_sz->height * sizeof(pixel *));
    assert(pixels != NULL);
    for(int i = 0; i < res_sz->height; i++)
    {
        pixels[i] = (pixel *)malloc(res_sz->width * sizeof(pixel));
        assert(pixels[i] != NULL);
    }

    // placing colors
    for(int i = 0; i < sz.height; i++)
        for(int j = 0; j < sz.width; j++)
        {
            int color = idToColor(board[i][j]);
            setPixelSquareAt(j, i, color, pixels);
        }

    return pixels;
}

// reads a bmp and returns it's pixels' data
pixel **readBMP(char * filename, size *sz)
{
    bmp_fileheader header;
    bmp_infoheader info;

    FILE *file = fopen(filename, "rb");
    if(file == NULL) // error checking
    {
        fprintf(stderr, "Could not open file %s\n", filename);
        return NULL;
    }

    // mostly useless information
    fread(&header, sizeof(bmp_fileheader), 1, file);
    fread(&info, sizeof(bmp_infoheader), 1, file);

    sz->width = info.width;
    sz->height = info.height;
    int pad_count = (4 - sz->width * 3 % 4) % 4; // padding

    // allocating memory for pixels
    pixel **pixels = (pixel **)malloc(sz->height * sizeof(pixel *));
    assert(pixels != NULL);
    for(int i = 0; i < sz->height; i++)
    {
        pixels[i] = (pixel *)malloc(sz->width * sizeof(pixel));
        assert(pixels != NULL);
    }

    for(int i = sz->height - 1; i >= 0; i--)
    {
        fread(pixels[i], sizeof(pixel), sz->width, file);
        fseek(file, pad_count, SEEK_CUR);
    }

    return pixels;
}

int getPixelColor(pixel pix)
{
    return pix.red + (pix.green << 8) + (pix.blue << 16);
}

// the inverse of idToColor
char colorToID(int color)
{
    switch(color)
    {
        case BLACK:
            return 0;

        case WHITE:
            return 1;
        
        case YELLOW:
            return 'O';

        case BLUE:
            return 'I';

        case RED:
            return 'S';

        case GREEN:
            return 'Z';

        case ORANGE:
            return 'L';

        case PINK:
            return 'J';

        case PURPLE:
            return 'T';
    }

    return -1;
}

// from pixels it is created a board with values coresponding to each piece id
char **createBoard(pixel **pixels, size image_sz, size *board_sz)
{
    board_sz->width = image_sz.width / 10;
    board_sz->height = image_sz.height / 10;

    char **board = (char **)malloc(board_sz->height * sizeof(char *));
    assert(board != NULL);
    for(int i = 0; i < board_sz->height; i++)
    {
        board[i] = (char *)malloc(board_sz->width * sizeof(char));
        assert(board[i] != NULL);
        for(int j = 0; j < board_sz->width; j++)
        {
            int color = getPixelColor(pixels[i * 10][j * 10]);
            int id = colorToID(color);
            board[i][j] = id;
        }
    }

    return board;
}