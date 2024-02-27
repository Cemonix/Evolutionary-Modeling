#include "utils.h"

#include <stdio.h>
#include <stdlib.h>

void PrintCityMatrix(double** cityMatrix, const unsigned int citiesCount)
{
    printf("City Distance Matrix:\n");
    for (int i = 0; i < citiesCount; ++i)
    {
        for (int j = 0; j < citiesCount; ++j)
        {
            printf("%7.1f", cityMatrix[i][j]);
        }
        printf("\n");
    }
}

void* SafeMalloc(const size_t size)
{
    void *ptr = malloc(size);

    if (ptr == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    return ptr;
}

void Free2DArray(double** array, const unsigned int rows)
{
    for (int i = 0; i < rows; ++i) {
        free(array[i]);
    }
    free(array);
}

double CalculateSolutionCost(size_t solution[], double** nodeMatrix, const size_t nodeCount)
{
    if (solution == NULL || nodeMatrix == NULL) {
        fprintf(stderr, "Error: Null pointer passed to CalculateSolutionCost.\n");
        exit(EXIT_FAILURE);
    }

    double totalCost = 0.0;
    for (size_t i = 0; i < nodeCount - 1; ++i) {
        totalCost += nodeMatrix[solution[i]][solution[i + 1]];
    }

    // Add the distance from the last node back to the first to complete the cycle
    totalCost += nodeMatrix[solution[nodeCount - 1]][solution[0]];
    return totalCost;
}
