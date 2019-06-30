#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// macro for memory handling 
#define CheckAllocation(ptr) if(ptr == NULL){fprintf(stderr, "Memory allocation\
 failed at line %d\nProgram will exit...\n", __LINE__ - 1); exit(-1);}
#define freeMemory(ptr) {free(ptr); ptr = NULL;}

#define MARKED 1
#define UNMARKED -1

typedef struct
{
    int x;
    int y;
} Point2D;

typedef struct 
{
    int powerPlant; // which power plant
    int zeros;      // how many tasks can it effectively do
} Task;


void printPointsArray(Point2D* array, int size)
{
    for(int i = 0; i < size; i++)
        printf("X: %d | Y: %d\n", array[i].x, array[i].y);

    return;
}

void printIntMatrix(int** matrix, int lin, int col)
{
    for(int i = 0; i < lin; i++)
    {
        for(int j = 0; j < col; j++)
            printf("%d\t", matrix[i][j]);
        printf("\n");
    }

    printf("\n");

    return;
}

int manhattanDist(Point2D point1, Point2D point2)
{
    return (abs(point1.x - point2.x) + abs(point1.y - point2.y));
}

int** allocateMatrix(int lin, int col)
{
    int** matrix = (int**)malloc(sizeof(int*) * lin);
    CheckAllocation(matrix);
    for(int i = 0; i < lin; i++)
    {
        matrix[i] = (int*)calloc(col, sizeof(int)); // M > N case
        CheckAllocation(matrix[i]);
    }

    return matrix;
}

void populateCostMatrix(int** costMatrix, Point2D* cities, int nCities, 
                        Point2D* powerPlants, int nPowerPlants)
{
    for(int i = 0; i < nPowerPlants; i++)
        for(int j = 0; j < nCities; j++)
            costMatrix[i][j] = manhattanDist(powerPlants[i], cities[j]);

    return;
}

void substractMinimumFromLine(int** costMatrix, int lin, int col)
{
    for(int i = 0; i < lin; i++)
    {
        int minimum = __INT_MAX__;
        for(int j = 0; j < col; j++)
            if(costMatrix[i][j] < minimum)
                minimum = costMatrix[i][j];

        for(int j = 0; j < col; j++)
            costMatrix[i][j] -= minimum;
    }

    return;
}

void substractMinimumFromColumn(int** costMatrix, int lin, int col)
{
    for(int i = 0; i < col; i++)
    {
        int minimum = __INT_MAX__;
        for(int j = 0; j < lin; j++)
            if(costMatrix[j][i] < minimum)
                minimum = costMatrix[j][i];

        for(int j = 0; j < lin; j++)
            costMatrix[j][i] -= minimum;
    }
    
    return;
}

int comparator(const void *p, const void *q)  
{ 
    int l = ((Task*)p)->zeros; 
    int r = ((Task*)q)->zeros;  
    return (l - r); 
} 

int* assignWorkesToTasks(int** costMatrix, int lin, int col)
{
    Task* tasks = (Task*)malloc(sizeof(Task) * lin);
    CheckAllocation(tasks);

    // find how many zeros are on each line
    for(int i = 0; i < lin; i++)
    {
        int zeros = 0;
        for(int j = 0; j < col; j++)
            if(costMatrix[i][j] == 0)
                zeros++;

        tasks[i].powerPlant = i;
        tasks[i].zeros = zeros;
    }

    // sort the tasks a.k.a lines
    // we will try to assign as many as possible
    qsort((void*)tasks, lin, sizeof(Task), comparator);

    int* taskToWorkerId = (int*)malloc(sizeof(int) * lin);
    CheckAllocation(taskToWorkerId);
    for(int i = 0; i < lin; i++)
        taskToWorkerId[i] = -1;

    // try to assign as many tasks as possible (Greedy Style)
    // we assing tasks to the ones that have the least amount of choices
    // i.e. the plants with the least amount of zeros
    for(int i = 0; i < lin; i++)
    {
        int currentPowerPlant = tasks[i].powerPlant;
        for(int j = 0; j < col; j++)
            if(costMatrix[currentPowerPlant][j] == 0 && taskToWorkerId[j] == -1)
            {
                taskToWorkerId[j] = currentPowerPlant;
                break;
            }
    }


    freeMemory(tasks);
    return taskToWorkerId;
}

int minCoverLines(int** costMatrix, int lin, int col)
{
    int* taskToWorkerId = assignWorkesToTasks(costMatrix, lin, col);

    int linesNeded = 0;
    for(int i = 0; i < lin; i++)
    {
        if(taskToWorkerId[i] != -1)
            linesNeded++;
    }

    freeMemory(taskToWorkerId);

    return linesNeded;
}

int countZerosOnLine(int** matrix, int lin, int col)
{
    int count = 0;
    for(int i = 0; i < col; i++)
        if(matrix[lin][i] == 0)
            count++;

    return count;
}


void subOrAddUncoveredValue(int** costMatrix, int lin, int col)
{
    int* taskToWorker = assignWorkesToTasks(costMatrix, lin, col);

    int* markedRows = (int*)malloc(sizeof(int) * lin);
    CheckAllocation(markedRows);
    int* markedCols = (int*)malloc(sizeof(int) * col);
    CheckAllocation(markedCols);

    for(int i = 0; i < lin; i++)
        markedRows[i] = MARKED;

    for(int i = 0; i < col; i++)
        markedCols[i] = UNMARKED;

    // mark rows that have no task
    // i.e. unmark those that have a task
    for(int i = 0; i < lin; i++)
        if(taskToWorker[i] != -1)
            markedRows[taskToWorker[i]] = UNMARKED;

    for(int i = 0; i < lin; i++)
        if(markedRows[i] == MARKED)
            for(int j = 0; j < col; j++)
                if(costMatrix[i][j] == 0)
                {
                    markedCols[j] = MARKED;
                    for(int l = 0; l < lin; l++)
                        if(costMatrix[l][j] == 0 && 
                           countZerosOnLine(costMatrix, l, col) <= 1 &&
                           markedRows[l] == UNMARKED)   
                            markedRows[l] = MARKED;
                }

    int minimum = __INT_MAX__;
    for(int i = 0; i < lin; i++)
        for(int j = 0; j < col; j++)
            if(markedRows[i] == MARKED && markedCols[j] == UNMARKED)
                if(costMatrix[i][j] < minimum)
                    minimum = costMatrix[i][j];

    // shit hit the fan and we stop an infinite cycle
    if(minimum == 0)
    {
        fprintf(stderr, "Something went wrong! Program will abort!\n");
        exit(-1);
    }

    for(int i = 0; i < lin; i++)
        for(int j = 0; j < col; j++)
        {
            if(markedRows[i] == MARKED)
                costMatrix[i][j] -= minimum;

            if(markedCols[j] == MARKED)
                costMatrix[i][j] += minimum;
        }

    freeMemory(taskToWorker)
    return;
}

int main(int argc, char** argv)
{
    int nCities;
    int nPowerPlants;

    if(argc != 3) 
    {
        fprintf(stderr, "Invalid number of arguments!\n");
        return -1;
    }

    // open input file
    FILE* input = fopen(argv[1], "r");
    if(input == NULL)
    {
        fprintf(stderr, "Invalid input file!\n");
        return -1;
    }

    // open output file
    FILE* output = fopen(argv[2], "w");
    if(input == NULL)
    {
        fprintf(stderr, "Invalid output file!\n");
        return -1;
    }

    fscanf(input, "%d %d\n", &nCities, &nPowerPlants);

    // allocating memory for reading the position of cities and power plants
    Point2D* cities = (Point2D*)malloc(sizeof(Point2D) * nCities);
    CheckAllocation(cities);

    Point2D* powerPlants = (Point2D*)malloc(sizeof(Point2D) * nPowerPlants);
    CheckAllocation(powerPlants);

    // reading data
    for(int i = 0; i < nCities; i++)
        fscanf(input, "%d %d\n", &cities[i].x, &cities[i].y);

    for(int i = 0; i < nPowerPlants; i++)
        fscanf(input, "%d %d\n", &powerPlants[i].x, &powerPlants[i].y);

    // allocate the cost matrix
    int** costMatrix = allocateMatrix(nPowerPlants, nPowerPlants);
    // keep a copy of the original cost matrix
    int** cCostMatrix = allocateMatrix(nPowerPlants, nPowerPlants);

    // populate cost matrix & it's copy
    populateCostMatrix(costMatrix, cities, nCities, powerPlants, nPowerPlants);
    populateCostMatrix(cCostMatrix, cities, nCities, powerPlants, nPowerPlants);

    // free coordinate vectors
    freeMemory(cities);
    freeMemory(powerPlants);

    // step 2 & 3
    substractMinimumFromLine(costMatrix, nPowerPlants, nPowerPlants);
    substractMinimumFromColumn(costMatrix, nPowerPlants, nPowerPlants);
    printIntMatrix(costMatrix, nPowerPlants, nPowerPlants);
    // step 4 & 5
    while(minCoverLines(costMatrix, nPowerPlants, nPowerPlants) != nPowerPlants)
        subOrAddUncoveredValue(costMatrix, nPowerPlants, nPowerPlants);
    
    int* tasksToWorkes = assignWorkesToTasks(costMatrix, nPowerPlants, 
                                             nPowerPlants);

    // write the configuration on disk
    for(int city = 0; city < nCities; city++)
        fprintf(output, "%d\n", tasksToWorkes[city]);

    // free memory
    freeMemory(tasksToWorkes);
    for(int i = 0; i < nPowerPlants; i++)
    {
        freeMemory(cCostMatrix[i]);
        freeMemory(costMatrix[i]);
    }
    freeMemory(costMatrix);
    freeMemory(cCostMatrix);

    // close file handles
    fclose(input);
    fclose(output);

    return 0;
}