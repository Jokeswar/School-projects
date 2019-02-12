/*
 * Nume: Baruta Daniel Mihail
 * Grupa: 311 CD
 */

#include <stdio.h>
#include <math.h>

#define LINE 0xff

#define min(x, y) x < y ? x : y

typedef unsigned long long uint64_t;

void printMap(uint64_t map)
{
	uint64_t one = 1;
	uint64_t i, j; 
	for(i = (one << 63), j = 1; i > 0; i = (i >> 1), j++)
	{
		if((i & map) == 0) 
			printf(".");
		if((i & map) != 0)
			printf("#");
		if(j % 8 == 0)
			printf("\n");
	}
	printf("\n");
}

uint64_t setBitAt(uint64_t x, int pos)
{
	uint64_t one = 1;
	return (x | (one << pos));
}

uint64_t getBitAt(uint64_t x, int pos)
{
	uint64_t mask = 0;
	mask = setBitAt(mask, pos);
	return (x & mask);
}

uint64_t setByteAt(uint64_t x, int line)
{
	uint64_t mask = LINE;
	mask = (mask << (8 * line));
	return (x | mask);
}

/*
 * Makes the first bit of every bite 1 and shifts them to the right
 * Uses and to find the left most bit set
 */
int getLeftBoundary(uint64_t piece)
{
	uint64_t mask = 0;
	int pos = 7;

	int i;
	for(i = 0; i < 8; i++)
		mask = setBitAt(mask, i*8 + 7);
	
	while((mask & piece) == 0)
	{
		pos = pos - 1;
		mask = mask >> 1;
	}

	return 7 - pos;
}

/*
 * Makes the first bit of every bite 1 and shifts them to the left
 * Uses and to find the right most bit set
 */
int getRightBoundary(uint64_t piece)
{
	uint64_t mask = 0;
	int pos = 0;

	int i;
	for(i = 0; i < 8; i++)
		mask = setBitAt(mask, i*8);

	while((mask & piece) == 0)
	{
		pos = pos + 1;
		mask = mask << 1;
	}

	return pos;
}

/*
 * Gets the maximum distance to the left it can go if there is a block
 */
int leftBlock(uint64_t piece, uint64_t map)
{
	int i = -1;
	int maxSpaceLeft = getLeftBoundary(piece);

	while((piece & map) == 0)
	{
		if(i > maxSpaceLeft)
			break; // We passed the map's edge

		piece = piece << 1;
		i++;
	}
	
	return i;
}

/*
 * Gets the maximum distance to the right it can go if there is a block
 */
int rightBlock(uint64_t piece, uint64_t map)
{
	int i = -1;
	int maxSpaceRight = getRightBoundary(piece);

	while((piece & map) == 0)
	{
		if(i > maxSpaceRight)
			break; // We passed the map's edge

		piece = piece >> 1;
		i++;
	}

	return i;
}

float getScore(unsigned int lines, uint64_t map)
{
	int zeroes = 0;
	uint64_t one = 1;
	
	uint64_t i;
	for(i = (one << 63); i > 0; i = (i >> 1))
		if((map & i) == 0) zeroes++;
	
	return (sqrt((float)zeroes) + pow(1.25, lines));
}

int main(void)
{
	uint64_t map;
	uint64_t piece;
	unsigned int numberOfPieces;
	unsigned int fullLines;
	int move;
	
	scanf("%llu", &map);
	scanf("%d", &numberOfPieces);

	printMap(map);
	fullLines = 0;

	unsigned int i;
	for(i = 0; i < numberOfPieces; i++)
	{
		scanf("%llu", &piece);
		uint64_t newPiece;
		
		int j;	
		for(j = 0; j < 8; ++j)
		{
			scanf("%d", &move);
					
			newPiece = (piece << 8*(7 - j));
			
			if((map & newPiece) != 0) // if things collide during moves
			{
				newPiece = newPiece << 8; // reset the move
				map = map | newPiece;

				if(j == 0 || (j == 1 && ((piece & (LINE << 8)) != 0)))
				{
					// Game Over
					printMap(map);
					printf("GAME OVER!\n");
					printf("Score:%.2f\n", getScore(fullLines, map));
					return 0;
				}
				
				int k;
				for(k = j + 1; k < 8; k++)
					scanf("%d", &move); // clearing the buffer

				break;
			}
			
			if(move < 0)
			{
				move = move * (-1); // shifting ==> move must be +
				// max distance it can go without leaving the map
				int maxLeftFree = getLeftBoundary(newPiece);

 				// max distance it can go if there is a block; 
				// coincides with maxLeftFree if none
				int maxLeftBlock = leftBlock(newPiece, map);
				int maxLeftMove = min(maxLeftFree, maxLeftBlock);
				newPiece = (newPiece << (min(maxLeftMove, move)));
				piece = (piece << (min(maxLeftMove, move))); // update the template
			}
			else
			{
				// max distance it can go without leaving the map
				int maxRightFree = getRightBoundary(newPiece); 
				
				// max distance it can go if there is a block;
				// coincides with maxRightFree if none
				int maxRightBlock = rightBlock(newPiece, map);
				int maxRightMove = min(maxRightFree, maxRightBlock);
				newPiece = (newPiece >> (min(maxRightMove, move)));
				piece = (piece >> (min(maxRightMove, move))); // update the template
			}
		
			if((map & newPiece) == 0)
			{
				printMap(map | newPiece);

				// after last iteration add the piece to the map
				if(j == 7) 
					map = map | newPiece; 			
			}
		}

		char linesDeleted = 0;

		// after all moves are over check for full lines
		int k;
		for(k = 0; k < 7; k++)
		{
			uint64_t line = LINE;
			int lineNumber = 0;

			while((line & map) != line)
			{
				lineNumber++;
				line = (line << 8);

				if(lineNumber > 7)
					break;
			}
		
			if(lineNumber > 7)
				break;

			// if there are lines deleted change increment and print the map at the end
			linesDeleted++; 
			map = map ^ line; // remove full line
			
			uint64_t maskTop = 0;
			int j;
			for(j = 0; j <= lineNumber; ++j)
				maskTop = setByteAt(maskTop, j);
			maskTop = ~maskTop; // all 1s above the deleted line
		
			uint64_t mapTop = map & maskTop; // save top part;
			map = (map ^ mapTop); // delete the top part
			mapTop = mapTop >> 8; // shift if over to the deleted line
			map = (map | mapTop); // add the shifted part back to the map
		}

		if(linesDeleted != 0)
			printMap(map);

		fullLines = fullLines + linesDeleted;
	}

	printf("GAME OVER!\n");
	printf("Score:%.2f\n", getScore(fullLines, map));

	return 0;
}
