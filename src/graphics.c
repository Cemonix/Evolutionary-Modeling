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
    button->state = BUTTON_NORMAL;
}

void UpdateButtonState(Button* button, const Vector2 mousePosition)
{
    if (CheckCollisionPointRec(mousePosition, button->bounds))
    {
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
            button->state = BUTTON_PRESSED;
        else
            button->state = BUTTON_HOVERED;
    }
    else
        button->state = BUTTON_NORMAL;
}

void DrawButton(
    const Button* button, const Color buttonColor, const Color buttonHoverColor,
    const int fontsize, const Color textColor
)
{
    if (button->state == BUTTON_HOVERED)
        DrawRectangleRec(button->bounds, buttonHoverColor);
    else
        DrawRectangleRec(button->bounds, buttonColor);

    DrawText(
        button->label, button->bounds.x + 20,
        button->bounds.y + 10, fontsize, textColor
    );
}

void DrawNodeWithLabel(const Vector2 position, const int nodeIndex, const float circleRadius)
{
    char label[20];
    sprintf(label, "Node: %d", nodeIndex);

    DrawCircleV(position, circleRadius, DARKBLUE);
    DrawText(label, position.x + 12, position.y - 5, 10, BLACK);
}

void DrawAntMovement(const Ant* ant, const Node* nodes, const float radius) {
    if (ant->previousNode == ant->currentNode) {
        DrawCircleV(nodes[ant->currentNode].position, radius, RED);
        return;
    }

    const Vector2 startPos = nodes[ant->previousNode].position;
    const Vector2 endPos = nodes[ant->currentNode].position;
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

void DrawSATour(const SAState* saState, const Node* nodes, const size_t nodesCount, const Color lineColor)
{
    if (!saState || !nodes || nodesCount < 2) {
        return;
    }

    for (size_t i = 0; i < nodesCount - 1; ++i) {
        const Vector2 startPos = nodes[saState->currentSolution[i]].position;
        const Vector2 endPos = nodes[saState->currentSolution[i + 1]].position;

        DrawLineEx(startPos, endPos, 1, lineColor);
    }

    const Vector2 startPos = nodes[saState->currentSolution[nodesCount - 1]].position;
    const Vector2 endPos = nodes[saState->currentSolution[0]].position;
    DrawLineEx(startPos, endPos, 1, lineColor);
}

void InitGraphicsWindow()
{
    SimulationType chosenSimType = NONE_SIMULATION;

    Button ACOButton;
    InitButton(
        &ACOButton, "Ant Colony Optimization",
        SCREEN_WIDTH * 0.11, SCREEN_HEIGHT * 0.5, SCREEN_WIDTH * 0.23, 40
    );
    Button SAButton;
    InitButton(
        &SAButton, "Simulated Annealing",
        SCREEN_WIDTH * 0.41, SCREEN_HEIGHT * 0.5, SCREEN_WIDTH * 0.20, 40
    );
    Button DUNNOButton;
    InitButton(
        &DUNNOButton, "Do not know yet",
        SCREEN_WIDTH * 0.66, SCREEN_HEIGHT * 0.5, SCREEN_WIDTH * 0.18, 40
    );

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
    strcpy(tourLabel, "Best tour lenght:");

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "ACO Visualization");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        const float deltaTime = GetFrameTime();
        const Vector2 mousePosition = GetMousePosition();

        if (chosenSimType == NONE_SIMULATION)
        {
            UpdateButtonState(&ACOButton, mousePosition);
            UpdateButtonState(&SAButton, mousePosition);
            UpdateButtonState(&DUNNOButton, mousePosition);

            if (ACOButton.state == BUTTON_PRESSED)
                chosenSimType = ACO_SIMULATION;
            if (SAButton.state == BUTTON_PRESSED)
                chosenSimType = SA_SIMULATION;
            if (DUNNOButton.state == BUTTON_PRESSED)
                chosenSimType = DUNNO_SIMULATION;
        }
        else
        {
            UpdateButtonState(&startButton, mousePosition);
            UpdateButtonState(&resetButton, mousePosition);

            if (startButton.state == BUTTON_PRESSED && nodeCount > 1)
                StartSimulation(chosenSimType);

            if (resetButton.state == BUTTON_PRESSED) {
                ResetSimulation(chosenSimType);
                strcpy(tourLabel, "Best tour lenght:");
            }

            if (
                !CheckCollisionPointRec(mousePosition, leftPanel) &&
                IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
                nodeCount < MAX_NODES &&
                simState == SIMULATION_STOPPED
            ) {
                CreateNode(mousePosition);
            }

            UpdateSimulation(chosenSimType, deltaTime);
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (chosenSimType == NONE_SIMULATION)
        {
            DrawButton(&ACOButton, LIGHTGRAY, GRAY, 20, BLACK);
            DrawButton(&SAButton, LIGHTGRAY, GRAY, 20, BLACK);
            DrawButton(&DUNNOButton, LIGHTGRAY, GRAY, 20, BLACK);
        }
        else
        {
            DrawRectangleRec(leftPanel, leftPanelColor);

            if (bestTour < INT_MAX)
                sprintf(tourLabel, "Best tour lenght: %zu", bestTour);
            DrawText(tourLabel, 10, 10, 20, BLACK);

            sprintf(iterationLabel, "Iteration: %d", iteration);
            DrawText(iterationLabel, 10, 35, 20, BLACK);

            DrawButton(&startButton, LIGHTGRAY, GRAY, 20, BLACK);
            DrawButton(&resetButton, LIGHTGRAY, GRAY, 20, BLACK);

            for (int i = 0; i < nodeCount; ++i)
                DrawNodeWithLabel(nodes[i].position, i, 10);

            if (simState == SIMULATION_RUNNING) {
                switch (chosenSimType) {
                    case ACO_SIMULATION: {
                        for (int i = 0; i < nodeCount; ++i) {
                            for (int j = i + 1; j < nodeCount; ++j) {
                                // Symmetric pheromone levels
                                DrawPheromoneLine(
                                    nodes[i].position, nodes[j].position, pheromoneMatrix[i][j]
                                );
                            }
                        }

                        for (int i = 0; i < NUM_ANTS; ++i) {
                            DrawAntMovement(&ants[i], nodes, 5);
                        }
                        break;
                    }
                    case SA_SIMULATION: {
                        DrawSATour(&saState, nodes, nodeCount, DARKGREEN);
                        break;
                    }
                    default:
                        break;
                }
            }
            else {
                for (int i = 0; i < nodeCount; ++i)
                    for (int j = i + 1; j < nodeCount; ++j)
                        DrawLineEx(nodes[i].position, nodes[j].position, 1, DARKGRAY);
            }
        }
        EndDrawing();
    }

    CloseWindow();

    FreeAnts(ants);
    Free2DArray(nodeMatrix, nodeCount);
    Free2DArray(pheromoneMatrix, nodeCount);
}
