#ifndef CONFIG_H
#define CONFIG_H

#define VERBOSE true

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 800

#define ANIMATION_SPEED 0.05
#define MIN_LINE_THICKNESS 0.5
#define MAX_LINE_THICKNESS 6

#define SYMETRIC true // Fully connected graph
#define MAX_CITIES 10
#define NUM_ANTS 15
#define INITIAL_PHEROMONE 1.0
#define MAX_PHEROMONE 10
#define RHO 0.1 // Evaporation rate
#define Q 100.0 // Pheromone deposit
#define ALPHA 1.0
#define BETA 2.0
#define ITERATIONS 10

#endif
