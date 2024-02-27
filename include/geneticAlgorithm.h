#ifndef GENETICALGORITHM_H
#define GENETICALGORITHM_H

#include <stddef.h>

typedef struct {
    size_t* tour;
    double cost;
    double fitness;
} GATour;

typedef struct {
    GATour* tours;
    size_t populationSize;
    size_t* bestTour;
    size_t bestCost;
} GAPopulation;

typedef struct {
    GAPopulation* currentPopulation; // Pointer to the current population
    size_t* bestTour;                // Array representing the best tour found
    size_t bestCost;                 // Cost of the best tour found
} GAState;

extern GAState gaState;

void InitializeGAState(GAState* state, size_t nodeCount);
GAPopulation* InitializeGAPopulation(size_t nodeCount);
void RunGeneticAlgorithm(GAState* state, double** nodeMatrix, size_t nodeCount);
void EvaluatePopulation(const GAPopulation* population, double** nodeMatrix, size_t nodeCount);
void PMXCrossover(const GATour* parent1, const GATour* parent2, GATour* offspring, size_t nodeCount);
void Mutate(const GATour* tour, size_t nodeCount, double mutationRate);
GATour TournamentSelection(const GAPopulation* population, size_t tournamentSize);
bool AreToursIdentical(const GATour* tour1, const GATour* tour2, size_t nodeCount);
void FreeGAPopulation(GAPopulation* population);
void FreeGAState(GAState* state);

#endif
