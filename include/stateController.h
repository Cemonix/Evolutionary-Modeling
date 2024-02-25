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
} City;

extern SimulationState simState;
extern Ant ants[];
extern City cities[];
extern double** cityMatrix;
extern double** pheromoneMatrix;
extern unsigned int citiesCount;
extern unsigned int iteration;
extern size_t bestTour;

void StartSimulation();
void PauseSimulation();
void ResetSimulation();
void InitializeSimulation(Ant* ants, double** pheromoneMatrix,  unsigned int citiesCount);
void UpdateSimulation(float deltaTime);
void AnimateAnts(Ant* ants, float deltaTime, bool* animating);
void FillCityMatrix(double** cityMatrix, const City* cities, unsigned int citiesCount);
void CreateCity(Vector2 mousePosition);

#endif
