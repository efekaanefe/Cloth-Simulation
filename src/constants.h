#pragma once
#include <raylib.h>

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 1000;

const int NUM_VERTICES_WIDTH = 71;
const int NUM_VERTICES_HEIGHT = 71;
const int NUM_PARTICLES = NUM_VERTICES_HEIGHT * NUM_VERTICES_HEIGHT;

const float PARTICLE_MASS = 0.1;
const float PARTICLE_RADIUS = 0.0f;

const Color CLOTH_COLOR = DARKGRAY;
const Color FIXED_PARTICLE_COLOR = RED;
const float LINE_THICKNESS = 1.0f;

const float SPRING_K = 30.0f;
const float DAMPING_FACTOR = 0.5f;
const float G = 9.81;
const float MIN_DISTANCE = 2.0f;
const float MAX_FORCE = 500.0f;
const float MAX_SPEED = 100.0f;

const float WIND_STRENGTH = 50.0f;
