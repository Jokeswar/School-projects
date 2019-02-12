#include <stdio.h>
#include "utils.h"

#define INPUT_FILE "cerinta3.in"

// removes full lines in a given matrix
static void removeFullLines(char **board, size board_sz)
{

    for(int i = board_sz.height - 1; i >= 0; i--)
    {
        int count = 0;
        for(int j = 0; j < board_sz.width; j++)
            if(board[i][j] == 0) // count null entries
                count++;

        if(count == 0) // found full line
        {
            for(int j = i; j > 0; j--)
                for(int k = 0; k < board_sz.width; k++)
                    board[j][k] = board[j-1][k]; 

            i++; // stay at the current line
        }
    }

    return;
}

// copies a piece inside a matrix at a given position
// the copied part does not include the border, i.e. we start at 1
// and end at lenght - 2 
static void addPieceAt(int row, int col, char **board,
                       char piece[6][6], size piece_sz)
{
    for(int i = 1; i < piece_sz.height - 1; i++)
        for(int j = 1; j < piece_sz. width - 1; j++)
            if(piece[i][j] != 0) // copy non-null elements; do not overtwrite
                board[row + i - 1][col + j - 1] = piece[i][j];

    return;
}

// given a board in a certain state, a piece and it's column we simulate the
// game from there on
int simulate(char **board, size board_sz, char piece[6][6], 
              size piece_sz, int col)
{
    // calculate the end line taking into account the image size
    // the collision checking does not take into account out of boundary entries
    int end = board_sz.height - 1 - piece_sz.height + 3;

    for(int i = 0; i <= end; i++)
    {
        int count = 0;
        for(int j = 1; j < piece_sz.height - 1; j++)
            for(int k = 1; k < piece_sz.width - 1; k++)
                if(piece[j][k] != 0 && board[i + j - 1][col + k - 1] != 0)
                    count++; // collision

        if(count != 0) // collided, go up a line
        {
            addPieceAt(i - 1, col, board, piece, piece_sz);

            // check for endgame
            for(int k = 0; k < 4; k++)
                for(int l = 0; l < board_sz.width; l++)
                    if(board[k][l] != 0)
                        return 1;

            // removing full lines
            removeFullLines(board, board_sz);

            return 0;
        }
    }

    // got to end of the map and no collision
    addPieceAt(end, col, board, piece, piece_sz);

    // check for endgame
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < board_sz.width; j++)
            if(board[i][j] != 0)
                return 1;

    // removing full lines
    removeFullLines(board, board_sz);
                    
    return 0;
}