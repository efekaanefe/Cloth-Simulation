#pragma once
#include "cloth.h"
#include "constants.h"
#include "raylib.h"
#include "raymath.h"
#include <algorithm>
#include <cmath>
#include <functional>
#include <stdio.h>
#include <unordered_set>

// Helper function to create a unique key for a connection
size_t connectionKey(int a, int b) {
    return a < b ? (size_t(a) << 32) | b : (size_t(b) << 32) | a;
}

Cloth::Cloth() {
    float start_x = 0.2f * SCREEN_WIDTH;
    float end_x = 0.8f * SCREEN_WIDTH;
    float start_y = 0.2f * SCREEN_HEIGHT;
    float end_y = 0.8f * SCREEN_HEIGHT;
    float delX = (end_x - start_x) / (NUM_VERTICES_WIDTH - 1);
    float delY = (end_y - start_y) / (NUM_VERTICES_HEIGHT - 1);

    /*restLength = std::sqrt(delY*delY + delX*delX);*/
    restLength = std::min(delX, delY);

    // Create all particles first
    for (int i = 0; i < NUM_VERTICES_WIDTH; i++) {
        for (int j = 0; j < NUM_VERTICES_HEIGHT; j++) {
            int particle_index = i * NUM_VERTICES_HEIGHT + j;
            particles[particle_index].position = {start_x + i * delX,
                                                  start_y + j * delY};
            particles[particle_index].mass = PARTICLE_MASS;
            /*particles[particle_index].isFixed = (j == 0); // Top row is fixed*/
            particles[particle_index].isFixed = (j == 0 && i % 2 == 0);
        }
    }

    // Use a set to avoid duplicate connections
    std::unordered_set<size_t> existingConnections;

    // Create connections (springs) between particles
    for (int i = 0; i < NUM_VERTICES_WIDTH; i++) {
        for (int j = 0; j < NUM_VERTICES_HEIGHT; j++) {
            int particle_index = i * NUM_VERTICES_HEIGHT + j;

            // Only create connections in one direction to avoid duplicates
            if (i < NUM_VERTICES_WIDTH - 1) { // Right neighbor
                int right_index = (i + 1) * NUM_VERTICES_HEIGHT + j;
                size_t key = connectionKey(particle_index, right_index);
                if (existingConnections.insert(key).second) {
                    connections.push_back(
                        {&particles[particle_index], &particles[right_index]});
                }
            }

            if (j < NUM_VERTICES_HEIGHT - 1) { // Bottom neighbor
                int bottom_index = i * NUM_VERTICES_HEIGHT + (j + 1);
                size_t key = connectionKey(particle_index, bottom_index);
                if (existingConnections.insert(key).second) {
                    connections.push_back(
                        {&particles[particle_index], &particles[bottom_index]});
                }
            }
            
            /*// Diagonal connections with duplicate check*/
            /*if (i > 0 && j > 0) { // Top-left diagonal*/
            /*    int diag_index = (i - 1) * NUM_VERTICES_HEIGHT + (j - 1);*/
            /*    size_t key = connectionKey(particle_index, diag_index);*/
            /*    if (existingConnections.insert(key).second) {*/
            /*        connections.push_back(*/
            /*            {&particles[particle_index], &particles[diag_index]});*/
            /*    }*/
            /*}*/
            /**/
            /*if (i < NUM_VERTICES_WIDTH - 1 && j > 0) { // Top-right diagonal*/
            /*    int diag_index = (i + 1) * NUM_VERTICES_HEIGHT + (j - 1);*/
            /*    size_t key = connectionKey(particle_index, diag_index);*/
            /*    if (existingConnections.insert(key).second) {*/
            /*        connections.push_back(*/
            /*            {&particles[particle_index], &particles[diag_index]});*/
            /*    }*/
            /*}*/
        }
    }
}

void Cloth::Draw() {
    for (size_t i = 0; i < connections.size(); i++) {
        Connection connection = connections[i];
        Particle *p1 = connection.p1;
        Particle *p2 = connection.p2;
        Color color = CLOTH_COLOR;
        if (p1->isFixed && p2->isFixed)
            color = FIXED_PARTICLE_COLOR;
        DrawLineEx(p1->position, p2->position, LINE_THICKNESS, color);
    }

    for (int i = 0; i < NUM_PARTICLES; i++) {
        particles[i].Draw();
    }
}

void Cloth::Update(float dt) {

    // TODO: drag particle and cut connection

    // Apply spring forces
    for (auto &conn : connections) {
        Particle *p1 = conn.p1;
        Particle *p2 = conn.p2;

        Vector2 delta = Vector2Subtract(p2->position, p1->position);
        float dist = Vector2Length(delta);

        // Skip if particles are too close (avoid division by zero)
        if (dist < MIN_DISTANCE) {
            continue;
        }

        Vector2 dir = Vector2Scale(delta, 1.0f / dist); 

        float springForce = SPRING_K * (dist - restLength);

        Vector2 relVel = Vector2Subtract(p2->velocity, p1->velocity);
        float dampingForce = DAMPING_FACTOR * Vector2DotProduct(relVel, dir);

        // Total force and clamp to prevent explosions
        float totalForce = springForce + dampingForce;
        totalForce = Clamp(totalForce, -MAX_FORCE, MAX_FORCE);

        Vector2 force = Vector2Scale(dir, totalForce);

        if (!p1->isFixed)
            p1->force = Vector2Add(p1->force, force);
        if (!p2->isFixed)
            p2->force = Vector2Subtract(p2->force, force);
    }

    // Update particles
    for (int i = 0; i < NUM_PARTICLES; i++) {
        Particle &p = particles[i];

        if (p.isFixed) {
            p.force = Vector2Zero();
            p.velocity = Vector2Zero();
            continue;
        }

        p.force.y += p.mass * G;

        Vector2 acceleration = Vector2Scale(p.force, 1.0f / p.mass);

        p.velocity = Vector2Add(p.velocity, Vector2Scale(acceleration, dt));

        p.velocity = Vector2Scale(p.velocity, 1.0f - DAMPING_FACTOR * dt);

        float speed = Vector2Length(p.velocity);
        if (speed > MAX_SPEED) {
            p.velocity = Vector2Scale(Vector2Normalize(p.velocity), MAX_SPEED);
        }

        // Check for NaN values
        // BUG: I don't know why sometimes this happens??
        if (std::isnan(p.velocity.x) || std::isnan(p.velocity.y)) {
            p.velocity = Vector2Zero();
        }

        p.position = Vector2Add(p.position, Vector2Scale(p.velocity, dt));

        // Check for NaN positions and reset if needed
        if (std::isnan(p.position.x) || std::isnan(p.position.y)) {
            p.position = {SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f};
            p.velocity = Vector2Zero();
        }

        p.force = Vector2Zero();
    }
}
