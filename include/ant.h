#ifndef ANT_H
#define ANT_H

#include <stddef.h>
#include "config.h"

typedef struct {
    int previousCity;
    int currentCity;
    float progress;
    int visited[NUM_CITIES];
    int path[NUM_CITIES];
    unsigned int pathIndex;
    size_t tourLength;
} Ant;

void initializeAnts(Ant* ants);
void antMove(
    Ant* ant, double** cityMatrix, int citiesCount, double pheromoneMatrix[NUM_CITIES][NUM_CITIES]
);
void antFindPath(
    Ant* ant, double** cityMatrix, int citiesCount, double pheromoneMatrix[NUM_CITIES][NUM_CITIES]
);
double attractiveness(double** cityMatrix, int i, int j);
void calculateTransitionProbabilities(
     int currentCity, int visited[], double probabilities[],
     double** cityMatrix, int citiesCount, double pheromoneMatrix[NUM_CITIES][NUM_CITIES]
);
int chooseNextCity(int currentCity, int visited[], double probabilities[], double** cityMatrix, int citiesCount);
int allVisited(const int* visited, int citiesCount);

#endif
