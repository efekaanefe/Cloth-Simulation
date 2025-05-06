#include "cloth.h"
#include "constants.h"
#include "raylib.h"
#include "raymath.h"
#include <algorithm>
#include <cmath>
#include <stdio.h>
#include <unordered_set>
#include <cmath>

// Helper function to create a unique key for a connection
size_t connectionKey(int a, int b) {
    return a < b ? (size_t(a) << 32) | b : (size_t(b) << 32) | a;
}

Vector3 GetWind(Vector3 pos, float time) {
    float scale = 0.03f;
    float timeScale = 0.9f;

    float noiseX = sinf(pos.y * scale + time * timeScale);
    float noiseY = cosf(pos.x * scale + time * timeScale);
    float noiseZ = sinf((pos.x + pos.y) * scale + time * timeScale * 1.5f);

    return Vector3{ noiseX, noiseY, noiseZ };
}

Cloth::Cloth() {
    float padding = 0.4f;
    float start_x =  padding * SCREEN_WIDTH;
    float end_x = (1-padding) * SCREEN_WIDTH;
    float start_y = padding * SCREEN_HEIGHT;
    float end_y = (1-padding) * SCREEN_HEIGHT;
    float z_position = 0.0f; 
    float delX = (end_x - start_x) / (NUM_VERTICES_WIDTH - 1);
    float delY = (end_y - start_y) / (NUM_VERTICES_HEIGHT - 1);

    restLength = std::min(delX, delY);

    // Create all particles first
    for (int i = 0; i < NUM_VERTICES_WIDTH; i++) {
        for (int j = 0; j < NUM_VERTICES_HEIGHT; j++) {
            int particle_index = i * NUM_VERTICES_HEIGHT + j;
            particles[particle_index].position = {
                start_x + i * delX,
                start_y + j * delY,
                z_position
            };
            particles[particle_index].mass = PARTICLE_MASS;
            particles[particle_index].isFixed = (j == 0 && i % 5 == 0);
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
            
            // Uncomment if you want diagonal connections
            /*
            if (i > 0 && j > 0) { // Top-left diagonal
                int diag_index = (i - 1) * NUM_VERTICES_HEIGHT + (j - 1);
                size_t key = connectionKey(particle_index, diag_index);
                if (existingConnections.insert(key).second) {
                    connections.push_back(
                        {&particles[particle_index], &particles[diag_index]});
                }
            }
            
            if (i < NUM_VERTICES_WIDTH - 1 && j > 0) { // Top-right diagonal
                int diag_index = (i + 1) * NUM_VERTICES_HEIGHT + (j - 1);
                size_t key = connectionKey(particle_index, diag_index);
                if (existingConnections.insert(key).second) {
                    connections.push_back(
                        {&particles[particle_index], &particles[diag_index]});
                }
            }
            */
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
        DrawLine3D(p1->position, p2->position, color);
    }

    for (int i = 0; i < NUM_PARTICLES; i++) {
        particles[i].Draw();
    }
}

void Cloth::Update(float dt, float totalFrames) {
    // Apply spring forces
    for (auto &conn : connections) {
        Particle *p1 = conn.p1;
        Particle *p2 = conn.p2;

        Vector3 delta = Vector3Subtract(p2->position, p1->position);
        float dist = Vector3Length(delta);

        // Skip if particles are too close (avoid division by zero)
        if (dist < MIN_DISTANCE) {
            continue;
        }

        Vector3 dir = Vector3Scale(delta, 1.0f / dist); 

        float springForce = SPRING_K * (dist - restLength);

        Vector3 relVel = Vector3Subtract(p2->velocity, p1->velocity);
        float dampingForce = DAMPING_FACTOR * Vector3DotProduct(relVel, dir);

        // Total force and clamp to prevent explosions
        float totalForce = springForce + dampingForce;
        totalForce = Clamp(totalForce, -MAX_FORCE, MAX_FORCE);

        Vector3 force = Vector3Scale(dir, totalForce);

        if (!p1->isFixed)
            p1->force = Vector3Add(p1->force, force);
        if (!p2->isFixed)
            p2->force = Vector3Subtract(p2->force, force);
    }

    // Update particles
    for (int i = 0; i < NUM_PARTICLES; i++) {
        Particle &particle = particles[i];

        if (particle.isFixed) {
            particle.force = Vector3Zero();
            particle.velocity = Vector3Zero();
            continue;
        }

        Vector3 wind = GetWind(particle.position, totalFrames);
        particle.force = Vector3Add(Vector3Scale(wind, WIND_STRENGTH * dt), particle.force);

        // Apply gravity in Y direction
        particle.force.y += particle.mass * G;

        Vector3 acceleration = Vector3Scale(particle.force, 1.0f / particle.mass);

        particle.velocity = Vector3Add(particle.velocity, Vector3Scale(acceleration, dt));

        particle.velocity = Vector3Scale(particle.velocity, 1.0f - DAMPING_FACTOR * dt);

        float speed = Vector3Length(particle.velocity);
        if (speed > MAX_SPEED) {
            particle.velocity = Vector3Scale(Vector3Normalize(particle.velocity), MAX_SPEED);
        }

        // Check for NaN values
        if (std::isnan(particle.velocity.x) || std::isnan(particle.velocity.y) || std::isnan(particle.velocity.z)) {
            particle.velocity = Vector3Zero();
        }

        particle.position = Vector3Add(particle.position, Vector3Scale(particle.velocity, dt));

        // Check for NaN positions and reset if needed
        if (std::isnan(particle.position.x) || std::isnan(particle.position.y) || std::isnan(particle.position.z)) {
            particle.position = {SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f};
            particle.velocity = Vector3Zero();
        }

        particle.force = Vector3Zero();
    }
}
