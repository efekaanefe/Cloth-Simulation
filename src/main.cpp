#include "cloth.h"
#include <raylib.h>

int main() {

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "2D N-Body Simulation");

    while (!WindowShouldClose()) {
        ClearBackground(WHITE);
        float deltaTime = GetFrameTime();

        BeginDrawing();

        DrawFPS(10, 10);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
