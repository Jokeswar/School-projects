#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "tasks.h"
#include "utils.h"

#define INPUT_FILE  "cerinta3.in"
#define OUTPUT_FILE "task3.bmp"

void task3(void)
{
    FILE *file = fopen(INPUT_FILE, "r");
    if(file == NULL) // error checking
    {
        fprintf(stderr, "Could not open file %s\n", INPUT_FILE);
        return;
    }

    int number_of_moves;
    size board_size;

    fscanf(file, "%d %d %d\n", &number_of_moves, 
           &board_size.height, &board_size.width);

    board_size.height += 4; // zone for white space at the top

    char **board = (char **)malloc((board_size.height)* sizeof(char *));
    assert(board != NULL);
    for(int i = 0; i < board_size.height; i++)
    {
        board[i] = (char *)calloc(board_size.width, sizeof(char)); 
        assert(board[i] != NULL);
    }

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
    
    size image_sz;
    bmp_fileheader header;
    bmp_infoheader info;
    
    // creates and writes image to a file
    pixel **pixels = createImage(board, board_size, &image_sz);
    fillHeader(&header, &info, &image_sz);
    writeBMP(&header, &info, (const pixel **)pixels, OUTPUT_FILE);

    // free memory
    freeMatrix(pixels, image_sz.height);
    for(int i = 0; i < board_size.height; i++)
        free(board[i]);
    free(board);

    return;
}