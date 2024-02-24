#include "antColonyOptimization.h"
#include "ant.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

void DepositPheromones(const Ant* ants, double** pheromoneMatrix, const int citiesCount)
{
    for (int i = 0; i < NUM_ANTS; ++i) {
        const double deltaTau = Q / ants[i].tourLength;
        for (int j = 0; j < citiesCount - 1; ++j) {
            const int city1 = ants[i].path[j];
            const int city2 = ants[i].path[j+1];
            pheromoneMatrix[city1][city2] += deltaTau;
            if (SYMETRIC)
                pheromoneMatrix[city2][city1] += deltaTau;
        }

        const int lastCity = ants[i].path[citiesCount - 1];
        const int firstCity = ants[i].path[0];
        pheromoneMatrix[lastCity][firstCity] += deltaTau;
        pheromoneMatrix[firstCity][lastCity] += deltaTau;
    }
}

void EvaporatePheromones(double** pheromoneMatrix, const int citiesCount)
{
    for (int i = 0; i < citiesCount; i++)
    {
        for (int j = 0; j < citiesCount; j++)
        {
            pheromoneMatrix[i][j] *= (1 - RHO);
        }
    }
}

void InitializeSimulation(Ant* ants, double** pheromoneMatrix, const int citiesCount)
{
    for (int i = 0; i < citiesCount; ++i)
    {
        for (int j = 0; j < citiesCount; ++j)
        {
            pheromoneMatrix[i][j] = INITIAL_PHEROMONE;
        }
    }

    InitializeAnts(ants, citiesCount);
}

void Simulation(
    Ant ants[NUM_ANTS], double** cityMatrix, const int citiesCount, double** pheromoneMatrix
)
{
    InitializeSimulation(ants, pheromoneMatrix, citiesCount);

    size_t bestTour = INT_MAX;
    for (int i = 0; i < ITERATIONS; ++i)
    {
        if (VERBOSE)
            printf("Iteration %d\n", i);

        for (int j = 0; j < NUM_ANTS; ++j)
        {
            while (!AllVisited(ants[j].visited, citiesCount))
                AntMove(&ants[j], cityMatrix, citiesCount, pheromoneMatrix);

            if (ants[j].tourLength < bestTour)
                bestTour = ants[j].tourLength;

            if (VERBOSE)
                printf("Ant: %d tour lenght %llu\n", j, ants[j].tourLength);
        }

        DepositPheromones(ants, pheromoneMatrix, citiesCount);
        EvaporatePheromones(pheromoneMatrix, citiesCount);
        InitializeAnts(ants, citiesCount);
    }

    printf("Total best tour lenght: %llu\n", bestTour);
}