#ifndef ANT_H
#define ANT_H

#include <stddef.h>

typedef struct {
    int previousNode;
    int currentNode;
    float progress;
    int* visited;
    int* path;
    unsigned int pathIndex;
    size_t tourLength;
} Ant;

void InitializeAnts(Ant* ants, unsigned int nodeCount);
void FreeAnts(const Ant* ants);
void AntMove(
    Ant* ant, double** nodeMatrix, unsigned int nodeCount, double** pheromoneMatrix
);
void UpdateAnt(Ant* ant, double deltaTime);
void AntFindPath(
    Ant* ant, double** nodeMatrix, unsigned int nodeCount, double** pheromoneMatrix
);
double Attractiveness(double** nodeMatrix, int idx_i, int idx_j);
void CalculateTransitionProbabilities(
     int currentNode, int visited[], double probabilities[],
     double** nodeMatrix, unsigned int nodeCount, double** pheromoneMatrix
);
int ChooseNextNode(
    int currentNode, int visited[], double probabilities[], double** nodeMatrix, unsigned int nodeCount
);
int AllVisited(const int* visited, unsigned int nodeCount);

#endif
