/*
 * NUME: BARUTA DANIEL MIHAIL
 * SERIA: CD
 * GRUPA: 311
 */

#include "tasks.h"

int main(int argc, char **argv)
{
    // guard against no input so it doesn't crash
    if(argc == 2)
        switch(argv[1][0])
        {
            case '1':
                task1();
                break;

            case '2':
                task2();
                break;

            case '3':
                task3();
                break;

            case '4':
                task4();
                break;

            case '5':
                bonus();
                break;
        }

    return 0;
}