#pragma once

#include "particle.h"
#include "constants.h"

class Cloth {
  private:
    Particle particles[NUM_VERTECES_WIDTH * NUM_VERTECES_HEIGHT];

  public:
    Cloth();
    void Draw();
    void Update(float dt);
};
