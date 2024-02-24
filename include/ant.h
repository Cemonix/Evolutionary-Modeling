#ifndef ANT_H
#define ANT_H

#include <stddef.h>

typedef struct {
    int previousCity;
    int currentCity;
    float progress;
    int* visited;
    int* path;
    unsigned int pathIndex;
    size_t tourLength;
} Ant;

void InitializeAnts(Ant* ants, int citiesCount);
void FreeAnts(const Ant* ants);
void AntMove(
    Ant* ant, double** cityMatrix, int citiesCount, double** pheromoneMatrix
);
void AntFindPath(
    Ant* ant, double** cityMatrix, int citiesCount, double** pheromoneMatrix
);
double Attractiveness(double** cityMatrix, int idx_i, int idx_j);
void CalculateTransitionProbabilities(
     int currentCity, int visited[], double probabilities[],
     double** cityMatrix, int citiesCount, double** pheromoneMatrix
);
int ChooseNextCity(int currentCity, int visited[], double probabilities[], double** cityMatrix, int citiesCount);
int AllVisited(const int* visited, int citiesCount);

#endif
