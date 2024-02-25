#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "ant.h"
#include "enums.h"
#include "stateController.h"

#include "raylib.h"

typedef struct {
    Rectangle bounds; // Rectangle struct holds x, y, width, height
    ButtonState state;
    char label[32];
} Button;

void InitButton(Button* button, const char* label, int x, int y, int width, int height);
void UpdateButtonState(Button* button, Vector2 mousePosition);
void DrawButton(const Button* button, Color buttonColor, Color buttonHoverColor, int fontsize, Color textColor);
void InitGraphicsWindow();
void DrawNodeWithLabel(Vector2 position, int nodeIndex, float circleRadius);
void DrawAnt(const Ant* ant, const Node* nodes, float radius);
void DrawPheromoneLine(Vector2 start, Vector2 end, double pheromoneLevel);

#endif
