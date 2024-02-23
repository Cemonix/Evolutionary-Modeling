#include "utils.h"

#include "config.h"

#include <stdio.h>

void printCityMatrix(double cityMatrix[NUM_CITIES][NUM_CITIES])
{
    printf("City Distance Matrix:\n");
    for (int i = 0; i < NUM_CITIES; ++i)
    {
        for (int j = 0; j < NUM_CITIES; ++j)
        {
            printf("%7.1f", cityMatrix[i][j]);
        }
        printf("\n");
    }
}
