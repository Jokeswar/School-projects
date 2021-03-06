#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"

int list(char text[MAX_LINE_NUMBER][MAX_LINE_LENGHT], unsigned long* lines,
         char argv[5][20], int argc)
{
    // wrong number of arguments
    if(argc < 2 || argc > 4)
    {
        fprintf(stdout, "Invalid operation!\n");
        return ERR_INVALID_OPERATION;
    }

    // invalid list type or special character
    if(strlen(argv[0]) != 1 || strlen(argv[1]) != 1)
    {
        fprintf(stdout, "Invalid operation!\n");
        return ERR_INVALID_OPERATION;
    }

    // default values
    char list_type = argv[0][0];
    char special_character = argv[1][0];
    int start_line = 0;
	int end_line = *lines - 1;
    int number;

    // setting up the list type
    switch(list_type)
    {
        case 'n':
            number = 1;
            break;
        case 'a':
            number = 'a';
            break;
        case 'A':
            number = 'A';
            break;
        case 'b':
            break;
        default:
            fprintf(stdout, "Invalid operation!\n");
		    return ERR_INVALID_OPERATION;
    }

	if(argc > 2)
		start_line = convertToInt(argv[2]);

	// we ignore the operation if it doesn't make sense
	if(start_line >= (int)*lines)
	{
		fprintf(stdout, "Invalid operation!\n");
		return ERR_INVALID_OPERATION;
	}

	// if we have a second argument
	if(argc == 4)
	{
		end_line = convertToInt(argv[3]);

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
    
    char buff[MAX_LINE_LENGHT];
    int i;
    for(i = start_line; i <= end_line; i++)
    {
        switch(list_type)
        {
            case 'n':
                sprintf(buff, "%d%c %s", number,
                        special_character, text[i]);
                number++;
                break;
            case 'a':
            case 'A':
                sprintf(buff, "%c%c %s", number,
                        special_character, text[i]);
                number++;
                break;
            case 'b':
                sprintf(buff, "%c %s", special_character, text[i]);
                break;
        }
        strcpy(text[i], buff);
    }

    return 0;
}
