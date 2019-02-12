#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"

int center(char text[MAX_LINE_NUMBER][MAX_LINE_LENGHT], unsigned long* lines,
           char argv[5][20], int argc)
{
    if(argc > 2)
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

    // calculating reference lenght
	int i;
	unsigned int max_line = 0;
	for(i = 0; i < (int)*lines; i++)
		if(strlen(text[i]) > max_line)
			max_line = strlen(text[i]);

    for(i = start_line; i <= end_line; i++)
	{
		int len = strlen(text[i]);

		if(text[i][0] != '\n')
		{	
            // calculate the ammount of spaces needed
            int diff = (int)max_line - len;
			int number_of_spaces = diff/2 + diff % 2;

			addWhiteSpacesAtPosition(text[i], 0, number_of_spaces);
		}
	}

    return 0;
}