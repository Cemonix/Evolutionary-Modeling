#include "graphics.h"

#include "ant.h"
#include "antColonyOptimization.h"
#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "raymath.h"

#define MAX_CITIES 5

City cities[MAX_CITIES];
int citiesCount = 0;

void initButton(Button* button, const char* label, const int x, const int y, const int width, const int height)
{
    button->bounds = (Rectangle){ x, y, width, height };
    strcpy(button->label, label);
    button->isHovered = false;
    button->isPressed = false;
}

void drawCityWithLabel(const Vector2 position, const int cityIndex, const float circleRadius)
{
    char label[20];
    sprintf(label, "City: %d", cityIndex);

    DrawCircleV(position, circleRadius, DARKBLUE);
    DrawText(label, position.x + 12, position.y - 5, 10, BLACK);
}

void drawAntMovement(Ant* ant, const City* cities, const float radius)
{
    const Vector2 startPos = cities[ant->previousCity].position;
    const Vector2 endPos = cities[ant->currentCity].position;
    const Vector2 interpolatedPos = {
        .x = startPos.x + (endPos.x - startPos.x) * ant->progress,
        .y = startPos.y + (endPos.y - startPos.y) * ant->progress
    };

    DrawCircleV(interpolatedPos, radius, RED);

    ant->progress += 0.02;
    // Movement completed
    if (ant->progress >= 1.0) {
        ant->progress = 0.0;
        ant->previousCity = ant->currentCity;
    }
}

void createCityMatrix(double** cityMatrix, const City* cities, const int citiesCount)
{
    for (int i = 0; i < citiesCount; i++)
    {
        cityMatrix[i] = (double*) malloc(citiesCount * sizeof(double));
    }

    for (int i = 0; i < citiesCount; i++) {
        for (int j = 0; j < citiesCount; j++) {
            if (i == j)
                cityMatrix[i][j] = 0.0;
            else
                cityMatrix[i][j] = Vector2Distance(cities[i].position, cities[j].position);
        }
    }
}

void initGraphicsWindow()
{
    bool matrixInited = false;
    bool animating = false;
    Button startSimulationButton;
    initButton(
        &startSimulationButton, "Start Simulation",
        SCREEN_WIDTH - 210, SCREEN_HEIGHT * 0.05, 200, 40
    );

    Ant ants[NUM_ANTS];
    double** cityMatrix = nullptr;
    double pheromoneMatrix[NUM_CITIES][NUM_CITIES];

    initializeSimulation(ants, pheromoneMatrix);

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "ACO Visualization");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        const Vector2 mousePosition = GetMousePosition();
        if (CheckCollisionPointRec(mousePosition, startSimulationButton.bounds))
            startSimulationButton.isHovered = true;
        else if (!startSimulationButton.isPressed)
        {
            startSimulationButton.isHovered = false;

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && citiesCount < MAX_CITIES)
            {
                cities[citiesCount].position = mousePosition;
                citiesCount++;
            }
        }

        if (startSimulationButton.isHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            startSimulationButton.isPressed = true;
        }

        if (startSimulationButton.isPressed)
        {
            if (!matrixInited)
            {
                cityMatrix = (double**) malloc(citiesCount * sizeof(double*));
                createCityMatrix(cityMatrix, cities, citiesCount);
                matrixInited = true;
            }

            if (!animating && !allVisited(ants[0].visited, citiesCount))
            {
                antMove(&ants[0], cityMatrix, citiesCount, pheromoneMatrix);
                animating = true;
            }
            else
            {
                drawAntMovement(&ants[0], cities, 5);
                if (ants[0].previousCity == ants[0].currentCity)
                    animating = false;
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (startSimulationButton.isHovered)
            DrawRectangleRec(startSimulationButton.bounds, GRAY);
        else
            DrawRectangleRec(startSimulationButton.bounds, LIGHTGRAY);

        DrawText(
            startSimulationButton.label, startSimulationButton.bounds.x + 20,
            startSimulationButton.bounds.y + 10, 20, BLACK
        );

        for (int i = 0; i < citiesCount; ++i)
        {
            for (int j = i + 1; j < citiesCount; ++j)
            {
                DrawLineV(cities[i].position, cities[j].position, LIGHTGRAY);
            }
        }

        for (int i = 0; i < citiesCount; ++i)
        {
            drawCityWithLabel(cities[i].position, i, 10);
        }

        EndDrawing();
    }

    CloseWindow();
    free(cityMatrix);
}
