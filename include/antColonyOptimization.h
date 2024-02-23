#ifndef ANTCOLONYOPTIMIZATION_H
#define ANTCOLONYOPTIMIZATION_H

#include <ant.h>

void depositPheromones(const Ant* ants, double pheromoneMatrix[NUM_CITIES][NUM_CITIES]);
void evaporatePheromones(double pheromoneMatrix[NUM_CITIES][NUM_CITIES]);
void initializeSimulation(Ant* ants, double pheromoneMatrix[NUM_CITIES][NUM_CITIES]);
void simulation(
    Ant ants[NUM_ANTS], double** cityMatrix, double pheromoneMatrix[NUM_CITIES][NUM_CITIES]
);

#endif
