#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "ant.h"

#include "raylib.h"

typedef struct {
    Vector2 position;
} City;

typedef struct {
    Rectangle bounds; // Rectangle struct holds x, y, width, height
    char label[32];
    bool isHovered;
    bool isPressed;
} Button;

void InitButton(Button* button, const char* label, int x, int y, int width, int height);
void InitGraphicsWindow(void);
void DrawCityWithLabel(Vector2 position, int cityIndex, float circleRadius);
void DrawAnt(const Ant* ant, const City* cities, float radius);
void FillCityMatrix(double** cityMatrix, const City* cities, int citiesCount);

#endif
