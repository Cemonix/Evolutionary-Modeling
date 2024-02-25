#ifndef CONFIG_H
#define CONFIG_H

#define VERBOSE true

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 800

#define ANIMATION_SPEED 3
#define MIN_LINE_THICKNESS 0.5
#define MAX_LINE_THICKNESS 8
#define SYMETRIC true // Fully connected graph
#define MAX_NODES 15

// Ant Colony Optimization
#define NUM_ANTS 50
#define INITIAL_PHEROMONE 1.0
#define MAX_PHEROMONE 10
#define RHO 0.1 // Evaporation rate
#define Q 100.0 // Pheromone deposit
#define ALPHA 1.0
#define BETA 3.0
#define ITERATIONS 10

// Simulated Annealing
#define INITIAL_TEMPERATURE 1000.0
#define FINAL_TEMPERATURE 1.0
#define COOLING_RATE 0.99
#define STEPS_PER_TEMPERATURE 100

#endif
