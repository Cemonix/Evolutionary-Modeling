#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "antColonyOptimization.h"
#include "graphics.h"

// double cityMatrix[NUM_CITIES][NUM_CITIES] = {
//     {0, 10,  15,  20},
//     {10, 0,  40,  10},
//     {15,  40,  0,  30},
//     {20,  10,  30,  0},
// };

int main()
{
    srand(time(NULL));
    initGraphicsWindow();
    // simulation(cityMatrix);
    return 0;
}

// TODO: Convert heromone matrix to heap allocation
// TODO: Simulate and animate multiple ants
