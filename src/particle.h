#pragma once

#include "constants.h"
#include <raylib.h>


class Particle {
  private:
  public:
    float mass;
    Vector2 position;
    bool is_fixed;
    
    void Draw() {
        DrawCircle(position.x, position.y, PARTICLE_RADIUS, CLOTH_COLOR);
    }
};
