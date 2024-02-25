#ifndef ANT_H
#define ANT_H

#include <stddef.h>

typedef struct {
    size_t previousNode;
    size_t currentNode;
    float progress;
    bool* visited;
    size_t* path;
    unsigned int pathIndex;
    size_t tourLength;
} Ant;

extern Ant ants[];
extern double** pheromoneMatrix;

void InitializeAnts(Ant* ants, unsigned int nodeCount);
void FreeAnts(const Ant* ants);
void AntMove(
    Ant* ant, double** nodeMatrix, unsigned int nodeCount, double** pheromoneMatrix
);
void AnimateAnts(Ant* ants, float deltaTime, bool* animating);
void UpdateAnt(Ant* ant, double deltaTime);

#endif
