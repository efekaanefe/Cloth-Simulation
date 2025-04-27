#pragma once

#include "constants.h"
#include <raylib.h>


class Body {
  private:
  public:
    float mass;
    Vector2 position;
    bool is_fixed;
    
    void Draw() {
        DrawCircle(position.x, position.y, BODY_RADIUS, DARKGRAY);
    }
};
