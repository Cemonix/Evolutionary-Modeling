#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>

void PrintCityMatrix(double** cityMatrix, unsigned int citiesCount);
void* SafeMalloc(size_t size);
void Free2DArray(double** array, unsigned int rows);

#endif
