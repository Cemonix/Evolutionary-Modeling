#include "geneticAlgorithm.h"

#include "config.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

GAState gaState;

void InitializeGAState(GAState* state, const size_t nodeCount)
{
    state->currentPopulation = InitializeGAPopulation(nodeCount);
    state->bestCost = SIZE_MAX;
    state->bestTour = SafeMalloc(nodeCount * sizeof(size_t));

    for (size_t i = 0; i < nodeCount; ++i) {
        state->bestTour[i] = i;
    }
}

GAPopulation* InitializeGAPopulation(const size_t nodeCount)
{
    const size_t tourSize = nodeCount * sizeof(size_t);

    GAPopulation* population = SafeMalloc(sizeof(GAPopulation));
    population->tours = SafeMalloc(POPULATION_SIZE * sizeof(GATour));
    population->populationSize = POPULATION_SIZE;
    population->bestCost = SIZE_MAX;
    population->bestTour = SafeMalloc(tourSize);

    // Allocate memory for the tour array within each GATour
    for (size_t i = 0; i < POPULATION_SIZE; ++i) {
        population->tours[i].tour = SafeMalloc(tourSize);
        for (size_t j = 0; j < nodeCount; ++j) {
            population->tours[i].tour[j] = j;
        }

        // Shuffle the tour to introduce diversity
        for (size_t j = nodeCount - 1; j > 0; --j) {
            const size_t swapIndex = rand() % (j + 1);
            const size_t temp = population->tours[i].tour[j];
            population->tours[i].tour[j] = population->tours[i].tour[swapIndex];
            population->tours[i].tour[swapIndex] = temp;
        }
    }

    return population;
}

void RunGeneticAlgorithm(
    GAState* state, double** nodeMatrix, const size_t nodeCount
)
{
    if (state->currentPopulation == NULL || nodeMatrix == NULL) {
        fprintf(stderr, "Error: Null pointer passed to RunGeneticAlgorithm.\n");
        exit(EXIT_FAILURE);
    }

    EvaluatePopulation(state->currentPopulation, nodeMatrix, nodeCount);

    GAPopulation* newPopulation = InitializeGAPopulation(nodeCount);

    for (size_t i = 0; i < state->currentPopulation->populationSize; ++i) {
        GATour parent1, parent2;
        do {
            parent1 = TournamentSelection(state->currentPopulation, TOURNAMENT_SIZE);
            parent2 = TournamentSelection(state->currentPopulation, TOURNAMENT_SIZE);
        } while (AreToursIdentical(&parent1, &parent2, nodeCount));

        GATour offspring1;
        PMXCrossover(&parent1, &parent2, &offspring1, nodeCount);
        Mutate(&offspring1, nodeCount, MUTATION_RATE);
        newPopulation->tours[i] = offspring1;
    }

    FreeGAPopulation(state->currentPopulation);

    state->currentPopulation = newPopulation;
    EvaluatePopulation(state->currentPopulation, nodeMatrix, nodeCount);

    for (size_t i = 0; i < state->currentPopulation->populationSize; ++i) {
        if (state->currentPopulation->tours[i].cost < state->bestCost) {
            state->bestCost = state->currentPopulation->tours[i].cost;
            memcpy(state->bestTour, state->currentPopulation->tours[i].tour, nodeCount * sizeof(size_t));
            if (VERBOSE)
                printf("New best cost: %zu\n", state->bestCost);
        }
    }
}

void EvaluatePopulation(const GAPopulation* population, double** nodeMatrix, const size_t nodeCount) {
    if (population == NULL || nodeMatrix == NULL) {
        fprintf(stderr, "Error: Null pointer passed to EvaluatePopulation.\n");
        exit(EXIT_FAILURE);
    }

    double maxCost = 0.0;
    for (size_t i = 0; i < population->populationSize; ++i) {
        const double cost = CalculateSolutionCost(population->tours[i].tour, nodeMatrix, nodeCount);
        if (cost > maxCost) {
            maxCost = cost;
        }
        population->tours[i].cost = cost;
    }

    for (size_t i = 0; i < population->populationSize; ++i) {
        // Adding 1 to cost to avoid division by zero
        population->tours[i].fitness = 1.0 / (population->tours[i].cost + 1);
        // Normalize fitness by maxCost
        population->tours[i].fitness = (maxCost - population->tours[i].cost) / maxCost;
    }
}

void PMXCrossover(const GATour* parent1, const GATour* parent2, GATour* offspring, const size_t nodeCount)
{
    offspring->tour = (size_t*) SafeMalloc(nodeCount * sizeof(size_t));

    const size_t start = rand() % nodeCount;
    const size_t end = start + rand() % (nodeCount - start);

    for (size_t i = 0; i < nodeCount; ++i) {
        if (i >= start && i <= end)
            offspring->tour[i] = parent2->tour[i];
        else
            offspring->tour[i] = SIZE_MAX;
    }

    for (size_t i = 0; i < nodeCount; i++) {
        if (i >= start && i <= end)
            continue;

        size_t node = parent1->tour[i];

        bool isInSegment;
        do {
            isInSegment = false;
            for (size_t j = start; j <= end; ++j) {
                if (offspring->tour[j] == node) {
                    isInSegment = true;
                    node = parent1->tour[j];
                    break;
                }
            }
            if (!isInSegment)
                offspring->tour[i] = node;

        } while (isInSegment);
        offspring->tour[i] = node;
    }
}

void Mutate(const GATour* tour, const size_t nodeCount, const double mutationRate)
{
    for (size_t i = 0; i < nodeCount; ++i) {
        if ((double)rand() / RAND_MAX < mutationRate) {
            const size_t j = rand() % nodeCount;
            const size_t temp = tour->tour[i];
            tour->tour[i] = tour->tour[j];
            tour->tour[j] = temp;
        }
    }
}

GATour TournamentSelection(const GAPopulation* population, const size_t tournamentSize)
{
    GATour* tournament = SafeMalloc(tournamentSize * sizeof(GATour));

    // Randomly select individuals for the tournament
    for (size_t i = 0; i < tournamentSize; ++i) {
        const size_t randomIndex = rand() % population->populationSize;
        tournament[i] = population->tours[randomIndex];
    }

    // Find the individual with the highest fitness in the tournament
    GATour fittest = tournament[0];
    for (size_t i = 1; i < tournamentSize; ++i) {
        if (tournament[i].fitness > fittest.fitness) {
            fittest = tournament[i];
        }
    }

    free(tournament);
    return fittest;
}

bool AreToursIdentical(const GATour* tour1, const GATour* tour2, const size_t nodeCount)
{
    for (size_t i = 0; i < nodeCount; ++i) {
        if (tour1->tour[i] != tour2->tour[i]) {
            return false;
        }
    }
    return true;
}

void FreeGAPopulation(GAPopulation* population)
{
    if (population != NULL) {
        for (size_t i = 0; i < population->populationSize; ++i) {
            free(population->tours[i].tour);
        }

        free(population->tours);
        free(population->bestTour);
        free(population);
    }
}

void FreeGAState(GAState* state)
{
    if (state != NULL) {
        FreeGAPopulation(state->currentPopulation);
        free(state->bestTour);
        free(state);
    }
}