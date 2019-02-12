#include "util.h"

#ifndef WRAP_H
#define WRAP_H
	
int wrap(char text[MAX_LINE_NUMBER][MAX_LINE_LENGHT], unsigned long* lines,
		 char argv[5][20], int argc);

#endif