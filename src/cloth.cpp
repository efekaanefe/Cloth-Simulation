#pragma once

#include "cloth.h"
#include "constants.h"
#include "raylib.h"

Cloth::Cloth() {
    float start_x = 0.2 * SCREEN_WIDTH;
    float end_x = 0.8 * SCREEN_WIDTH;
    float start_y = 0.2 * SCREEN_HEIGHT;
    float end_y = 0.8 * SCREEN_HEIGHT;
    float del_x = (end_x - start_x) / NUM_VERTICES_WIDTH;
    float del_y = (end_y - start_y) / NUM_VERTICES_HEIGHT;

    for (int i = 0; i < NUM_VERTICES_WIDTH; i++) {
        for (int j = 0; j < NUM_VERTICES_HEIGHT; j++) {
            int particle_index = i + NUM_VERTICES_HEIGHT * j;

            particles[particle_index].position = {start_x + i * del_x,
                                                  start_y + j * del_y};

            particles[particle_index].mass = PARTICLE_MASS;
            particles[particle_index].is_fixed = true;

            // Connections
            if (i > 0) { // Has left neighbor
                int left_index = (i - 1) + NUM_VERTICES_HEIGHT * j;
                connections.push_back({&particles[particle_index], &particles[left_index]});
            }
            if (i < NUM_VERTICES_HEIGHT - 1) { // Has right neighbor
                int right_index = (i + 1) + NUM_VERTICES_HEIGHT * j;
                connections.push_back({&particles[particle_index], &particles[right_index]});
            }
            if (j > 0) { // Has top neighbor
                int top_index = i + NUM_VERTICES_HEIGHT * (j - 1);
                connections.push_back({&particles[particle_index], &particles[top_index]});
            }
            if (j < NUM_VERTICES_WIDTH - 1) { // Has bottom neighbor
                int bottom_index = i + NUM_VERTICES_HEIGHT * (j + 1);
                connections.push_back({&particles[particle_index], &particles[bottom_index]});
            }
        }
    }
}

void Cloth::Draw() {
    for (int i = 0; i < NUM_PARTICLES; i++) {
        particles[i].Draw();
    }
    for (size_t i = 0; i < connections.size(); ++i){
        Connection connection = connections[i];
        Particle* p1 = connection.p1;
        Particle* p2 = connection.p2;
        DrawLine(p1->position.x, p1->position.y, p2->position.x, p2->position.y, CLOTH_COLOR);
    }
}

void Cloth::Update(float dt) {}
