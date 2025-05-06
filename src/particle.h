#pragma once
#include "constants.h"
#include <raylib.h>

class Particle {
  private:
  public:
    float mass;
    Vector3 position;
    Vector3 velocity;
    Vector3 force;
    bool isFixed;
    
    void Draw() {
        Color color = CLOTH_COLOR;
        if (isFixed == true) color = FIXED_PARTICLE_COLOR; 
        DrawSphere(position, PARTICLE_RADIUS, color);
    }
};
