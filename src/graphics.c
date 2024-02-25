#include "graphics.h"

#include "ant.h"
#include "config.h"
#include "stateController.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void InitButton(Button* button, const char* label, const int x, const int y, const int width, const int height)
{
    button->bounds = (Rectangle){ x, y, width, height };
    strcpy(button->label, label);
    button->isHovered = false;
}

void ButtonHover(Button* button, const Vector2 mousePosition)
{
    if (CheckCollisionPointRec(mousePosition, button->bounds))
        button->isHovered = true;
    else
        button->isHovered = false;
}

void DrawButton(
    const Button* button, const Color buttonColor, const Color buttonHoverColor,
    const int fontsize, const Color textColor
)
{
    if (button->isHovered)
        DrawRectangleRec(button->bounds, buttonHoverColor);
    else
        DrawRectangleRec(button->bounds, buttonColor);

    DrawText(
        button->label, button->bounds.x + 20,
        button->bounds.y + 10, fontsize, textColor
    );
}

void DrawCityWithLabel(const Vector2 position, const int cityIndex, const float circleRadius)
{
    char label[20];
    sprintf(label, "City: %d", cityIndex);

    DrawCircleV(position, circleRadius, DARKBLUE);
    DrawText(label, position.x + 12, position.y - 5, 10, BLACK);
}

void DrawAntMovement(const Ant* ant, const City* cities, const float radius) {
    if (ant->previousCity == ant->currentCity) {
        DrawCircleV(cities[ant->currentCity].position, radius, RED);
        return;
    }

    const Vector2 startPos = cities[ant->previousCity].position;
    const Vector2 endPos = cities[ant->currentCity].position;
    const Vector2 interpolatedPos = {
        .x = startPos.x + (endPos.x - startPos.x) * ant->progress,
        .y = startPos.y + (endPos.y - startPos.y) * ant->progress
    };

    DrawCircleV(interpolatedPos, radius, RED);
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

void InitGraphicsWindow()
{
    Button startButton;
    InitButton(
        &startButton, "Start",
        SCREEN_WIDTH * 0.775, SCREEN_HEIGHT * 0.05, 200, 40
    );
    Button resetButton;
    InitButton(
        &resetButton, "Reset",
        SCREEN_WIDTH * 0.775, SCREEN_HEIGHT * 0.15, 200, 40
    );

    const Rectangle leftPanel = {SCREEN_WIDTH * 0.7, 0,SCREEN_WIDTH * 0.3,SCREEN_HEIGHT};
    const Color leftPanelColor = {225, 225, 225, 250};

    char* iterationLabel = SafeMalloc(50 * sizeof(char));
    char* tourLabel = SafeMalloc(100 * sizeof(char));

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "ACO Visualization");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        const float deltaTime = GetFrameTime();
        const Vector2 mousePosition = GetMousePosition();

        ButtonHover(&startButton, mousePosition);
        ButtonHover(&resetButton, mousePosition);

        if (startButton.isHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && citiesCount > 1)
            StartSimulation();

        if (resetButton.isHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            ResetSimulation();

        if (
            !CheckCollisionPointRec(mousePosition, leftPanel) &&
            IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
            citiesCount < MAX_CITIES
        ) {
            CreateCity(mousePosition);
        }

        UpdateSimulation(deltaTime);

        BeginDrawing();
        ClearBackground(RAYWHITE);

        for (int i = 0; i < citiesCount; ++i)
        {
            for (int j = i + 1; j < citiesCount; ++j)
            {
                // Symmetric pheromone levels
                const double pheromoneLevel = simState == SIMULATION_RUNNING ?
                    pheromoneMatrix[i][j] : INITIAL_PHEROMONE;
                DrawPheromoneLine(cities[i].position, cities[j].position, pheromoneLevel);
            }
        }

        for (int i = 0; i < citiesCount; ++i)
            DrawCityWithLabel(cities[i].position, i, 10);

        if (simState == SIMULATION_RUNNING) {
            for (int i = 0; i < NUM_ANTS; ++i) {
                DrawAntMovement(&ants[i], cities, 5);
            }
        }

        DrawRectangleRec(leftPanel, leftPanelColor);

        if (bestTour < INT_MAX)
            sprintf(tourLabel, "Best tour lenght: %llu", bestTour);
        else if (bestTour == INT_MAX && strcmp(tourLabel, "Best tour lenght:") == -1)
            strcpy(tourLabel, "Best tour lenght:");
        DrawText(tourLabel, 10, 10, 20, BLACK);

        sprintf(iterationLabel, "Iteration: %d", iteration);
        DrawText(iterationLabel, 10, 35, 20, BLACK);

        DrawButton(&startButton, LIGHTGRAY, GRAY, 20, BLACK);
        DrawButton(&resetButton, LIGHTGRAY, GRAY, 20, BLACK);

        EndDrawing();
    }

    CloseWindow();

    FreeAnts(ants);
    Free2DArray(cityMatrix, citiesCount);
    Free2DArray(pheromoneMatrix, citiesCount);
}
