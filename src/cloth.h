#pragma once

#include "particle.h"
#include "constants.h"
#include <vector>

struct Connection{
    Particle* p1 = nullptr;
    Particle* p2 = nullptr;
};

class Cloth {
  private:
    Particle particles[NUM_VERTICES_WIDTH * NUM_VERTICES_HEIGHT];
    std::vector<Connection> connections;
    float restLength;

  public:
    Cloth();
    void Draw();
    void Update(float dt);
};
