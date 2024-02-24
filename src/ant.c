#include "ant.h"
#include "config.h"
#include "utils.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void InitializeAnts(Ant* ants, const int citiesCount)
{
    for (int i = 0; i < NUM_ANTS; ++i)
    {
        ants[i].visited = (int*) SafeMalloc(citiesCount * sizeof(int));
        memset(ants[i].visited, 0, citiesCount * sizeof(int));

        ants[i].path = (int*) SafeMalloc(citiesCount * sizeof(int));
        memset(ants[i].path, -1, citiesCount * sizeof(int));

        ants[i].previousCity = -1;
        ants[i].currentCity = rand() % citiesCount;
        ants[i].visited[ants[i].currentCity] = 1; // Starting city is set as visited
        ants[i].tourLength = 0;
        ants[i].path[0] = ants[i].currentCity; // Start the path with the current city
        ants[i].pathIndex = 1; // Next city will be placed at index 1
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
    Ant* ant, double** cityMatrix, const int citiesCount, double** pheromoneMatrix
)
{
    double probabilities[citiesCount];

    CalculateTransitionProbabilities(
        ant->currentCity, ant->visited, probabilities, cityMatrix, citiesCount, pheromoneMatrix
    );
    const int nextCity = ChooseNextCity(
        ant->currentCity, ant->visited, probabilities, cityMatrix, citiesCount
    );

    if (nextCity != -1)
    {
        ant->tourLength += cityMatrix[ant->currentCity][nextCity];
        ant->visited[nextCity] = 1;
        ant->path[ant->pathIndex++] = nextCity;
        ant->previousCity = ant->currentCity;
        ant->currentCity = nextCity;
        ant->progress = 0.0;
    }
    else
    {
        printf("ERROR: Next city was not chosen.");
        exit(1);
    }
}

double Attractiveness(double** cityMatrix, const int idx_i, const int idx_j)
{
    if (cityMatrix[idx_i][idx_j] == 0) return 0;
    return 1.0 / cityMatrix[idx_i][idx_j];
}

void CalculateTransitionProbabilities(
    const int currentCity, int visited[], double probabilities[],
    double** cityMatrix, const int citiesCount, double** pheromoneMatrix
)
{
    double probabilityDenominator = 0.0;

    for (int i = 0; i < citiesCount; ++i)
    {
        if (!visited[i] && cityMatrix[currentCity][i] != 0)
        {
            probabilityDenominator += pow(pheromoneMatrix[currentCity][i], ALPHA) *
                pow(Attractiveness(cityMatrix, currentCity, i), BETA);
        }
    }

    for (int i = 0; i < citiesCount; ++i)
    {
        if (!visited[i] && cityMatrix[currentCity][i] != 0)
        {
            probabilities[i] = (
                pow(pheromoneMatrix[currentCity][i], ALPHA) *
                pow(Attractiveness(cityMatrix, currentCity, i), BETA)
            ) / probabilityDenominator;
        }
        else
            probabilities[i] = 0; // City j has already been visited, so probability is 0
    }
}

int ChooseNextCity(
    const int currentCity, int visited[], double probabilities[],
    double** cityMatrix, const int citiesCount
)
{
    const double randomValue = (double)rand() / RAND_MAX;
    double cumulativeProbability = 0.0;

    // Roulette wheel selection
    for (int i = 0; i < citiesCount; ++i)
    {
        if (!visited[i] && cityMatrix[currentCity][i] != 0)
        {
            cumulativeProbability += probabilities[i];
            if (randomValue <= cumulativeProbability)
                return i;
        }
    }

    // In case no city is chosen
    return -1;
}

int AllVisited(const int* visited, const int citiesCount)
{
    int numVisited = 0;
    for (int i = 0; i < citiesCount; ++i)
    {
        if (visited[i])
            numVisited++;
    }
    return citiesCount == numVisited;
}