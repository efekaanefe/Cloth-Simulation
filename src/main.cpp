#include "cloth.h"
#include "constants.h"
#include <raylib.h>

int main() {

    Cloth cloth = Cloth();

    float totalFrames = 0.0f;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Cloth Simulation");

    while (!WindowShouldClose()) {
        ClearBackground(WHITE);
        float deltaTime = GetFrameTime();

        BeginDrawing();

        cloth.Update(deltaTime, totalFrames);
        cloth.Draw();

        DrawFPS(10, 10);
        EndDrawing();

        totalFrames++;
    }

    CloseWindow();
    return 0;
}
