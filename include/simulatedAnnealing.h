#ifndef SIMULATEDANNEALING_H
#define SIMULATEDANNEALING_H

#include "stateController.h"

typedef struct {
    double temperature;
    size_t* currentSolution; // Array of indices representing the current tour
    size_t* bestSolution; // Array of indices representing the best tour found
    size_t bestCost;
} SAState;

extern SAState saState;

void InitializeSA(SAState* saState, size_t nodeCount);
void RunSimulatedAnnealing(SAState* saState, double** nodeMatrix, size_t nodeCount);
void GenerateNeighborSolution(size_t currentSolution[], size_t nodeCount, size_t* newSolution);
void UpdateTemperature(SAState* saState);

#endif
