#include "Map.h"

void View::update()
{
    float factor = 0.01f;
    posX = (1.f - factor) * posX + factor * targetX;
    posY = (1.f - factor) * posY + factor * targetY;
}