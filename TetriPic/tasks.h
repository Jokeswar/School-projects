#include "utils.h"

#ifndef TASKS_H
#define TASKS_H

// define tasks
void task1(void);
void task2(void);
void task3(void);
void task4(void);
void bonus(void);

// define simulation
int simulate(char **board, size board_sz, char piece[6][6],
             size piece_sz, int col);

#endif