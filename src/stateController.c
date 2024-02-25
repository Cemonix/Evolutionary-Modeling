#include "stateController.h"

#include "ant.h"
#include "antColonyOptimization.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>

#include "raymath.h"

SimulationState simState = SIMULATION_STOPPED;
Node nodes[MAX_NODES];
double** nodeMatrix = nullptr;
unsigned int nodeCount = 0;
unsigned int iteration = 0;
size_t bestTour = INT_MAX;
bool animating = false;

void StartSimulation()
{
    if (simState != SIMULATION_RUNNING) {
        nodeMatrix = (double**) SafeMalloc(nodeCount * sizeof(double*));
        pheromoneMatrix = (double**) SafeMalloc(nodeCount * sizeof(double*));
        FillNodeMatrix(nodeMatrix, nodes, nodeCount);
        InitializeACO(ants, pheromoneMatrix, nodeCount);
        simState = SIMULATION_RUNNING;
    }
}

void PauseSimulation()
{
    if (simState == SIMULATION_RUNNING) {
        simState = SIMULATION_PAUSED;
    }
}

void ResetSimulation()
{
    if (simState != SIMULATION_STOPPED) {
        Free2DArray(nodeMatrix, nodeCount);
        Free2DArray(pheromoneMatrix, nodeCount);
        nodeCount = 0;
        iteration = 0;
        bestTour = INT_MAX;
        simState = SIMULATION_STOPPED;
    }
}

void UpdateSimulation(const float deltaTime)
{
    if (simState == SIMULATION_RUNNING) {
        int numAntPathFinished = 0;
        if (!animating) {
            for (int i = 0; i < NUM_ANTS; ++i) {
                if (!AllVisited(ants[i].visited, nodeCount)) {
                    AntMove(&ants[i], nodeMatrix, nodeCount, pheromoneMatrix);
                }
                else {
                    numAntPathFinished++;
                }
            }
        }

        if (numAntPathFinished == NUM_ANTS) {
            for (int i = 0; i < NUM_ANTS; ++i) {
                if (ants[i].tourLength < bestTour) {
                    bestTour = ants[i].tourLength;
                }
            }
            DepositPheromones(ants, pheromoneMatrix, nodeCount);
            EvaporatePheromones(pheromoneMatrix, nodeCount);
            InitializeAnts(ants, nodeCount);
            iteration++;
            animating = false;
        }
        else
            animating = true;
    }

    if (animating) {
        AnimateAnts(ants, deltaTime, &animating);
    }
}

void FillNodeMatrix(double** nodeMatrix, const Node* nodes, const unsigned int nodeCount)
{
    if (nodeMatrix == NULL) {
        fprintf(stderr, "Error: Null pointer passed to FillNodeMatrix.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < nodeCount; i++) {
        nodeMatrix[i] = (double*) SafeMalloc(nodeCount * sizeof(double));
        for (int j = 0; j < nodeCount; j++) {
            if (i == j)
                nodeMatrix[i][j] = 0.0;
            else
                nodeMatrix[i][j] = Vector2Distance(nodes[i].position, nodes[j].position);
        }
    }
}

void CreateNode(const Vector2 mousePosition)
{
    nodes[nodeCount].position = mousePosition;
    nodeCount++;
}