#include "simulatedAnnealing.h"

#include "config.h"
#include "utils.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void InitializeSA(SAState* saState, const size_t nodesCount)
{
    const size_t solutionSize = nodesCount * sizeof(size_t);
    saState->currentSolution = SafeMalloc(solutionSize);
    saState->bestSolution = SafeMalloc(solutionSize);

    for (size_t i = 0; i < nodesCount; ++i) {
        saState->currentSolution[i] = i;
    }

    // Shuffle the currentSolution to randomize the starting tour
    for (size_t i = 0; i < nodesCount - 1; ++i) {
        const size_t j = i + rand() / (RAND_MAX / (nodesCount - i) + 1);
        const size_t temp = saState->currentSolution[i];
        saState->currentSolution[i] = saState->currentSolution[j];
        saState->currentSolution[j] = temp;
    }

    saState->bestCost = CalculateSolutionCost(saState->currentSolution, nodeMatrix, nodesCount);
    memcpy(saState->bestSolution, saState->currentSolution, solutionSize);
    saState->temperature = INITIAL_TEMPERATURE;
}

void RunSimulatedAnnealing(SAState* saState, double** nodeMatrix, const size_t nodesCount)
{
    saState->bestCost = CalculateSolutionCost(saState->currentSolution, nodeMatrix, nodesCount);

    while (saState->temperature > FINAL_TEMPERATURE) {
        for (int step = 0; step < STEPS_PER_TEMPERATURE; ++step) {
            size_t newSolution[nodesCount];
            GenerateNeighborSolution(saState->currentSolution, nodesCount, newSolution);
            const double newCost = CalculateSolutionCost(newSolution, nodeMatrix, nodesCount);
            const double deltaEnergy = newCost - saState->bestCost;

            if (deltaEnergy < 0 || exp(-deltaEnergy / saState->temperature) > rand() / (RAND_MAX + 1.0)) {
                memcpy(saState->currentSolution, newSolution, nodesCount * sizeof(size_t));

                if (newCost < saState->bestCost) {
                    memcpy(saState->bestSolution, newSolution, nodesCount * sizeof(size_t));
                    saState->bestCost = newCost;
                }
            }
        }
        UpdateTemperature(saState);
    }
}

void GenerateNeighborSolution(size_t currentSolution[], const size_t nodesCount, size_t* newSolution)
{
    for (size_t i = 0; i < nodesCount; ++i) {
        newSolution[i] = currentSolution[i];
    }

    // Randomly select two indices for the 2-opt swap
    size_t i = rand() % (nodesCount - 1); // Ensure i < nodesCount - 1
    size_t j = i + 1 + (rand() % (nodesCount - i - 1)); // Ensure i < j

    // Perform the 2-opt swap (reverse the segment between i and j)
    while (i < j) {
        const size_t temp = newSolution[i];
        newSolution[i] = newSolution[j];
        newSolution[j] = temp;
        i++;
        j--;
    }
}

double CalculateSolutionCost(size_t solution[], double** nodeMatrix, const size_t nodesCount)
{
    if (solution == NULL || nodeMatrix == NULL) {
        fprintf(stderr, "Error: Null pointer passed to CalculateSolutionCost.\n");
        exit(EXIT_FAILURE);
    }

    double totalCost = 0.0;
    for (size_t i = 0; i < nodesCount - 1; ++i) {
        totalCost += nodeMatrix[solution[i]][solution[i + 1]];
    }

    // Add the distance from the last node back to the first to complete the cycle
    totalCost += nodeMatrix[solution[nodesCount - 1]][solution[0]];
    return totalCost;
}

void UpdateTemperature(SAState* saState)
{
    saState->temperature *= COOLING_RATE;
}

void FreeSAState(SAState* saState)
{
    if (saState->currentSolution != NULL) {
        free(saState->currentSolution);
        saState->currentSolution = NULL;
    }

    if (saState->bestSolution != NULL) {
        free(saState->bestSolution);
        saState->bestSolution = NULL;
    }
}