#include "antColonyOptimization.h"
#include "ant.h"
#include "config.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

void depositPheromones(const Ant* ants, double pheromoneMatrix[NUM_CITIES][NUM_CITIES])
{
    for (int i = 0; i < NUM_ANTS; ++i) {
        const double deltaTau = Q / ants[i].tourLength;
        for (int j = 0; j < NUM_CITIES - 1; ++j) {
            const int city1 = ants[i].path[j];
            const int city2 = ants[i].path[j+1];
            pheromoneMatrix[city1][city2] += deltaTau;
            if (SYMETRIC)
                pheromoneMatrix[city2][city1] += deltaTau;
        }

        const int lastCity = ants[i].path[NUM_CITIES - 1];
        const int firstCity = ants[i].path[0];
        pheromoneMatrix[lastCity][firstCity] += deltaTau;
        pheromoneMatrix[firstCity][lastCity] += deltaTau;
    }
}

void evaporatePheromones(double pheromoneMatrix[NUM_CITIES][NUM_CITIES])
{
    for (int i = 0; i < NUM_CITIES; i++)
    {
        for (int j = 0; j < NUM_CITIES; j++)
        {
            pheromoneMatrix[i][j] *= (1 - RHO);
        }
    }
}

void initializeSimulation(Ant* ants, double pheromoneMatrix[NUM_CITIES][NUM_CITIES])
{
    for (int i = 0; i < NUM_CITIES; ++i)
    {
        for (int j = 0; j < NUM_CITIES; ++j)
        {
            pheromoneMatrix[i][j] = INITIAL_PHEROMONE;
        }
    }

    initializeAnts(ants);
}

void simulation(
    Ant ants[NUM_ANTS], double** cityMatrix, double pheromoneMatrix[NUM_CITIES][NUM_CITIES]
)
{
    initializeSimulation(ants, pheromoneMatrix);

    size_t bestTour = INT_MAX;
    for (int i = 0; i < ITERATIONS; ++i)
    {
        if (VERBOSE)
            printf("Iteration %d\n", i);

        for (int j = 0; j < NUM_ANTS; ++j)
        {
            while (!allVisited(ants[j].visited, NUM_CITIES))
                antMove(&ants[j], cityMatrix, NUM_CITIES, pheromoneMatrix);

            if (ants[j].tourLength < bestTour)
                bestTour = ants[j].tourLength;

            if (VERBOSE)
                printf("Ant: %d tour lenght %llu\n", j, ants[j].tourLength);
        }

        depositPheromones(ants, pheromoneMatrix);
        evaporatePheromones(pheromoneMatrix);
        initializeAnts(ants);
    }

    printf("Total best tour lenght: %llu\n", bestTour);
}