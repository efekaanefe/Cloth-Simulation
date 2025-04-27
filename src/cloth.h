#pragma once

#include "body.h"
#include "constants.h"

class Cloth {
  private:
    Body bodies[NUM_VERTECES_WIDTH * NUM_VERTECES_HEIGHT];

  public:
    Cloth();
    void Draw();
    void Update(float dt);
};
