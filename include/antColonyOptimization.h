#ifndef ANTCOLONYOPTIMIZATION_H
#define ANTCOLONYOPTIMIZATION_H

#include "ant.h"
#include "config.h"

void DepositPheromones(const Ant* ants, double** pheromoneMatrix, unsigned int citiesCount);
void EvaporatePheromones(double** pheromoneMatrix, unsigned int citiesCount);
void Simulation(
    Ant ants[NUM_ANTS], double** cityMatrix, unsigned int citiesCount, double** pheromoneMatrix
);

#endif
