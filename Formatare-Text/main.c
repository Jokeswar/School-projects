/*
 * NUME: BARUTA DANIEL MIHAIL
 * SERIA: CD
 * GRUPA: 311
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "wrap.h"
#include "alignLeft.h"
#include "alignRight.h"
#include "center.h"
#include "list.h"
#include "paragraph.h"
#include "orderedList.h"
#include "justify.h"
#include "util.h"

void extractArguments(char* argv, char result[5][20], int* argc);
void getCommands(char* argv, char result[10][25], unsigned int* count);
int processCommand(char text[1000][1000], unsigned long* lines,
            	   char argv[5][20], int argc, char command_type);

int main(int argc, char *argv[])
{
	char buffer[MAX_LINE_LENGHT]; // buffer for reading from file
	char text[MAX_LINE_LENGHT][MAX_LINE_LENGHT]; // original text, line by line
	char text_copy[MAX_LINE_LENGHT][MAX_LINE_LENGHT]; // copy of text, 
													  // line by line
	char commands[10][25];

	unsigned long line_count;

	if(argc != 4) // invalid number of arguments
	{
		fprintf(stderr, "Invalid number of arguments\n");
		return ERR_INVALID_OPERATION;
	}

	// input file
	FILE* input = fopen(argv[2], "r");
	
	if(input == NULL)
	{
		fprintf(stderr, "File %s does not exist.\n", argv[2]);
		return ERR_INVALID_OPERATION;
	}

	// reading line by line
	while(fgets(buffer, MAX_LINE_LENGHT, input) != NULL)
	{
		strcpy(text[line_count], buffer);
		strcpy(text_copy[line_count], buffer);
		line_count++;
	}

	fclose(input);

	// keep a copy of the original file
	copyMatrix(text, text_copy, line_count);
	unsigned long line_count_copy = line_count;

	// saving the first 10 commands
	unsigned int command_count = 0;
	getCommands(argv[1], commands, &command_count);

	// empty command string
	if(command_count == 0)
	{
		fprintf(stdout, "Invalid operation!\n");
		writeToFile(argv[3], text, &line_count);
		return ERR_INVALID_OPERATION;
	}

	char arguments[5][20];
	char command_type; // holds the letter speccific to the operation
	int argumnets_count;
	unsigned int i;

	// walk through other commands
	for(i = 0; i < command_count; i++)
	{
		// invalid command, lenght > 20
		if(strlen(commands[i]) > 20)
		{
			fprintf(stdout, "Invalid operation!\n");
			writeToFile(argv[3], text, &line_count);
			return -1;
		}

		argumnets_count = 0;
		char* return_value = strpbrk(commands[i], COMMANDS);

		// did not identify the command
		if(return_value == NULL)
		{
			fprintf(stdout, "Invalid operation!\n");
			writeToFile(argv[3], text_copy, &line_count_copy);
			return ERR_INVALID_OPERATION;
		}

		command_type = *return_value;
		return_value++;

		// extracting arguments for the current command
		extractArguments(return_value, arguments, &argumnets_count);
		
		// processing current command
		int result = processCommand(text, &line_count, arguments, 
									argumnets_count, command_type);

		// in case of an error we end the program
		if(result == ERR_INVALID_OPERATION)
		{
			writeToFile(argv[3], text_copy, &line_count_copy);
			return ERR_INVALID_OPERATION;
		}
	}

	// remove trailing white spaces an end main
	removeTrailingWhiteSpaces(text, 0, line_count - 1);
	return writeToFile(argv[3], text, &line_count);
}

// extracting arguments from argv and returns them in results
void extractArguments(char* argv, char result[5][20], int* argc)
{
	int len = strlen(argv);

	// adding white space at the end of the arguments
	char* aux = (char*)malloc(len+2);
	strcpy(aux, argv);
	aux[len++] = ' ';
	aux[len] = '\0';

	int arglen;
	int i;
	for(i = 0, arglen = 0, *argc = 0; i < len; i++)
	{
		if(aux[i] != ' ' && aux[i] != '\n')
		{
			// appends current character
			result[*argc][arglen] = aux[i];
			arglen++;
		}
		else if(arglen != 0)
		{
			// new argument
			result[*argc][arglen] = '\0';
			(*argc)++;
			arglen = 0;
		}
	}

	free(aux);

	return;
}

// saves the first 10 commands found in argv in result and their
// number in count
void getCommands(char* argv, char result[10][25], unsigned int* count)
{
	// NOTE: strtok for more than 10 commnads breaks the program
	// makes text[0] full if '\0'
	int len = strlen(argv);
	int pos = 0;
	int i;
	for(i = 0; i < len; i++)
	{
		if(argv[i] == ',')
		{
			// error for more than 10 commands
			if(*count >= 10)
			{
				fprintf(stdout, 
					"Too many operations! Only the first 10 will be applied.\n"
					);
				return;
			}
			
			// copy the current command
			strncpy(result[*count], argv + pos, i - pos);
			pos = i + 1;
			(*count)++;
		}
	}

	// get the last command
	strncpy(result[*count], argv + pos, i - pos);
	pos = i + 1;
	(*count)++;
	return;
}

int processCommand(char text[1000][1000], unsigned long* lines,
				   char argv[5][20], int argc, char command_type)
{
	if(command_type == 'w' || command_type == 'W')
	{
		// call wrap
		int result = wrap(text, lines, argv, argc);
		return result;
	}

	if(command_type == 'l' || command_type == 'L')
	{
		// call allign left
		int result = align_left(text, lines, argv,
								argc);
		return result;
	}

	if(command_type == 'r' || command_type == 'R')
	{
		// call allign right
		int result = align_right(text, lines, argv,
									argc);
		return result;
	}

	if(command_type == 'p' || command_type == 'P')
	{
		// call paragraph
		int result = paragraph(text, lines, argv,
								argc);
		return result;
	}

	if(command_type == 'c' || command_type == 'C')
	{
		// call center
		int result = center(text, lines, argv, argc);
		return result;
	}

	if(command_type == 'i' || command_type == 'I')
	{
		// call center
		int result = list(text, lines, argv, argc);
		return result;
	}

	if(command_type == 'o' || command_type == 'O')
	{
		// call center
		int result = ordered_list(text, lines, argv,
									argc);
		return result;
	}

	if(command_type == 'j' || command_type == 'J')
	{
		// call center
		int result = justify(text, lines, argv, argc);
		return result;
	}

	return 0;
}