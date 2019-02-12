#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tasks.h"
#include "utils.h"

#define BMP_INPUT_FILE  "bonus.bmp"
#define OUTPUT_FILE     "bonus.out"

// compares a piece with a given position in a matrix
// retunrs 1 if not equal, 0 otherwise
static int compare(char **board, int row, int col, char piece[6][6],
                   size sz, size board_sz)
{
    for(int i = 1; i < sz.height - 1; i++)
        for(int j = 1; j < sz.width- 1; j++)
            if(piece[i][j] != 0)
                if(row + i - 1 >= board_sz.height || // out of boundaris
                   col + j - 1 >=  board_sz.width || 
                   piece[i][j] != board[row + i - 1][col + j - 1]) //wrong piece
                    return 1;

    return 0;
}

// deletes a given piece from a matrix
static void deletePiece(char **board, int row, int col, char piece[6][6],
                        size sz)
{
    for(int i = 1; i < sz.height - 1; i++)
        for(int j = 1; j < sz.width- 1; j++)
            if(piece[i][j] != 0) // we don't want to overwrite data
                board[row + i - 1][col + j - 1] = 0;
    
    return;
}

// distance from the corner to the first non-zero value in a piece
// when I find a piece by it's id, I do find it's first non zero value
// I need to know the offset from which I start so I can use the compare
// function corectly
// Eg:
// .# <- I find the the id on the second column but I want to start compare
// ##    from the corner; offset = 1
// #.
static int offset(char piece[6][6], size sz)
{
    int offs = 0;

    for(int j = 1; j < sz.width- 1; j++)
    {
        if(piece[1][j] != 0)
            return offs;

        offs++;
    }

    return 0;
}

void bonus(void)
{
    char output[5001][20];
    char line[20];
    int count = 0;

    char **board = NULL;
    size board_size;

    pixel **pixels = NULL;
    size image_size;

    // create a board from a matrix
    pixels = readBMP(BMP_INPUT_FILE, &image_size);
    board = createBoard(pixels, image_size, &board_size);
    freeMatrix(pixels, image_size.height);

    for(int i = 4; i < board_size.height; i++)
        for(int j  = 0; j < board_size.width; j++)
            if(board[i][j] != 0)
            {
                char piece_id = board[i][j]; // piece id
                char base_piece[6][6];
                size piece_size;
                int rot;

                // compare the piece with all it's rotations
                for(rot = 0; rot <= 270; rot += 90)
                {
                    getPiece(piece_id, rot, base_piece, &piece_size);
                    addID(base_piece, piece_size, piece_id);
                    int offs = offset(base_piece, piece_size);

                    // found the right rotation, add it to the file
                    if(compare(board, i, j - offs, base_piece, piece_size, 
                               board_size) == 0)
                    {
                        sprintf(line, "%c %d %d\n", piece_id, rot, j - offs);
                        strcpy(output[count], line);
                        count++;
                        deletePiece(board, i, j - offs, base_piece, piece_size);
                        break;
                    }
                }
            }

    // open the output file and write the pieces
    FILE *file = fopen(OUTPUT_FILE, "w");
    if(file == NULL) // error checking
    {
        fprintf(stderr, "Could not create file %s\n", OUTPUT_FILE);
        return;
    }

    fprintf(file, "%d\n", count);

    for(int i = 0; i < count; i++)
        fputs(output[i], file);

    fclose(file);

    return;
}