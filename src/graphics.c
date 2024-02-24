#include "graphics.h"
#include "ant.h"
#include "antColonyOptimization.h"
#include "config.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "raymath.h"

City cities[MAX_CITIES];
int citiesCount = 0;

void InitButton(Button* button, const char* label, const int x, const int y, const int width, const int height)
{
    button->bounds = (Rectangle){ x, y, width, height };
    strcpy(button->label, label);
    button->isHovered = false;
    button->isPressed = false;
}

void DrawCityWithLabel(const Vector2 position, const int cityIndex, const float circleRadius)
{
    char label[20];
    sprintf(label, "City: %d", cityIndex);

    DrawCircleV(position, circleRadius, DARKBLUE);
    DrawText(label, position.x + 12, position.y - 5, 10, BLACK);
}

void DrawAntMovement(Ant* ant, const City* cities, const float radius)
{
    const Vector2 startPos = cities[ant->previousCity].position;
    const Vector2 endPos = cities[ant->currentCity].position;
    const Vector2 interpolatedPos = {
        .x = startPos.x + (endPos.x - startPos.x) * ant->progress,
        .y = startPos.y + (endPos.y - startPos.y) * ant->progress
    };

    DrawCircleV(interpolatedPos, radius, RED);

    ant->progress += ANIMATION_SPEED;
    // Movement completed
    if (ant->progress >= 1.0) {
        ant->progress = 0.0;
        ant->previousCity = ant->currentCity;
    }
}

void DrawPheromoneLine(const Vector2 start, const Vector2 end, const double pheromoneLevel)
{
    float thickness = MIN_LINE_THICKNESS;
    if (pheromoneLevel > 0) {
        thickness += (MAX_LINE_THICKNESS - MIN_LINE_THICKNESS) * (pheromoneLevel / MAX_PHEROMONE);
        thickness = thickness > MAX_LINE_THICKNESS ? MAX_LINE_THICKNESS : thickness; // Clamp value
    }

    DrawLineEx(start, end, thickness, DARKGRAY);
}

void FillCityMatrix(double** cityMatrix, const City* cities, const int citiesCount)
{
    for (int i = 0; i < citiesCount; i++)
    {
        cityMatrix[i] = (double*) SafeMalloc(citiesCount * sizeof(double));
        for (int j = 0; j < citiesCount; j++)
        {
            if (i == j)
                cityMatrix[i][j] = 0.0;
            else
                cityMatrix[i][j] = Vector2Distance(cities[i].position, cities[j].position);
        }
    }
}

void InitGraphicsWindow()
{
    bool matrixInitilized = false;
    bool animating = false;

    Button startSimulationButton;
    InitButton(
        &startSimulationButton, "Start Simulation",
        SCREEN_WIDTH * 0.775, SCREEN_HEIGHT * 0.05, 200, 40
    );
    const Rectangle leftPanel = {SCREEN_WIDTH * 0.7, 0,SCREEN_WIDTH * 0.3,SCREEN_HEIGHT};
    const Color leftPanelColor = {225, 225, 225, 250};

    Ant ants[NUM_ANTS];
    double** cityMatrix = nullptr;
    double** pheromoneMatrix = nullptr;

    unsigned int iteration = 0;
    char* iterationLabel = SafeMalloc(50 * sizeof(char));

    size_t bestTour = INT_MAX;
    char* tourLabel = SafeMalloc(100 * sizeof(char));
    strcpy(tourLabel, "Best tour lenght:");

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "ACO Visualization");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        const Vector2 mousePosition = GetMousePosition();
        if (CheckCollisionPointRec(mousePosition, startSimulationButton.bounds))
            startSimulationButton.isHovered = true;
        else
            startSimulationButton.isHovered = false;

        if (startSimulationButton.isHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            startSimulationButton.isPressed = true;
        }

        if (!CheckCollisionPointRec(mousePosition, leftPanel))
        {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && citiesCount < MAX_CITIES)
            {
                cities[citiesCount].position = mousePosition;
                citiesCount++;
            }
        }

        if (startSimulationButton.isPressed && citiesCount > 1)
        {
            if (!matrixInitilized)
            {
                cityMatrix = (double**) SafeMalloc(citiesCount * sizeof(double*));
                pheromoneMatrix = (double**) SafeMalloc(citiesCount * sizeof(double*));

                FillCityMatrix(cityMatrix, cities, citiesCount);
                InitializeSimulation(ants, pheromoneMatrix, citiesCount);

                matrixInitilized = true;
            }

            if (!animating)
            {
                int numAntPathFinished = 0;
                for (int i = 0; i < NUM_ANTS; ++i)
                {
                    if (!AllVisited(ants[i].visited, citiesCount))
                        AntMove(&ants[i], cityMatrix, citiesCount, pheromoneMatrix);
                    else
                        numAntPathFinished++;
                }

                if (numAntPathFinished == NUM_ANTS)
                {
                    for (int i = 0; i < NUM_ANTS; ++i)
                    {
                        if (ants[i].tourLength < bestTour)
                            bestTour = ants[i].tourLength;
                    }

                    DepositPheromones(ants, pheromoneMatrix, citiesCount);
                    EvaporatePheromones(pheromoneMatrix, citiesCount);
                    InitializeAnts(ants, citiesCount);
                    iteration++;
                }
                else
                    animating = true;
            }
            else
            {
                for (int i = 0; i < NUM_ANTS; ++i)
                {
                    DrawAntMovement(&ants[i], cities, 5);
                }

                for (int i = 0; i < NUM_ANTS; ++i)
                {
                    animating = (ants[i].previousCity != ants[i].currentCity);
                    if (animating)
                        break;
                }
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawRectangleRec(leftPanel, leftPanelColor);

        if (bestTour < INT_MAX)
            sprintf(tourLabel, "Best tour lenght: %llu", bestTour);
        DrawText(tourLabel, 10, 10, 20, BLACK);

        sprintf(iterationLabel, "Iteration: %d", iteration);
        DrawText(iterationLabel, 10, 35, 20, BLACK);

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
                // Symmetric pheromone levels
                const double pheromoneLevel = matrixInitilized ? pheromoneMatrix[i][j] : INITIAL_PHEROMONE;
                DrawPheromoneLine(cities[i].position, cities[j].position, pheromoneLevel);
            }
        }

        for (int i = 0; i < citiesCount; ++i)
        {
            DrawCityWithLabel(cities[i].position, i, 10);
        }

        EndDrawing();
    }

    CloseWindow();

    FreeAnts(ants);
    free(cityMatrix);
    free(pheromoneMatrix);
}
