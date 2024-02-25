#include "stateController.h"

#include "ant.h"
#include "antColonyOptimization.h"
#include "simulatedAnnealing.h"
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

void StartSimulation(const SimulationType simType)
{
    if (simState != SIMULATION_RUNNING) {
        simState = SIMULATION_RUNNING;

        nodeMatrix = (double**) SafeMalloc(nodeCount * sizeof(double*));
        FillNodeMatrix(nodeMatrix, nodes, nodeCount);

        switch (simType) {
            case ACO_SIMULATION: {
                pheromoneMatrix = (double**) SafeMalloc(nodeCount * sizeof(double*));
                InitializeACO(ants, pheromoneMatrix, nodeCount);
                break;
            }
            case SA_SIMULATION: {
                InitializeSA(&saState, nodeCount);
                break;
            }
            default:
                break;
        }
    }
}

void PauseSimulation()
{
    if (simState == SIMULATION_RUNNING) {
        simState = SIMULATION_PAUSED;
    }
}

void ResetSimulation(const SimulationType simType)
{
    if (simState != SIMULATION_STOPPED) {
        simState = SIMULATION_STOPPED;

        Free2DArray(nodeMatrix, nodeCount);
        nodeCount = 0;
        iteration = 0;
        bestTour = INT_MAX;

        // Reallocate nodeMatrix
        nodeMatrix = (double**) SafeMalloc(nodeCount * sizeof(double*));
        FillNodeMatrix(nodeMatrix, nodes, nodeCount);

        switch (simType) {
            case ACO_SIMULATION: {
                Free2DArray(pheromoneMatrix, nodeCount);
                break;
            }
            case SA_SIMULATION: {
                break;
            }
            default:
                break;
        }
    }
}

void UpdateSimulation(const SimulationType simType, const float deltaTime)
{
    if (simState == SIMULATION_RUNNING) {
        switch (simType) {
            case ACO_SIMULATION: {
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

                if (animating) {
                    AnimateAnts(ants, deltaTime, &animating);
                }
                break;
            }
            case SA_SIMULATION: {
                if (saState.temperature > FINAL_TEMPERATURE) {
                    RunSimulatedAnnealing(&saState, nodeMatrix, nodeCount);
                    bestTour = saState.bestCost;
                    iteration++;
                }
                break;
            }
            default:
                break;
        }
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