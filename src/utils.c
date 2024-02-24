#include "utils.h"

#include <stdio.h>
#include <stdlib.h>

void PrintCityMatrix(double** cityMatrix, const int citiesCount)
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

    if (ptr == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    return ptr;
}
