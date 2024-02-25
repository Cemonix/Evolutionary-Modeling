#ifndef STATECONTROLLER_H
#define STATECONTROLLER_H

#include "ant.h"
#include "config.h"

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

void StartSimulation();
void PauseSimulation();
void ResetSimulation();
void InitializeSimulation(Ant* ants, double** pheromoneMatrix,  unsigned int nodeCount);
void UpdateSimulation(float deltaTime);
void AnimateAnts(Ant* ants, float deltaTime, bool* animating);
void FillNodeMatrix(double** nodeMatrix, const Node* nodes, unsigned int nodeCount);
void CreateNode(Vector2 mousePosition);

#endif
