#include "cloth.h"
#include "constants.h"
#include <raylib.h>

int main() {

    Cloth cloth = Cloth();

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Cloth Simulation");

    while (!WindowShouldClose()) {
        ClearBackground(WHITE);
        float deltaTime = GetFrameTime();

        BeginDrawing();

        cloth.Update(0.01f);
        cloth.Draw();

        DrawFPS(10, 10);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
