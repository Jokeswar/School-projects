#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "util.h"

int wrap(char text[MAX_LINE_NUMBER][MAX_LINE_LENGHT], unsigned long* lines, 
		 char argv[5][20], int argc)
{
	// wrong number of arguments
	if(argc != 1)
	{
		fprintf(stdout, "Invalid operation!\n");
		return ERR_INVALID_OPERATION;
	}

	// we add \n so the algorithm adds a new line 
	// at the end of the last line of the text
	text[*lines][0]='\n';
	text[*lines][1]='\0';
	(*lines)++;

	int max_line_lenght = convertToInt(argv[0]);
	char buffer[MAX_LINE_NUMBER][MAX_LINE_LENGHT];

	// make it empty, just in case
	for(int i = 0; i < MAX_LINE_NUMBER; i++)
		for(int j = 0; j < MAX_LINE_LENGHT; j++)
			buffer[i][j] = '\0';

	// negative number for wrap or invalid argument
	if(max_line_lenght <= 0)
	{
		fprintf(stdout, "Invalid operation!\n");
		return ERR_INVALID_OPERATION;
	}

	removeTrailingWhiteSpaces(text, 0, (*lines) - 1);

	unsigned int current_line = 0;
	unsigned int i;
	for(i = 0; i < *lines; i++)
	{
		char* result = strtok(text[i], " ");

		// keep indentation of the first line of the text
		// no need for new line
		if(i == 0)
		{
			int count = 0;
			while(text[i][count] == ' ')
				count++;

			addWhiteSpacesAtPosition(buffer[current_line], 0,
									 count%max_line_lenght);
		}

		// solving line indents for paragraphs
		if(i > 0 && text[i - 1][0] == '\n' && text[i][0] == ' ')
		{
			// count white spaces
			int count = 0;
			while(text[i][count] == ' ')
				count++;

			// max_line_lenght of ' ' means a new line
			int add_newline = count / max_line_lenght;
			if(add_newline != 0)
			{
				buffer[current_line-1][strlen(buffer[current_line-1])-1] = '\n';
				current_line++;
			}
			
			for(int i = 0; i < add_newline; i++)
			{
				buffer[current_line-1][0] = '\n';
				current_line++;
			}

			// add the rest of the spaces
			addWhiteSpacesAtPosition(buffer[current_line], 0, 
									 MAX(count % max_line_lenght - 1, 0));
		}

		// processing word
		while(result != NULL)
		{
			int current_line_lenght = strlen(buffer[current_line]);
			int current_word_lenght = strlen(result);
	
			// word biggr than max_line_lenght
			if(current_word_lenght > max_line_lenght)
			{
				fprintf(stdout, "Cannot wrap!\n");
				return ERR_INVALID_OPERATION;
			}

			if(result[0] == '\n' && current_word_lenght == 1)
			{
				// remove space from last character and add \n
				buffer[current_line][current_line_lenght - 1] = '\n';
				current_line++;
				
				//keep the \n and move to another line
				strcat(buffer[current_line], "\n");
				current_line++;
			}
			else
			{
				// remove new line character from word
				if(result[current_word_lenght - 1] == '\n')
				{
					result[current_word_lenght - 1] = '\0';
					current_word_lenght--;
				}

				// calculation for ease of use
				int new_lenght = current_word_lenght + current_line_lenght;

				if(new_lenght > max_line_lenght)
				{
					// remove space
					buffer[current_line][current_line_lenght - 1] = '\n';
					current_line++;

					// reset lenghts 
					new_lenght = current_word_lenght;
					current_line_lenght = 0;
				}

				// if word fits, add it
				if(new_lenght <= max_line_lenght)
				{
					strcat(buffer[current_line], result);
					strcat(buffer[current_line], " "); 
				}

			}

			// retrieving next word
			result = strtok(NULL, " ");
		}
	}

	// because we have an extra '\n' character that we double in last the
	// iteration we remove it
	current_line--;

	*lines = current_line;
	for(i = 0; i < *lines; i++)
		strcpy(text[i], buffer[i]);

	return 0;
}