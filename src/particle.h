#pragma once

#include "constants.h"
#include <raylib.h>


class Particle {
  private:
  public:
    float mass;
    Vector2 position;
    bool isFixed;
    
    void Draw() {
        Color color = CLOTH_COLOR;
        if (isFixed == true) color = FIXED_PARTICLE_COLOR; 
        DrawCircle(position.x, position.y, PARTICLE_RADIUS, color);
    }
};
