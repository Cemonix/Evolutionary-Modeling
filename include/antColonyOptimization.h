#ifndef ANTCOLONYOPTIMIZATION_H
#define ANTCOLONYOPTIMIZATION_H

#include "ant.h"
#include "config.h"

void DepositPheromones(const Ant* ants, double** pheromoneMatrix, unsigned int nodeCount);
void EvaporatePheromones(double** pheromoneMatrix, unsigned int nodeCount);
void Simulation(
    Ant ants[NUM_ANTS], double** nodeMatrix, unsigned int nodeCount, double** pheromoneMatrix
);

#endif
