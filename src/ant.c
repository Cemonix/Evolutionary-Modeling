#include "ant.h"
#include "config.h"
#include "utils.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void InitializeAnts(Ant* ants, const unsigned int nodeCount)
{
    for (int i = 0; i < NUM_ANTS; ++i)
    {
        ants[i].visited = (int*) SafeMalloc(nodeCount * sizeof(int));
        memset(ants[i].visited, 0, nodeCount * sizeof(int));

        ants[i].path = (int*) SafeMalloc(nodeCount * sizeof(int));
        memset(ants[i].path, -1, nodeCount * sizeof(int));

        ants[i].previousNode = -1;
        ants[i].currentNode = rand() % nodeCount;
        ants[i].visited[ants[i].currentNode] = 1; // Starting node is set as visited
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
        ant->visited[nextNode] = 1;
        ant->path[ant->pathIndex++] = nextNode;
        ant->previousNode = ant->currentNode;
        ant->currentNode = nextNode;
        ant->progress = 0.0;
    }
    else {
        printf("ERROR: Next node was not chosen.");
        exit(1);
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

double Attractiveness(double** nodeMatrix, const int idx_i, const int idx_j)
{
    if (nodeMatrix[idx_i][idx_j] == 0) return 0;
    return 1.0 / nodeMatrix[idx_i][idx_j];
}

void CalculateTransitionProbabilities(
    const int currentNode, int visited[], double probabilities[],
    double** nodeMatrix, const unsigned int nodeCount, double** pheromoneMatrix
)
{
    double probabilityDenominator = 0.0;

    for (int i = 0; i < nodeCount; ++i) {
        if (!visited[i] && nodeMatrix[currentNode][i] != 0) {
            probabilityDenominator += pow(pheromoneMatrix[currentNode][i], ALPHA) *
                pow(Attractiveness(nodeMatrix, currentNode, i), BETA);
        }
    }

    for (int i = 0; i < nodeCount; ++i) {
        if (!visited[i] && nodeMatrix[currentNode][i] != 0) {
            probabilities[i] = (
                pow(pheromoneMatrix[currentNode][i], ALPHA) *
                pow(Attractiveness(nodeMatrix, currentNode, i), BETA)
            ) / probabilityDenominator;
        }
        else
            probabilities[i] = 0; // Node j has already been visited, so probability is 0
    }
}

int ChooseNextNode(
    const int currentNode, int visited[], double probabilities[],
    double** nodeMatrix, const unsigned int nodeCount
)
{
    const double randomValue = (double)rand() / RAND_MAX;
    double cumulativeProbability = 0.0;

    // Roulette wheel selection
    for (int i = 0; i < nodeCount; ++i) {
        if (!visited[i] && nodeMatrix[currentNode][i] != 0) {
            cumulativeProbability += probabilities[i];
            if (randomValue <= cumulativeProbability)
                return i;
        }
    }

    // No Node was chosen
    return -1;
}

int AllVisited(const int* visited, const unsigned int nodeCount)
{
    int numVisited = 0;
    for (int i = 0; i < nodeCount; ++i) {
        if (visited[i])
            numVisited++;
    }
    return nodeCount == numVisited;
}