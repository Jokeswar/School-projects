#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"

int paragraph(char text[MAX_LINE_NUMBER][MAX_LINE_LENGHT], unsigned long* lines,
              char argv[5][20], int argc)
{
    // wrong number of arguments
    if(argc > 3 || argc < 1)
    {
		fprintf(stdout, "Invalid operation!\n");
		return ERR_INVALID_OPERATION;
	}

    // default values in case there are no optional parameters
    int indent_lenght = convertToInt(argv[0]);
    int start_line = 0;
    int end_line = (int)(*lines - 1);

    if(indent_lenght > 8)
    {
        fprintf(stdout, "Invalid operation!\n");
		return ERR_INVALID_OPERATION;
    }

    // getting optional arguments, if they exist
    if(argc >= 2)
        start_line = convertToInt(argv[1]);

    if(argc == 3)
    {
        end_line = convertToInt(argv[2]);

        if(end_line >= (int)*lines)
			end_line = *lines - 1;
    }

    // we ignore the operation if it doesn't make sense
	if(start_line >= (int)*lines)
	{
		fprintf(stdout, "Invalid operation!\n");
		return ERR_INVALID_OPERATION;
	}

    // either start > end or they aren't numbers or they aren't positive
	if(start_line > end_line || start_line < 0 || 
       end_line < 0 || indent_lenght < 0)
	{
		fprintf(stdout, "Invalid operation!\n");
		return ERR_INVALID_OPERATION;
	}

    int i;
    for(i = start_line; i <= end_line; i++)
    {
        // special case for the first paragraph if it is in range
        if(i == 0)
        {
            // corectly paragraph the text
            removeBeginingWhiteSpaces(text, 0, 0);
            addWhiteSpacesAtPosition(text[i], 0, indent_lenght);
        }
        else
            if(i > 0 && text[i-1][0] == '\n' && text[i-2][strlen(text[i-2])-1] == '\n')
            {
                // corectly paragraph the text
                removeBeginingWhiteSpaces(text, i, i);
                addWhiteSpacesAtPosition(text[i], 0, indent_lenght);
            }
    }

    return 0;
}