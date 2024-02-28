#include "antColonyOptimization.h"

#include "ant.h"
#include "stateController.h"
#include "utils.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

void InitializeACO(Ant* ants, double** pheromoneMatrix, const unsigned int nodeCount)
{
    for (int i = 0; i < nodeCount; ++i)
    {
        pheromoneMatrix[i] = (double*) SafeMalloc(nodeCount * sizeof(double));
        for (int j = 0; j < nodeCount; ++j)
        {
            pheromoneMatrix[i][j] = INITIAL_PHEROMONE;
        }
    }

    InitializeAnts(ants, nodeCount);
}

double Attractiveness(double** nodeMatrix, const int idx_i, const int idx_j)
{
    if (nodeMatrix[idx_i][idx_j] == 0) return 0;
    return 1.0 / nodeMatrix[idx_i][idx_j];
}

void CalculateTransitionProbabilities(
    const int currentNode, bool visited[], double probabilities[],
    double** nodeMatrix, const unsigned int nodeCount, double** pheromoneMatrix
)
{
    double probabilityDenominator = 0.0;

    for (int i = 0; i < nodeCount; ++i) {
        if (!visited[i] && nodeMatrix[currentNode][i] != 0) {
            probabilityDenominator += pow(pheromoneMatrix[currentNode][i], ALPHA) *
                pow(Attractiveness(nodeMatrix, currentNode, i), BETA);
        }
    }

    for (int i = 0; i < nodeCount; ++i) {
        if (!visited[i] && nodeMatrix[currentNode][i] != 0) {
            probabilities[i] = (
                pow(pheromoneMatrix[currentNode][i], ALPHA) *
                pow(Attractiveness(nodeMatrix, currentNode, i), BETA)
            ) / probabilityDenominator;
        }
        else
            probabilities[i] = 0; // Node j has already been visited, so probability is 0
    }
}

int ChooseNextNode(
    const int currentNode, bool visited[], double probabilities[],
    double** nodeMatrix, const unsigned int nodeCount
)
{
    const double randomValue = (double)rand() / RAND_MAX;
    double cumulativeProbability = 0.0;

    // Roulette wheel selection
    for (int i = 0; i < nodeCount; ++i) {
        if (!visited[i] && nodeMatrix[currentNode][i] != 0) {
            cumulativeProbability += probabilities[i];
            if (randomValue <= cumulativeProbability)
                return i;
        }
    }

    // No Node was chosen
    return -1;
}

int AllVisited(bool visited[], const unsigned int nodeCount)
{
    int numVisited = 0;
    for (int i = 0; i < nodeCount; ++i) {
        if (visited[i])
            numVisited++;
    }
    return nodeCount == numVisited;
}

void DepositPheromones(const Ant* ants, double** pheromoneMatrix, const unsigned int nodeCount)
{
    for (int i = 0; i < NUM_ANTS; ++i) {
        const double deltaTau = Q / ants[i].tourLength;
        for (int j = 0; j < nodeCount - 1; ++j) {
            const int node1 = ants[i].path[j];
            const int node2 = ants[i].path[j+1];
            pheromoneMatrix[node1][node2] += deltaTau;
            if (SYMETRIC)
                pheromoneMatrix[node2][node1] += deltaTau;
        }

        const int lastNode = ants[i].path[nodeCount - 1];
        const int firstNode = ants[i].path[0];
        pheromoneMatrix[lastNode][firstNode] += deltaTau;
        pheromoneMatrix[firstNode][lastNode] += deltaTau;
    }
}

void EvaporatePheromones(double** pheromoneMatrix, const unsigned int nodeCount)
{
    for (int i = 0; i < nodeCount; i++) {
        for (int j = 0; j < nodeCount; j++) {
            pheromoneMatrix[i][j] *= (1 - RHO);
        }
    }
}

void Simulation(
    Ant ants[NUM_ANTS], double** nodeMatrix, const unsigned int nodeCount, double** pheromoneMatrix
)
{
    InitializeACO(ants, pheromoneMatrix, nodeCount);

    size_t bestTour = INT_MAX;
    for (int i = 0; i < ITERATIONS; ++i) {
        if (VERBOSE)
            printf("Iteration %d\n", i);

        for (int j = 0; j < NUM_ANTS; ++j) {
            while (!AllVisited(ants[j].visited, nodeCount))
                AntMove(&ants[j], nodeMatrix, nodeCount, pheromoneMatrix);

            if (ants[j].tourLength < bestTour)
                bestTour = ants[j].tourLength;

            if (VERBOSE)
                printf("Ant: %d tour lenght %llu\n", j, ants[j].tourLength);
        }

        DepositPheromones(ants, pheromoneMatrix, nodeCount);
        EvaporatePheromones(pheromoneMatrix, nodeCount);
        InitializeAnts(ants, nodeCount);
    }

    printf("Total best tour lenght: %llu\n", bestTour);
}