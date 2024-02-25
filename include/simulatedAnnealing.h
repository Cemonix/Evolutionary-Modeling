#ifndef SIMULATEDANNEALING_H
#define SIMULATEDANNEALING_H

#include "stateController.h"

typedef struct {
    double temperature;
    size_t* currentSolution; // Array of indices representing the current tour
    size_t* bestSolution; // Array of indices representing the best tour found
    double bestCost;
} SAState;

void InitializeSA(SAState* saState, size_t nodesCount);
void RunSimulatedAnnealing(SAState* saState, double** nodeMatrix, size_t nodesCount);
void GenerateNeighborSolution(size_t currentSolution[], size_t nodesCount, size_t* newSolution);
double CalculateSolutionCost(size_t solution[], double** nodeMatrix, size_t nodesCount);
void UpdateTemperature(SAState* saState);

#endif
