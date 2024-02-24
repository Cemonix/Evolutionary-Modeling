#ifndef ANTCOLONYOPTIMIZATION_H
#define ANTCOLONYOPTIMIZATION_H

#include "ant.h"
#include "config.h"

void DepositPheromones(const Ant* ants, double** pheromoneMatrix, int citiesCount);
void EvaporatePheromones(double** pheromoneMatrix, int citiesCount);
void InitializeSimulation(Ant* ants, double** pheromoneMatrix, int citiesCount);
void Simulation(
    Ant ants[NUM_ANTS], double** cityMatrix, int citiesCount, double** pheromoneMatrix
);

#endif
