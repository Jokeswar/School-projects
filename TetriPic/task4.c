#include <stdio.h>
#include <stdlib.h>
#include "tasks.h"
#include "utils.h"

#define BMP_INPUT_FILE  "cerinta4.bmp"
#define INPUT_FILE      "cerinta4.in"
#define OUTPUT_FILE     "task4.bmp"

void task4(void)
{
    FILE *file = fopen(INPUT_FILE, "r");
    if(file == NULL) // error checking
    {
        fprintf(stderr, "Could not open file %s\n", INPUT_FILE);
        return;
    }

    int number_of_moves;
    char **board = NULL;
    size board_size;

    fscanf(file, "%d %d %d\n", &number_of_moves, 
           &board_size.height, &board_size.width);

    pixel **pixels = NULL;
    size image_size;

    // creates the initial board from a given bmp
    pixels = readBMP(BMP_INPUT_FILE, &image_size);
    board = createBoard(pixels, image_size, &board_size);
    freeMatrix(pixels, image_size.height);

    // remove white spaces
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < board_size.width; j++)
                board[i][j] = 0; 
 
    for(int i = 0; i < number_of_moves; i++)
    {
        char piece_ID;
        int rotation;
        int col;

        char base[6][6];
        size base_size;

        fscanf(file ,"%c%d%d", &piece_ID, &rotation, &col);
        fgetc(file); // ignores \n at the end of every line
        
        // getting the piece
        getPiece(piece_ID, rotation, base, &base_size);
        addID(base, base_size, piece_ID);
        int end = simulate(board, board_size, base, base_size, col); // endgame?
        if(end == 1)
            break;
    }

    fclose(file);

    // add whitespace id
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < board_size.width; j++)
            if(board[i][j] == 0)
                board[i][j] = 1; 
    
    bmp_fileheader header;
    bmp_infoheader info;
    
    // creates and writes image to a file
    pixels = createImage(board, board_size, &image_size);
    fillHeader(&header, &info, &image_size);
    writeBMP(&header, &info, (const pixel **)pixels, OUTPUT_FILE);

    // free memory
    freeMatrix(pixels, image_size.height);
    for(int i = 0; i < board_size.height; i++)
        free(board[i]);
    free(board);

    return;
}