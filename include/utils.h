#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>

void PrintCityMatrix(double** cityMatrix, unsigned int citiesCount);
void* SafeMalloc(size_t size);
void Free2DArray(double** array, unsigned int rows);
double CalculateSolutionCost(size_t solution[], double** nodeMatrix, size_t nodeCount);

#endif
