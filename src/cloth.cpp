#pragma once

#include "cloth.h"
#include "constants.h"
#include "raylib.h"
#include "raymath.h"
#include <cmath>
#include <stdio.h>

Cloth::Cloth() {
    float start_x = 0.2 * SCREEN_WIDTH;
    float end_x = 0.8 * SCREEN_WIDTH;
    float start_y = 0.2 * SCREEN_HEIGHT;
    float end_y = 0.8 * SCREEN_HEIGHT;
    float delX = (end_x - start_x) / NUM_VERTICES_WIDTH;
    float delY = (end_y - start_y) / NUM_VERTICES_HEIGHT;
    restLength = sqrt(delX*delX + delY*delY);

    for (int i = 0; i < NUM_VERTICES_WIDTH; i++) {
        for (int j = 0; j < NUM_VERTICES_HEIGHT; j++) {
            int particle_index = i + NUM_VERTICES_HEIGHT * j;

            particles[particle_index].position = {start_x + i * delX,
                                                  start_y + j * delY};

            particles[particle_index].mass = PARTICLE_MASS;
            particles[particle_index].isFixed = (j == 0); // Top row is fixed

            // Connections
            if (i > 0) { // Has left neighbor
                int left_index = (i - 1) + NUM_VERTICES_HEIGHT * j;
                connections.push_back(
                    {&particles[particle_index], &particles[left_index]});
            }
            if (i < NUM_VERTICES_HEIGHT - 1) { // Has right neighbor
                int right_index = (i + 1) + NUM_VERTICES_HEIGHT * j;
                connections.push_back(
                    {&particles[particle_index], &particles[right_index]});
            }
            if (j > 0) { // Has top neighbor
                int top_index = i + NUM_VERTICES_HEIGHT * (j - 1);
                connections.push_back(
                    {&particles[particle_index], &particles[top_index]});
            }
            if (j < NUM_VERTICES_WIDTH - 1) { // Has bottom neighbor
                int bottom_index = i + NUM_VERTICES_HEIGHT * (j + 1);
                connections.push_back(
                    {&particles[particle_index], &particles[bottom_index]});
            }
        }
    }
}

void Cloth::Draw() {
    for (int i = 0; i < NUM_PARTICLES; i++) {
        particles[i].Draw();
    }
    for (size_t i = 0; i < connections.size(); i++) {
        Connection connection = connections[i];
        Particle *p1 = connection.p1;
        Particle *p2 = connection.p2;

        Color color = CLOTH_COLOR;

        if (p1->isFixed and p2->isFixed) color = FIXED_PARTICLE_COLOR; 
            
        DrawLineEx(p1->position, p2->position, LINE_THICKNESS, color);
    }
}

void Cloth::Update(float dt) {

    // Springs
    for (size_t i = 0; i<connections.size(); i++){
        Connection connection = connections[i];
        Particle* p1 = connection.p1;
        Particle* p2 = connection.p2;

        Vector2 delta = p2->position - p1->position;
        float currentLength = Vector2Length(delta);
        Vector2 direction = Vector2Normalize(delta);
        
        float springForceMag = -SPRING_K * (currentLength - restLength);

        // TODO: add damping force here

        Vector2 totalForce =  Vector2Scale(direction, springForceMag);
        if (!p1->isFixed){
            p1->force = Vector2Add(p1->force, totalForce);
        }

        if (!p2->isFixed){
            p2->force = Vector2Add(p2->force, totalForce);
        }
    }

    for (int i = 0; i<NUM_PARTICLES; i++){
        Particle p = particles[i];
        if (p.isFixed) 
            continue;
        
        Vector2 weight = {0, p.mass * G};
        p.force = Vector2Add(p.force, weight);

        Vector2 acceleration = p.force / p.mass;
        Vector2 accdt = Vector2Scale(acceleration, dt);
        p.velocity = Vector2Add(p.velocity, accdt);

        Vector2 veldt = Vector2Scale(p.velocity, dt);
        p.position = Vector2Add(p.position, veldt);

        /*printf("pos: %f %f\n", p.position.x, p.position.y);*/

        p.force = {0, 0};

    }


}
