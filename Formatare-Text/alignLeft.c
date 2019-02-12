#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "util.h"


int align_left(char text[MAX_LINE_NUMBER][MAX_LINE_LENGHT],
			   unsigned long* lines, char argv[5][20], int argc)
{
	// invalid number of arguments
	if(argc >= 3)
	{
		fprintf(stdout, "Invalid operation!\n");
		return ERR_INVALID_OPERATION;
	}

	int start_line = 0;
	int end_line = *lines - 1;

	if(argc > 0)
		start_line = convertToInt(argv[0]);

	// we ignore the operation if it doesn't make sense
	if(start_line >= (int)*lines)
	{
		fprintf(stdout, "Invalid operation!\n");
		return ERR_INVALID_OPERATION;
	}

	// if we have a second argument
	if(argc == 2)
	{
		end_line = convertToInt(argv[1]);

		if(end_line >= (int)*lines)
			end_line = *lines - 1;
	}

	// either start > end or they aren't numbers or positive
	if(start_line > end_line || start_line < 0 || end_line < 0)
	{
		fprintf(stdout, "Invalid operation!\n");
		return ERR_INVALID_OPERATION;
	}

	removeBeginingWhiteSpaces(text, start_line, end_line);
	
	return 0;
}