#include "graphics.h"

#include "ant.h"
#include "config.h"
#include "stateController.h"
#include "utils.h"

#include <math.h>
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
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
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

void InitSlider(
    Slider* slider, const float minValue, const float maxValue, const float currentValue,
    const Vector2 position, const float width, const float height
)
{
    slider->minValue = minValue;
    slider->maxValue = maxValue;
    slider->previousValue = currentValue;
    slider->currentValue = currentValue;
    slider->dragOffsetX = 0.0f;
    slider->isDragging = false;
    slider->sliderBar = (Rectangle){ position.x, position.y, width, height };
    const float handleWidth = height / 2;
    const float handlePosition = (currentValue - minValue) / (maxValue - minValue) * (width - handleWidth);
    slider->handle = (Rectangle){ position.x + handlePosition, position.y, handleWidth, height };
}

void UpdateSlider(Slider* slider, const Vector2 mousePosition)
{
    const bool mouseButtonDown = IsMouseButtonDown(MOUSE_LEFT_BUTTON);
    if (mouseButtonDown && CheckCollisionPointRec(mousePosition, slider->handle)) {
        slider->isDragging = true;
        slider->dragOffsetX = mousePosition.x - slider->handle.x;
    }

    if (slider->isDragging) {
        if (mouseButtonDown) {
            float newPosition = mousePosition.x - slider->dragOffsetX - slider->sliderBar.x;
            if (newPosition < 0)
                newPosition = 0;
            if (newPosition > slider->sliderBar.width - slider->handle.width)
                newPosition = slider->sliderBar.width - slider->handle.width;

            slider->handle.x = slider->sliderBar.x + newPosition;
            const float valueRange = slider->maxValue - slider->minValue;
            slider->currentValue = slider->minValue +
                newPosition / (slider->sliderBar.width - slider->handle.width) * valueRange;
        }
        else {
            slider->previousValue = slider->currentValue;
            slider->isDragging = false;
        }
    }
}

void DrawSlider(const Slider* slider, const Color sliderColor, const Color handleColor)
{
    DrawRectangleRec(slider->sliderBar, sliderColor);
    DrawRectangleRec(slider->handle, handleColor);
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

void DrawSATour(const SAState* saState, const Node* nodes, const size_t nodeCount, const Color lineColor)
{
    if (saState == NULL || nodes == NULL) {
        fprintf(stderr, "Error: Null pointer passed to DrawSATour.\n");
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < nodeCount - 1; ++i) {
        const Vector2 startPos = nodes[saState->currentSolution[i]].position;
        const Vector2 endPos = nodes[saState->currentSolution[i + 1]].position;

        DrawLineEx(startPos, endPos, 1, lineColor);
    }

    const Vector2 startPos = nodes[saState->currentSolution[nodeCount - 1]].position;
    const Vector2 endPos = nodes[saState->currentSolution[0]].position;
    DrawLineEx(startPos, endPos, 1, lineColor);
}

void DrawGATour(const GAState* gaState, const Node* nodes, const size_t nodeCount, const Color lineColor)
{
    if (gaState == NULL || nodes == NULL) {
        fprintf(stderr, "Error: Null pointer passed to DrawGATour.\n");
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < nodeCount - 1; ++i) {
        const Vector2 startPos = nodes[gaState->bestTour[i]].position;
        const Vector2 endPos = nodes[gaState->bestTour[i + 1]].position;

        DrawLineEx(startPos, endPos, 1, lineColor);
    }

    const Vector2 startPos = nodes[gaState->bestTour[nodeCount - 1]].position;
    const Vector2 endPos = nodes[gaState->bestTour[0]].position;
    DrawLineEx(startPos, endPos, 1, lineColor);
}

void InitGraphicsWindow()
{
    SimulationType chosenSimType = NONE_SIMULATION;

    Button ACOButton;
    InitButton(
        &ACOButton, "Ant Colony Optimization",
        SCREEN_WIDTH * 0.11f, SCREEN_HEIGHT * 0.5f, SCREEN_WIDTH * 0.23f, 40
    );
    Button SAButton;
    InitButton(
        &SAButton, "Simulated Annealing",
        SCREEN_WIDTH * 0.41f, SCREEN_HEIGHT * 0.5f, SCREEN_WIDTH * 0.20f, 40
    );
    Button GAButton;
    InitButton(
        &GAButton, "Genetic Algorithm",
        SCREEN_WIDTH * 0.66f, SCREEN_HEIGHT * 0.5f, SCREEN_WIDTH * 0.18f, 40
    );

    const int panelElementsPositionX = SCREEN_WIDTH * 0.775f;

    Button startButton;
    InitButton(
        &startButton, "Start",
        panelElementsPositionX, SCREEN_HEIGHT * 0.05f, 200, 40
    );
    Button resetButton;
    InitButton(
        &resetButton, "Reset",
        panelElementsPositionX, SCREEN_HEIGHT * 0.15f, 200, 40
    );

    Slider nodeSlider;
    InitSlider(
        &nodeSlider, 1.0f, MAX_NODES, MAX_NODES,
        (Vector2){panelElementsPositionX, SCREEN_HEIGHT * 0.25f}, 200, 20
    );

    Button generateNodesButton;
    InitButton(
        &generateNodesButton, "Generate Nodes",
        panelElementsPositionX, SCREEN_HEIGHT * 0.35f, 200, 40
    );

    Button menuButton;
    InitButton(
        &menuButton, "Menu",
        panelElementsPositionX, SCREEN_HEIGHT * 0.8f, 200, 40
    );

    const Rectangle leftPanel = {SCREEN_WIDTH * 0.7f, 0,SCREEN_WIDTH * 0.3f,SCREEN_HEIGHT};
    const Color leftPanelColor = {225, 225, 225, 250};

    char* iterationLabel = SafeMalloc(50 * sizeof(char));
    char* tourLabel = SafeMalloc(100 * sizeof(char));
    char* sliderLabel = SafeMalloc(MAX_NODES + 5 * sizeof(char));

    strcpy(tourLabel, "Best tour lenght:");
    sprintf(sliderLabel, "Nodes to generate: %d", (int)nodeSlider.currentValue);

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
            UpdateButtonState(&GAButton, mousePosition);

            if (ACOButton.state == BUTTON_PRESSED)
                chosenSimType = ACO_SIMULATION;
            if (SAButton.state == BUTTON_PRESSED)
                chosenSimType = SA_SIMULATION;
            if (GAButton.state == BUTTON_PRESSED)
                chosenSimType = GA_SIMULATION;
        }
        else
        {
            UpdateButtonState(&startButton, mousePosition);
            UpdateButtonState(&resetButton, mousePosition);
            UpdateButtonState(&generateNodesButton, mousePosition);
            UpdateButtonState(&menuButton, mousePosition);

            UpdateSlider(&nodeSlider, mousePosition);

            if (startButton.state == BUTTON_PRESSED && nodeCount > 1)
                StartSimulation(chosenSimType);

            if (resetButton.state == BUTTON_PRESSED) {
                ResetSimulation(chosenSimType);
                strcpy(tourLabel, "Best tour lenght:");
            }

            if (generateNodesButton.state == BUTTON_PRESSED && simState == SIMULATION_STOPPED) {
                GenerateNodes(
                    nodes, ceil(nodeSlider.currentValue), SCREEN_WIDTH * 0.69f,
                    SCREEN_HEIGHT, 30
                );
            }

            if (menuButton.state == BUTTON_PRESSED) {
                ResetSimulation(chosenSimType);
                chosenSimType = NONE_SIMULATION;
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
            DrawButton(&GAButton, LIGHTGRAY, GRAY, 20, BLACK);
        }
        else
        {
            DrawRectangleRec(leftPanel, leftPanelColor);

            DrawButton(&startButton, LIGHTGRAY, GRAY, 20, BLACK);
            DrawButton(&resetButton, LIGHTGRAY, GRAY, 20, BLACK);

            DrawSlider(&nodeSlider, GRAY, LIGHTGRAY);
            if (nodeSlider.previousValue != nodeSlider.currentValue)
                sprintf(sliderLabel, "Nodes to generate: %d", (int)ceil(nodeSlider.currentValue));
            DrawText(
                sliderLabel, SCREEN_WIDTH * 0.765f, SCREEN_HEIGHT * 0.3f, 20, BLACK
            );

            DrawButton(&generateNodesButton, LIGHTGRAY, GRAY, 20, BLACK);
            DrawButton(&menuButton, LIGHTGRAY, GRAY, 20, BLACK);

            if (bestTour < INT_MAX)
                sprintf(tourLabel, "Best tour lenght: %zu", bestTour);
            DrawText(tourLabel, 10, 10, 20, BLACK);

            sprintf(iterationLabel, "Iteration: %d", iteration);
            DrawText(iterationLabel, 10, 35, 20, BLACK);

            for (int i = 0; i < nodeCount; ++i)
                DrawNodeWithLabel(nodes[i].position, i, 10);

            if (simState == SIMULATION_RUNNING) {
                if (chosenSimType == ACO_SIMULATION)
                {
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
                }
                else if (chosenSimType == SA_SIMULATION)
                    DrawSATour(&saState, nodes, nodeCount, DARKGREEN);
                else
                    DrawGATour(&gaState, nodes, nodeCount, DARKGREEN);
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

    free(iterationLabel);
    free(tourLabel);
    free(sliderLabel);

    Free2DArray(nodeMatrix, nodeCount);
    if (chosenSimType == ACO_SIMULATION)
    {
        FreeAnts(ants);
        Free2DArray(pheromoneMatrix, nodeCount);
    }
    else if (chosenSimType == GA_SIMULATION)
    {
        FreeGAState(&gaState);
    }
}
