#include "ant.h"

#include "antColonyOptimization.h"
#include "config.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Ant ants[NUM_ANTS];
double** pheromoneMatrix = nullptr;

void InitializeAnts(Ant* ants, const unsigned int nodeCount)
{
    const size_t visitedAllocSize = nodeCount * sizeof(bool);
    const size_t pathAllocSize = nodeCount * sizeof(size_t);
    for (int i = 0; i < NUM_ANTS; ++i)
    {
        ants[i].visited = (bool*) SafeMalloc(visitedAllocSize);
        memset(ants[i].visited, false, visitedAllocSize);

        ants[i].path = (size_t*) SafeMalloc(pathAllocSize);
        memset(ants[i].path, -1, pathAllocSize);

        ants[i].previousNode = -1;
        ants[i].currentNode = rand() % nodeCount;
        ants[i].visited[ants[i].currentNode] = true; // Starting node is set as visited
        ants[i].tourLength = 0;
        ants[i].path[0] = ants[i].currentNode; // Start the path with the current node
        ants[i].pathIndex = 1; // Next node will be placed at index 1
    }
}

void FreeAnts(const Ant* ants)
{
    for (int i = 0; i < NUM_ANTS; ++i)
    {
        free(ants[i].visited);
        free(ants[i].path);
    }
}

void AntMove(
    Ant* ant, double** nodeMatrix, const unsigned int nodeCount, double** pheromoneMatrix
)
{
    double probabilities[nodeCount];

    CalculateTransitionProbabilities(
        ant->currentNode, ant->visited, probabilities, nodeMatrix, nodeCount, pheromoneMatrix
    );
    const int nextNode = ChooseNextNode(
        ant->currentNode, ant->visited, probabilities, nodeMatrix, nodeCount
    );

    if (nextNode != -1) {
        ant->tourLength += nodeMatrix[ant->currentNode][nextNode];
        ant->visited[nextNode] = true;
        ant->path[ant->pathIndex++] = nextNode;
        ant->previousNode = ant->currentNode;
        ant->currentNode = nextNode;
        ant->progress = 0.0;
    }
    else {
        fprintf(stderr, "Error: Next node was not chosen.\n");
        exit(EXIT_FAILURE);
    }
}

void AnimateAnts(Ant* ants, const float deltaTime, bool* animating)
{
    for (int i = 0; i < NUM_ANTS; ++i) {
        UpdateAnt(&ants[i], deltaTime);
        if (ants[i].previousNode == ants[i].currentNode) {
            *animating = false;
        }
    }
}

void UpdateAnt(Ant* ant, const double deltaTime)
{
    ant->progress += ANIMATION_SPEED * deltaTime;

    if (ant->progress >= 1.0) {
        ant->progress = 0.0;
        ant->previousNode = ant->currentNode;
    }
}
