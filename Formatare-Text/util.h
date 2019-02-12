#ifndef UTIL_H
#define UTIL_H

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

#define MAX_LINE_LENGHT 1000
#define MAX_LINE_NUMBER 1000
#define COMMANDS "WwCcLlRrJjPpOoIi"
#define LONG_MIN (-2147483647L - 1)
#define LONG_MAX 2147483647
#define ERR_INVALID_OPERATION -1

int convertToInt(char* string);
int countWords(char sentence[MAX_LINE_LENGHT]);
int writeToFile(char* filename, char text[MAX_LINE_NUMBER][MAX_LINE_LENGHT],
				unsigned long* lines);

void removeTrailingWhiteSpaces(char text[MAX_LINE_NUMBER][MAX_LINE_LENGHT],
							   unsigned int start,  unsigned int end);
void removeBeginingWhiteSpaces(char text[MAX_LINE_NUMBER][MAX_LINE_LENGHT],
						       unsigned int start, unsigned int end);
void copyMatrix(char destination[MAX_LINE_NUMBER][MAX_LINE_LENGHT],
				char source[MAX_LINE_NUMBER][MAX_LINE_LENGHT], int lines);
void addWhiteSpacesAtPosition(char sentence[MAX_LINE_LENGHT], int start,
							  int count);
void sortLines(char text[MAX_LINE_NUMBER][MAX_LINE_LENGHT], int start, int end,
			   int sort_type);

#endif