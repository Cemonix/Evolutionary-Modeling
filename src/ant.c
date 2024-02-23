#include "ant.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void initializeAnts(Ant* ants)
{
    for (int i = 0; i < NUM_ANTS; ++i)
    {
        ants[i].currentCity = rand() % NUM_CITIES;
        memset(ants[i].visited, 0, sizeof(ants[i].visited));
        memset(ants[i].path, -1, sizeof(ants[i].path));
        ants[i].visited[ants[i].currentCity] = 1; // Starting city is set as visited
        ants[i].tourLength = 0;
        ants[i].path[0] = ants[i].currentCity; // Start the path with the current city
        ants[i].pathIndex = 1; // Next city will be placed at index 1
    }
}

void antMove(
    Ant* ant, double** cityMatrix, const int citiesCount, double pheromoneMatrix[NUM_CITIES][NUM_CITIES]
)
{
    double probabilities[NUM_CITIES];

    calculateTransitionProbabilities(
        ant->currentCity, ant->visited, probabilities, cityMatrix, citiesCount, pheromoneMatrix
    );
    const int nextCity = chooseNextCity(
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

double attractiveness(double** cityMatrix, const int i, const int j)
{
    if (cityMatrix[i][j] == 0) return 0;
    return 1.0 / cityMatrix[i][j];
}

void calculateTransitionProbabilities(
    const int currentCity, int visited[], double probabilities[],
    double** cityMatrix, const int citiesCount, double pheromoneMatrix[NUM_CITIES][NUM_CITIES]
)
{
    double probabilityDenominator = 0.0;

    for (int i = 0; i < citiesCount; ++i)
    {
        if (!visited[i] && cityMatrix[currentCity][i] != 0)
        {
            probabilityDenominator += pow(pheromoneMatrix[currentCity][i], ALPHA) *
                pow(attractiveness(cityMatrix, currentCity, i), BETA);
        }
    }

    for (int i = 0; i < citiesCount; ++i)
    {
        if (!visited[i] && cityMatrix[currentCity][i] != 0)
        {
            probabilities[i] = (
                pow(pheromoneMatrix[currentCity][i], ALPHA) *
                pow(attractiveness(cityMatrix, currentCity, i), BETA)
            ) / probabilityDenominator;
        }
        else
            probabilities[i] = 0; // City j has already been visited, so probability is 0
    }
}

int chooseNextCity(
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

int allVisited(const int* visited, const int citiesCount)
{
    int numVisited = 0;
    for (int i = 0; i < citiesCount; ++i)
    {
        if (visited[i])
            numVisited++;
    }
    return NUM_CITIES == numVisited;
}