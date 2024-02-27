#ifndef CONFIG_H
#define CONFIG_H

#define VERBOSE true

#define SCREEN_WIDTH 1400
#define SCREEN_HEIGHT 900

#define MIN_LINE_THICKNESS 0.5
#define MAX_LINE_THICKNESS 8
#define SYMETRIC true // Fully connected graph

#define MAX_NODES 25

// Ant Colony Optimization
#define NUM_ANTS 50
#define INITIAL_PHEROMONE 1.0
#define MAX_PHEROMONE 10
#define RHO 0.1 // Evaporation rate
#define Q 100.0 // Pheromone deposit
#define ALPHA 1.0
#define BETA 3.0
#define ANIMATION_SPEED 3
#define ITERATIONS 10

// Simulated Annealing
#define INITIAL_TEMPERATURE 3500.0
#define FINAL_TEMPERATURE 0.01
#define COOLING_RATE 0.99

// Genetic Algorithm
#define POPULATION_SIZE 50
#define TOURNAMENT_SIZE 5
#define MUTATION_RATE 0.01

#endif
