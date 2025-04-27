#include <raylib.h>
#include "constants.h"
#include "cloth.h"


int main() {
    
    Cloth cloth = Cloth();

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Cloth Simulation");

    while (!WindowShouldClose()) {
        ClearBackground(WHITE);
        float deltaTime = GetFrameTime();

        BeginDrawing();

        cloth.Draw();

        DrawFPS(10, 10);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
