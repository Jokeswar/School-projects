#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"

int justify(char text[MAX_LINE_NUMBER][MAX_LINE_LENGHT], unsigned long* lines,
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

    removeBeginingWhiteSpaces(text, start_line, end_line);

    int max_lenght = 0;
    int i;
    for(i = 0; i < (int)*lines; i++)
        if((int)strlen(text[i]) > max_lenght)
            max_lenght = strlen(text[i]);

    for(i = start_line; i <= end_line; i++)
    {
        int len = strlen(text[i]);
        if(text[i][0] != '\n' && (text[i + 1][0] != '\n'))
        {
            int spaces_needed = max_lenght - len;
            if(spaces_needed != 0)
            {
                int words = countWords(text[i]);
                int minimum_spaces = spaces_needed / (words - 1);
                int rest_spaces = spaces_needed % (words - 1);
                char* space = strchr(text[i], ' ');

                while (space != NULL)
                {
                    addWhiteSpacesAtPosition(space, 0, minimum_spaces);
                    if(rest_spaces > 0)
                    {
                        addWhiteSpacesAtPosition(space, 0, 1);
                        rest_spaces--;
                    }
                
                    space = strchr(space + minimum_spaces + 2 , ' ');
                }
            }
        } 
    }

    return 0;
}