#ifndef ANTCOLONYOPTIMIZATION_H
#define ANTCOLONYOPTIMIZATION_H

#include "ant.h"
#include "config.h"

void InitializeACO(Ant* ants, double** pheromoneMatrix,  unsigned int nodeCount);
double Attractiveness(double** nodeMatrix, int idx_i, int idx_j);
void CalculateTransitionProbabilities(
     int currentNode, bool visited[], double probabilities[],
     double** nodeMatrix, unsigned int nodeCount, double** pheromoneMatrix
);
int ChooseNextNode(
    int currentNode, bool visited[], double probabilities[], double** nodeMatrix, unsigned int nodeCount
);
int AllVisited(bool visited[], unsigned int nodeCount);
void DepositPheromones(const Ant* ants, double** pheromoneMatrix, unsigned int nodeCount);
void EvaporatePheromones(double** pheromoneMatrix, unsigned int nodeCount);
void Simulation(
    Ant ants[NUM_ANTS], double** nodeMatrix, unsigned int nodeCount, double** pheromoneMatrix
);

#endif
