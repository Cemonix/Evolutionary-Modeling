#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VERBOSE 1
#define SYMETRIC 1 // Symetric cityMatrix
#define NUM_CITIES 4
#define NUM_ANTS 5
#define INITIAL_PHEROMONE 1.0
#define RHO 0.1 // Evaporation rate
#define Q 100.0 // Pheromone deposit
#define ALPHA 1.0
#define BETA 2.0
#define ITERATIONS 10

typedef struct {
    int currentCity;
    int visited[NUM_CITIES];
    int path[NUM_CITIES];
    unsigned int pathIndex;
    size_t tourLength;
} Ant;

Ant ants[NUM_ANTS];
double pheromoneMatrix[NUM_CITIES][NUM_CITIES];
double cityMatrix[NUM_CITIES][NUM_CITIES] = {
    {0, 10,  15,  20},
    {10, 0,  40,  10},
    {15,  40,  0,  30},
    {20,  10,  30,  0},
};

void printCityMatrix() {
    printf("City Distance Matrix:\n");
    for (int i = 0; i < NUM_CITIES; ++i) {
        for (int j = 0; j < NUM_CITIES; ++j) {
            if (cityMatrix[i][j] == INFINITY)
                printf("\tINF");
            else
                printf("%7.1f", cityMatrix[i][j]);

        }
        printf("\n");
    }
}

double attractiveness(const int i, const int j)
{
    if (cityMatrix[i][j] == 0) return 0;
    return 1.0 / cityMatrix[i][j];
}

void calculateTransitionProbabilities(const int currentCity, int visited[], double probabilities[]) {
    double probabilityDenominator = 0.0;

    for (int i = 0; i < NUM_CITIES; ++i)
    {
        if (!visited[i] && cityMatrix[currentCity][i] != 0)
        {
            probabilityDenominator += pow(pheromoneMatrix[currentCity][i], ALPHA) *
                pow(attractiveness(currentCity, i), BETA);
        }
    }

    for (int i = 0; i < NUM_CITIES; ++i)
    {
        if (!visited[i] && cityMatrix[currentCity][i] != 0)
        {
            probabilities[i] = (
                pow(pheromoneMatrix[currentCity][i], ALPHA) *
                pow(attractiveness(currentCity, i), BETA)
            ) / probabilityDenominator;
        }
        else
            probabilities[i] = 0; // City j has already been visited, so probability is 0
    }
}

int chooseNextCity(const int currentCity, int visited[], double probabilities[]) {
    const double randomValue = (double)rand() / RAND_MAX;
    double cumulativeProbability = 0.0;

    // Roulette wheel selection
    for (int i = 0; i < NUM_CITIES; ++i)
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

int allVisited(const int* visited)
{
    int numVisited = 0;
    for (int i = 0; i < NUM_CITIES; ++i)
    {
        if (visited[i])
            numVisited++;
    }
    return NUM_CITIES == numVisited;
}

void antMovement(Ant* ant)
{
    while (!allVisited(ant->visited))
    {
        double probabilities[NUM_CITIES];

        calculateTransitionProbabilities(ant->currentCity, ant->visited, probabilities);
        const int nextCity = chooseNextCity(ant->currentCity, ant->visited, probabilities);
        if (nextCity != -1)
        {
            ant->tourLength += cityMatrix[ant->currentCity][nextCity];
            ant->visited[nextCity] = 1;
            ant->path[ant->pathIndex++] = nextCity;
            ant->currentCity = nextCity;
        }
        else
        {
            printf("ERROR: Next city was not chosen.");
            exit(1);
        }
    }
}

void depositPheromones()
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

void evaporatePheromones()
{
    for (int i = 0; i < NUM_CITIES; i++)
    {
        for (int j = 0; j < NUM_CITIES; j++)
        {
            pheromoneMatrix[i][j] *= (1 - RHO);
        }
    }
}

void resetAnts()
{
    for (int i = 0; i < NUM_ANTS; ++i)
    {
        ants[i].currentCity = rand() % NUM_CITIES;
        memset(ants[i].visited, 0, sizeof(ants[i].visited));
        memset(ants[i].path, -1, sizeof(ants[i].path));
        ants[i].visited[ants[i].currentCity] = 1;
        ants[i].tourLength = 0;
        ants[i].path[0] = ants[i].currentCity; // Start the path with the current city
        ants[i].pathIndex = 1; // Next city will be placed at index 1
    }
}

void initialize()
{
    for (int i = 0; i < NUM_CITIES; ++i)
    {
        for (int j = 0; j < NUM_CITIES; ++j)
        {
            pheromoneMatrix[i][j] = INITIAL_PHEROMONE;
        }
    }

    resetAnts();
}

void simulation()
{
    initialize();

    size_t bestTour = INT_MAX;
    for (int i = 0; i < ITERATIONS; ++i)
    {
        if (VERBOSE)
            printf("Iteration %d\n", i);

        for (int j = 0; j < NUM_ANTS; ++j)
        {
            antMovement(&ants[j]);
            if (ants[j].tourLength < bestTour)
                bestTour = ants[j].tourLength;

            if (VERBOSE)
                printf("Ant: %d tour lenght %llu\n", j,ants[j].tourLength);

        }

        depositPheromones();
        evaporatePheromones();
        resetAnts();
    }

    printf("Total best tour lenght: %llu\n", bestTour);
}

int main()
{
    printCityMatrix();
    simulation();
    return 0;
}
