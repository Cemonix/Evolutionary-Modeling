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

void initButton(Button* button, const char* label, int x, int y, int width, int height);
void initGraphicsWindow(void);
void drawCityWithLabel(Vector2 position, int cityIndex, float circleRadius);
void drawAnt(const Ant* ant, const City* cities, float radius);
void createCityMatrix(double** cityMatrix, const City* cities, int citiesCount);

#endif
