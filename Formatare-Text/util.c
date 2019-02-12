#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "util.h"

// converts to an long the first number in a given string
int convertToInt(char* string)
{
	long value = LONG_MIN;
	char* current_character = string;

	// extracting arguments value for every command
	while(*current_character != '\0')
	{
		if(isdigit(*current_character) || 
		  (*current_character == '-' && isdigit(*(current_character + 1))))
		{
			value = strtol(current_character, &current_character, 10);
			return value;
		}
		else
			current_character++;
	}

	return (int)value;
}

void removeTrailingWhiteSpaces(char text[MAX_LINE_NUMBER][MAX_LINE_LENGHT],
							   unsigned int start,  unsigned int end)
{
	unsigned int i;
	for(i = start; i <= end; i++)
	{
		int len = strlen(text[i])-2;

		// delete the current white character until you find a differ character
		while(text[i][len] == ' ' || text[i][len] == '\t')
		{
			strcpy(text[i] + len, text[i] + len + 1);
			len--;
		}
	}

	return;
}

void removeBeginingWhiteSpaces(char text[MAX_LINE_NUMBER][MAX_LINE_LENGHT],	
						       unsigned int start, unsigned int end)
{
	unsigned int i;
	for(i = start; i <= end; i++)
	{
		// delete the current white character until you find a differ character
		while(text[i][0] == ' ' || text[i][0] == '\t')
			memmove(text[i], text[i] + 1, 
					strlen(text[i]));
	}

	return;
}

void copyMatrix(char destination[MAX_LINE_NUMBER][MAX_LINE_LENGHT],
				char source[MAX_LINE_NUMBER][MAX_LINE_LENGHT], int lines)
{
	for(int i = 0; i < lines; i++)
		strcpy(destination[i], source[i]);

	return;
}

// adds a number of white spaces at a given position in a string
void addWhiteSpacesAtPosition(char sentence[MAX_LINE_LENGHT], int start,
							  int count)
{
	char aux[1000];

	// shifting the right half of the sentence
	strcpy(aux, sentence + start);
	strcpy(sentence + start + count, aux);

	int j;
	for(j = 0; j < count; j++)
		sentence[j + start] = ' ';

	return;
}

// writes given text to a file
int writeToFile(char* filename, char text[MAX_LINE_NUMBER][MAX_LINE_LENGHT], 
				unsigned long* lines)
{
	// Open output file for writing
  	FILE *output = fopen(filename, "w");

	if(output == NULL) 
	{
	    fprintf(stderr, "Could not open or create file %s.\n",filename);
	    return -1;
	}

	int i;
	// Write result in output file
	for (i = 0; i < (int)*lines; i++)
		fputs(text[i], output);

	fclose(output);
	return 0;
}

// sorting text from line start to line end 
// sorting alphabetically <=> sort_type = 1
// sorting reverse alphabetically <=> sort_type = -1;
void sortLines(char text[MAX_LINE_NUMBER][MAX_LINE_LENGHT], int start, int end,
			   int sort_type)
{
	char temp[MAX_LINE_LENGHT];
	int i, j;
	for(i = start; i < end; i++)
        for(j = i+1; j <= end; j++)
        {
            if(strcmp(text[i], text[j]) * sort_type > 0)
            {
                strcpy(temp, text[i]);
                strcpy(text[i], text[j]);
                strcpy(text[j], temp);
            }
        }
	return;
}

// counts the number of words in a given sentence
int countWords(char sentence[MAX_LINE_LENGHT])
{
	int count = 0;

	int i;
	for(i = 0; i < (int)strlen(sentence); i++)
		if(sentence[i] == ' ')
			count++;

	// in case we have word \n
	// there is just one word
	if(sentence[i -2] != ' ')
		count++;
	
	return count ;
}