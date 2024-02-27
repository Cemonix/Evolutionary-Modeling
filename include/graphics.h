#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "ant.h"
#include "enums.h"
#include "geneticAlgorithm.h"
#include "simulatedAnnealing.h"
#include "stateController.h"

#include "raylib.h"

typedef struct {
    Rectangle bounds;
    ButtonState state;
    char label[32];
} Button;

typedef struct {
    float minValue;
    float maxValue;
    float previousValue;
    float currentValue;
    Rectangle sliderBar;
    Rectangle handle;
    float dragOffsetX;
    bool isDragging;
} Slider;

void InitButton(Button* button, const char* label, int x, int y, int width, int height);
void UpdateButtonState(Button* button, Vector2 mousePosition);
void DrawButton(const Button* button, Color buttonColor, Color buttonHoverColor, int fontsize, Color textColor);
void InitSlider(
    Slider* slider, float minValue, float maxValue, float currentValue, Vector2 position, float width, float height
);
void UpdateSlider(Slider* slider, Vector2 mousePosition);
void DrawSlider(const Slider* slider, Color sliderColor, Color handleColor);
void InitGraphicsWindow();
void DrawNodeWithLabel(Vector2 position, int nodeIndex, float circleRadius);
void DrawAnt(const Ant* ant, const Node* nodes, float radius);
void DrawPheromoneLine(Vector2 start, Vector2 end, double pheromoneLevel);
void DrawSATour(const SAState* saState, const Node* nodes, size_t nodeCount, Color lineColor);
void DrawGATour(const GAState* gaState, const Node* nodes, size_t nodeCount, Color lineColor);

#endif
