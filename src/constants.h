#pragma once
#include <raylib.h>

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 1000;

const int NUM_VERTICES_WIDTH = 31;
const int NUM_VERTICES_HEIGHT = 31;
const int NUM_PARTICLES = NUM_VERTICES_HEIGHT * NUM_VERTICES_HEIGHT;

const float PARTICLE_MASS = 0.5;
const float PARTICLE_RADIUS = 0.0f;

const Color CLOTH_COLOR = DARKGRAY;
const Color FIXED_PARTICLE_COLOR = RED;
const float LINE_THICKNESS = 3.0f;

const float SPRING_K = 30.0f;
const float DAMPING_FACTOR = 0.5f;
const float G = 3.0f;
const float MIN_DISTANCE = 2.0f;
const float MAX_FORCE = 300.0f;
const float MAX_SPEED = 50.0f;

const float WIND_STRENGTH = 10.0f;

// Camera settings
const float CAMERA_DISTANCE = 400.0f;
const float CAMERA_ROTATION_SPEED = 1.0f;
