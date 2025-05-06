#include "cloth.h"
#include "constants.h"
#include "raylib.h"
#include <cmath>
#include <raymath.h>

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "3D Cloth Simulation");
    SetTargetFPS(60);

    // Initialize camera to face the cloth from the front (normal to the cloth)
    Camera3D camera = { 0 };
    camera.position = (Vector3){ SCREEN_WIDTH/2.0f, SCREEN_HEIGHT/2.0f, CAMERA_DISTANCE };
    camera.target = (Vector3){ SCREEN_WIDTH/2.0f, SCREEN_HEIGHT/2.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, -1.0f, 0.0f }; // Keep Y-up orientation
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    Cloth cloth;

    float frameTime = 0.0f;
    float totalFrames = 0.0f;
    float cameraAngle = 0.0f;

    // Main game loop
    while (!WindowShouldClose()) {
        frameTime = GetFrameTime();
        totalFrames += frameTime;

        // Update camera position (rotate around cloth)
        if (IsKeyDown(KEY_RIGHT)) cameraAngle += CAMERA_ROTATION_SPEED * frameTime;
        if (IsKeyDown(KEY_LEFT)) cameraAngle -= CAMERA_ROTATION_SPEED * frameTime;

        // Calculate camera position - rotated 90 degrees to start (by setting initial angle)
        float adjustedAngle = cameraAngle + PI/2.0f; // 90 degrees offset
        camera.position.x = SCREEN_WIDTH/2.0f + cosf(adjustedAngle) * CAMERA_DISTANCE;
        camera.position.z = sinf(adjustedAngle) * CAMERA_DISTANCE;
        
        BeginDrawing();
        ClearBackground(RAYWHITE);

        BeginMode3D(camera);
        
        cloth.Update(frameTime, totalFrames);
        cloth.Draw();
        
        EndMode3D();

        // Draw UI
        DrawFPS(10, 10);
        DrawText("Use LEFT/RIGHT arrow keys to rotate camera", 10, 40, 20, DARKGRAY);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
