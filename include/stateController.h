#ifndef STATECONTROLLER_H
#define STATECONTROLLER_H

#include "ant.h"
#include "enums.h"

#include <limits.h>

#include "raylib.h"

typedef enum {
    SIMULATION_STOPPED,
    SIMULATION_RUNNING,
    SIMULATION_PAUSED
} SimulationState;

typedef struct {
    Vector2 position;
} Node;

extern SimulationState simState;
extern Ant ants[];
extern Node nodes[];
extern double** nodeMatrix;
extern double** pheromoneMatrix;
extern unsigned int nodeCount;
extern unsigned int iteration;
extern size_t bestTour;

void StartSimulation(SimulationType simType);
void PauseSimulation();
void ResetSimulation(SimulationType simType);
void UpdateSimulation(SimulationType simType, float deltaTime);
void FillNodeMatrix(double** nodeMatrix, const Node* nodes, unsigned int nodeCount);
void CreateNode(Vector2 position);
void GenerateNodes(const Node* nodes, unsigned int generateNodeCount, int canvasWidth, int canvasHeight, int offset);

#endif
