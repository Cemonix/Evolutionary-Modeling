#include "stateController.h"

#include "ant.h"
#include "antColonyOptimization.h"
#include "utils.h"

#include "raymath.h"

SimulationState simState = SIMULATION_STOPPED;
Ant ants[NUM_ANTS];
City cities[MAX_CITIES];
double** cityMatrix = nullptr;
double** pheromoneMatrix = nullptr;
unsigned int citiesCount = 0;
unsigned int iteration = 0;
size_t bestTour = INT_MAX;
bool animating = false;

void StartSimulation()
{
    if (simState != SIMULATION_RUNNING && citiesCount > 1) {
        cityMatrix = (double**) SafeMalloc(citiesCount * sizeof(double*));
        pheromoneMatrix = (double**) SafeMalloc(citiesCount * sizeof(double*));
        FillCityMatrix(cityMatrix, cities, citiesCount);
        InitializeSimulation(ants, pheromoneMatrix, citiesCount);

        simState = SIMULATION_RUNNING;
    }
}

void PauseSimulation()
{
    if (simState == SIMULATION_RUNNING) {
        simState = SIMULATION_PAUSED;
    }
}

void ResetSimulation()
{
    if (simState != SIMULATION_STOPPED) {
        Free2DArray(cityMatrix, citiesCount);
        Free2DArray(pheromoneMatrix, citiesCount);
        citiesCount = 0;
        iteration = 0;
        bestTour = INT_MAX;
        simState = SIMULATION_STOPPED;
    }
}

void InitializeSimulation(Ant* ants, double** pheromoneMatrix, const unsigned int citiesCount)
{
    for (int i = 0; i < citiesCount; ++i)
    {
        pheromoneMatrix[i] = (double*) SafeMalloc(citiesCount * sizeof(double));
        for (int j = 0; j < citiesCount; ++j)
        {
            pheromoneMatrix[i][j] = INITIAL_PHEROMONE;
        }
    }

    InitializeAnts(ants, citiesCount);
}

void UpdateSimulation(const float deltaTime)
{
    if (simState == SIMULATION_RUNNING) {
        int numAntPathFinished = 0;
        if (!animating) {
            for (int i = 0; i < NUM_ANTS; ++i) {
                if (!AllVisited(ants[i].visited, citiesCount)) {
                    AntMove(&ants[i], cityMatrix, citiesCount, pheromoneMatrix);
                }
                else {
                    numAntPathFinished++;
                }
            }
        }

        if (numAntPathFinished == NUM_ANTS) {
            for (int i = 0; i < NUM_ANTS; ++i) {
                if (ants[i].tourLength < bestTour) {
                    bestTour = ants[i].tourLength;
                }
            }
            DepositPheromones(ants, pheromoneMatrix, citiesCount);
            EvaporatePheromones(pheromoneMatrix, citiesCount);
            InitializeAnts(ants, citiesCount);
            iteration++;
            animating = false;
        }
        else
            animating = true;
    }

    if (animating) {
        AnimateAnts(ants, deltaTime, &animating);
    }
}

void AnimateAnts(Ant* ants, const float deltaTime, bool* animating)
{
    *animating = false;
    for (int i = 0; i < NUM_ANTS; ++i) {
        UpdateAnt(&ants[i], deltaTime);
        if (ants[i].previousCity != ants[i].currentCity) {
            *animating = true;
        }
    }
}

void FillCityMatrix(double** cityMatrix, const City* cities, const unsigned int citiesCount)
{
    for (int i = 0; i < citiesCount; i++) {
        cityMatrix[i] = (double*) SafeMalloc(citiesCount * sizeof(double));
        for (int j = 0; j < citiesCount; j++) {
            if (i == j)
                cityMatrix[i][j] = 0.0;
            else
                cityMatrix[i][j] = Vector2Distance(cities[i].position, cities[j].position);
        }
    }
}

void CreateCity(const Vector2 mousePosition)
{
    cities[citiesCount].position = mousePosition;
    citiesCount++;
}