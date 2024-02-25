#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "graphics.h"

int main()
{
    srand(time(NULL));
    InitGraphicsWindow();
    return 0;
}

// TODO: Error when nodes lies on each other
